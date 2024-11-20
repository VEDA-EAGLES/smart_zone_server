#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <mutex>

class RequestHandler {
public:
    RequestHandler();
    void handleRequest(boost::asio::ip::tcp::socket socket);

private:
    static std::vector<std::string> data_store;
    static std::mutex data_mutex;
    static const std::string HTTP_OK;
    static const std::string HTTP_BAD_REQUEST;

    void processGet(boost::asio::ip::tcp::socket& socket);
    void processPost(boost::asio::ip::tcp::socket& socket, const std::string& request);
    void sendResponse(boost::asio::ip::tcp::socket& socket, const std::string& response);
};

#endif // REQUEST_HANDLER_H

