#include "HttpServer.h"

using namespace std;
using boost::asio::ip::tcp;

Server::Server(const string& host, unsigned short port)
    : host_(host), port_(port) {}

void Server::run() {
    boost::asio::io_service io_service;
    tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_);
    tcp::acceptor acceptor(io_service, endpoint);

    {
        lock_guard<mutex> lock(cout_mutex_);
        cout << "Server running on " << host_ << ":" << port_ << endl;
    }

    while (true) {
        tcp::socket socket(io_service);
        acceptor.accept(socket);
        {
            lock_guard<mutex> lock(cout_mutex_);
            cout << "New client connected: " << socket.remote_endpoint() << endl;
        }
        // Handle requests in a separate thread
        thread(&RequestHandler::handleRequest, RequestHandler(), move(socket)).detach();
    }
}

