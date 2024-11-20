#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include "RequestHandler.h"

class Server {
public:
    Server(const std::string& host, unsigned short port);
    void run();

private:
    std::string host_;
    unsigned short port_;
    std::mutex cout_mutex_;
};

#endif // HTTP_SERVER_H

