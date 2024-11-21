#include "RequestHandler.h"
#include "database.h"

using boost::asio::ip::tcp;

sqlite3* Eaglesdb;

// Static member initialization
std::vector<std::string> RequestHandler::data_store;
std::mutex RequestHandler::data_mutex;
const std::string RequestHandler::HTTP_OK = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
const std::string RequestHandler::HTTP_BAD_REQUEST = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\n";

RequestHandler::RequestHandler() {}

void RequestHandler::handleRequest(tcp::socket socket) {
    char data[1024];
    boost::system::error_code error;

    size_t length = socket.read_some(boost::asio::buffer(data), error);
    if (error && error != boost::asio::error::eof) {
        throw boost::system::system_error(error);
    }
    std::string request(data, length);

    if (request.find("GET /device/all") == 0) {
        getDeviceAll(socket);
    } else if (request.find("GET /peoplecnt/all") == 0) {
        getPeopleAll(socket);
    } else if (request.find("GET /peoplecnt/unit") == 0) {
        size_t startPos = request.find("start=") + 6;
        size_t endPos = request.find("&", startPos);
        std::string startTimeStr = request.substr(startPos, endPos - startPos);
        size_t endStartPos = request.find("end=") + 4;
        size_t endEndPos = request.find(" ", endStartPos);
        std::string endTimeStr = request.substr(endStartPos, endEndPos - endStartPos);
        getPeopleInRange(socket, std::stoi(startTimeStr), std::stoi(endTimeStr));
    } else if (request.find("POST /area/insert") == 0) {
        postAreaInsert(socket, request.substr(request.find("\r\n\r\n") + 4));
    } else {
        sendResponse(socket, HTTP_BAD_REQUEST + "Invalid request\n");
    }
}
void RequestHandler::sendResponse(tcp::socket& socket, const std::string& response) {
    boost::system::error_code error;
    std::cout << response << std::endl;
    boost::asio::write(socket, boost::asio::buffer(response), error);
}

void RequestHandler::getDeviceAll(boost::asio::ip::tcp::socket& socket) {
    std::string response = HTTP_OK + fetchCameras(Eaglesdb);
    sendResponse(socket, response);
}
void RequestHandler::getPeopleAll(boost::asio::ip::tcp::socket& socket) {
    std::string response = HTTP_OK + fetchData(Eaglesdb);
    sendResponse(socket, response);
}
void RequestHandler::getPeopleInRange(boost::asio::ip::tcp::socket& socket, int start, int end) {
    std::string response = HTTP_OK + fetchData(Eaglesdb, start, end);
    sendResponse(socket, response);
}

void RequestHandler::postAreaInsert(boost::asio::ip::tcp::socket& socket, const std::string& request) {
    std::cout << request << std::endl;
    insertAreas(Eaglesdb, request);
    std::string response = HTTP_OK + "POST request processed\n";
    sendResponse(socket, response);
}

// void RequestHandler::delAreaAll(int Area_id)
// std::string RequestHandler::postAreaInsert_C(boost::asio::ip::tcp::socket& socket, const std::string& request);
// void RequestHandler::delAreaAll_C(int Area_id)
// std::string RequestHandler::getCamData(boost::asio::ip::tcp::socket& socket);