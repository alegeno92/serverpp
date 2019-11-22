FROM  alpine:latest AS paho-c
RUN apk add --no-cache cmake g++ make
RUN apk add --no-cache git libressl-dev
RUN git clone https://github.com/eclipse/paho.mqtt.c.git
WORKDIR paho.mqtt.c
RUN git checkout v1.3.1
RUN cmake -Bbuild -H. -DPAHO_WITH_SSL=ON -DPAHO_ENABLE_TESTING=OFF
RUN cmake --build build/ --target install

FROM paho-c AS paho-cpp
WORKDIR /
RUN apk add --no-cache git libressl-dev
RUN apk add --no-cache --upgrade bash
RUN git clone https://github.com/eclipse/paho.mqtt.cpp.git
WORKDIR /paho.mqtt.cpp
RUN cmake -Bbuild -H. -DPAHO_BUILD_DOCUMENTATION=FALSE -DPAHO_BUILD_SAMPLES=FALSE
RUN cmake --build build/ --target install

FROM paho-cpp AS build-container
RUN apk add --no-cache zlib-dev
COPY . /home/server
WORKDIR /home/server
RUN if [ \-d "./build" ] ; then rm -rvf build  && mkdir build ; else mkdir build ; fi
WORKDIR build
RUN cmake ..
RUN cmake --build . --target ixwebsocket --
RUN cmake --build . --target server --


FROM alpine:latest
RUN apk add --no-cache zlib libressl libstdc++
COPY --from=build-container /paho.mqtt.c/build/src/* /usr/lib/
COPY --from=build-container /paho.mqtt.cpp/build/src/* /usr/lib/
COPY --from=build-container /home/server/build/server ./
CMD ["./server", "./config/config.json"]
