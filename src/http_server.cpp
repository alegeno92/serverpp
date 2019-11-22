//
// Created by Alessandro Genovese on 28/10/19.
// ale.geno92@gmail.com
//

#include <iostream>
#include <fstream>
#include <sstream>
#include "http_server.h"

using namespace std;

http_server::http_server(int port, const std::string &host, const std::string &public_directory) :
        _host(host),
        _port(port),
        _public_directory(public_directory) {
    _server = new ix::HttpServer(port, host);
    _server->setOnConnectionCallback(
            std::bind(&http_server::handle_on_connection, this, std::placeholders::_1, std::placeholders::_2));
}

http_server::~http_server() {
    _server->stop();
}

bool http_server::start() {
    std::cout << "[HTTP] Opening http server on " << _host << ':' << _port << std::endl;
    auto res = _server->listen();
    if (!res.first) {
        std::cerr << res.second << std::endl;
        return false;
    }
    _server->start();
    _server->wait();
    return true;
}

ix::HttpResponsePtr http_server::handle_on_connection(const ix::HttpRequestPtr &request,
                                                      const std::shared_ptr<ix::ConnectionState> &) {
    std::cout << "[HTTP] [" << request->method << "]" << request->uri;
    std::string ss = request->uri == "/" ? "/index.html" : request->uri;
    std::string filename = _public_directory + ss.erase(0, 1);
    std::ifstream requestedPage(filename);

    if (!requestedPage.is_open()) {
        std::cout << " - FAIL" << std::endl;
        return std::make_shared<ix::HttpResponse>(404, "NOT FOUND",
                                                  ix::HttpErrorCode::MissingLocation,
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
}
