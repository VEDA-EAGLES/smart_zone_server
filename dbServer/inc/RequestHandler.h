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
    static std::mutex db_mutex;
    static const std::string HTTP_OK;
    static const std::string HTTP_BAD_REQUEST;
    // Client to Server
    void sendResponse(boost::asio::ip::tcp::socket& socket, const std::string& response);
    void getDeviceAll(boost::asio::ip::tcp::socket& socket);
    void getPeopleAll(boost::asio::ip::tcp::socket& socket);
    void getPeopleInRange(boost::asio::ip::tcp::socket& socket, int start, int end);
    void postAreaInsert(boost::asio::ip::tcp::socket& socket, const std::string& request);
    void delAreaAll(int Area_id);
    // CAM to Server
    std::string postAreaInsert_C(boost::asio::ip::tcp::socket& socket, const std::string& request);
    void delAreaAll_C(int Area_id);
    std::string getCamData(boost::asio::ip::tcp::socket& socket);


};

#endif // REQUEST_HANDLER_H

