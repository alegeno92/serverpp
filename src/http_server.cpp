#include <ixwebsocket/IXHttpServer.h>
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>


ix::HttpServer server(80, "0.0.0.0");
std::string delimiter = "/";
using namespace ix;

int main() {

    auto res = server.listen();
    if (!res.first) {
        std::cerr << res.second << std::endl;
        return 1;
    }

    server.setOnConnectionCallback(
            [](const HttpRequestPtr &request,
               const std::shared_ptr<ConnectionState> & /*connectionState*/) -> HttpResponsePtr {
                // Build a string for the response
                std::string ss = request->uri;
                std::string filename = "./templates/" + ss.erase(0, 1);
                std::ifstream requested_page(filename);

                if (!requested_page.is_open()) {
                    return std::make_shared<HttpResponse>(404, "NOT FOUND", HttpErrorCode::MissingLocation,
                                                          WebSocketHttpHeaders(), "Page Not Found");
                }

                std::stringstream file_content;
                file_content << requested_page.rdbuf();
                std::string content = file_content.str();
                std::cout << filename << std::endl;

                WebSocketHttpHeaders headers;

                if (filename.find(".js") != -1) {
                    headers["Content-Type"] = "application/javascript";
                } else if (filename.find(".css") != -1) {
                    headers["Content-Type"] = "stylesheet";
                } else if (filename.find(".html") != -1) {
                    headers["Content-Type"] = "text/html";
                }

                return std::make_shared<HttpResponse>(200, "OK", HttpErrorCode::Ok, headers, content);
            }
    );
    server.start();
    server.wait();
    return 1;
}
