#include <ixwebsocket/IXHttpServer.h>
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>


ix::HttpServer server(80, "0.0.0.0");
std::string delimiter = "/";
using namespace ix;
int main(){

    auto res = server.listen();
    if(!res.first){
        std::cerr << res.second << std::endl;
        return 1;
    }

    server.setOnConnectionCallback(
        [](HttpRequestPtr request,
               std::shared_ptr<ConnectionState> /*connectionState*/) -> HttpResponsePtr
        {
            // Build a string for the response
        std::string ss = request->uri;
        std::string filename = "./templates/" + ss.erase(0, 1);
        std::ifstream requested_page(filename);
        
        if(!requested_page.is_open()){
            return std::make_shared<HttpResponse>(404, "NOT FOUND", HttpErrorCode::MissingLocation, WebSocketHttpHeaders(), "Page Not Found");
        }
    
        std::stringstream file_content;
        file_content << requested_page.rdbuf(); 
        std::string content = file_content.str();
        if(filename.find(".html")==1){ 
            return std::make_shared<HttpResponse>(200, "OK",

                                              HttpErrorCode::Ok,
                                              WebSocketHttpHeaders(),
                                              content);
        }else{
            return std::make_shared<HttpResponse>(200, "OK",

                                              HttpErrorCode::Ok,
                                              std::map("content-type:","application/javascript", CaseInsensitiveLess),
                                              content);
        }
        
        }
    );
    server.start();
    server.wait();
    return 1;
}
