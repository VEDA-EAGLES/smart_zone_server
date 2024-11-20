#include "RequestHandler.h"

using namespace std;
using boost::asio::ip::tcp;

// Static member initialization
vector<string> RequestHandler::data_store;
mutex RequestHandler::data_mutex;
const string RequestHandler::HTTP_OK = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
const string RequestHandler::HTTP_BAD_REQUEST = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\n";

RequestHandler::RequestHandler() {}

void RequestHandler::handleRequest(tcp::socket socket) {
    char data[1024];
    boost::system::error_code error;

    size_t length = socket.read_some(boost::asio::buffer(data), error);
    if (error && error != boost::asio::error::eof) {
        throw boost::system::system_error(error);
    }

    string request(data, length);

    if (request.find("GET") == 0) {
        processGet(socket);
    } else if (request.find("POST") == 0) {
        processPost(socket, request);
    } else {
        sendResponse(socket, HTTP_BAD_REQUEST + "Invalid request\n");
    }
}

void RequestHandler::processGet(tcp::socket& socket) {
    string response = HTTP_OK + "GET request processed\n";
    sendResponse(socket, response);
}

void RequestHandler::processPost(tcp::socket& socket, const string& request) {
    {
        lock_guard<mutex> lock(data_mutex);
        data_store.push_back(request);
    }
    string response = HTTP_OK + "POST request processed\n";
    sendResponse(socket, response);
}

void RequestHandler::sendResponse(tcp::socket& socket, const string& response) {
    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer(response), error);
}

