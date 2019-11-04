//
// Created by alessandro on 28/10/19.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include "http_server.h"

HttpServer::HttpServer(const int port, const std::string &host) {
    _server = new ix::HttpServer(port, host);
    _server->setOnConnectionCallback([](const ix::HttpRequestPtr &request,
                                        const std::shared_ptr<ix::ConnectionState> & /*connectionState*/) -> ix::HttpResponsePtr {
        // Build a string for the response
        std::cout << "HTTP: [" << request->method << "]" << request->uri;
        std::string ss = request->uri;
        std::string filename = "../public/" + ss.erase(0, 1);
        std::ifstream requestedPage(filename);

        if (!requestedPage.is_open()) {
            std::cout << " - FAIL" << std::endl;
            return std::make_shared<ix::HttpResponse>(404, "NOT FOUND", ix::HttpErrorCode::MissingLocation,
                                                      ix::WebSocketHttpHeaders(), "Page Not Found");
        }
        std::cout << " - OK" << std::endl;

        std::stringstream file_content;
        file_content << requestedPage.rdbuf();
        std::string content = file_content.str();

        ix::WebSocketHttpHeaders headers;

        if (filename.find(".js") != -1) {
            headers["Content-Type"] = "application/javascript";
        } else if (filename.find(".css") != -1) {
            headers["Content-Type"] = "text/css";
        } else if (filename.find(".html") != -1) {
            headers["Content-Type"] = "text/html";
        }

        return std::make_shared<ix::HttpResponse>(200, "OK", ix::HttpErrorCode::Ok, headers, content);
    });
}

HttpServer::~HttpServer() {
    _server->stop();
}

bool HttpServer::start() {
    auto res = _server->listen();
    if (!res.first) {
        std::cerr << res.second << std::endl;
        return false;
    }
    _server->start();
    _server->wait();
    return true;
}
