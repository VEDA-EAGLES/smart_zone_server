#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include <mutex>

using boost::asio::ip::tcp;
using namespace std;

const string HTTP_OK = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
const string HTTP_BAD_REQUEST = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\n";
const string HTTP_NOT_FOUND = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";

// Global vector to store data
vector<string> data_store;
mutex data_mutex;
mutex cout_mutex; // Protect std::cout

void handle_request(tcp::socket socket) {
    char data[1024];
    boost::system::error_code error;
    // Read data from client
    size_t length = socket.read_some(boost::asio::buffer(data), error);
    if (error && error != boost::asio::error::eof)
    {
        throw boost::system::system_error(error);
    }
    stringstream request_stream;
    request_stream.write(data, length);
    string request = request_stream.str();
    cout << "request : " << request << endl;
    // Handle GET and POST requests
    if (request.find("GET") == 0) {
        string response = HTTP_OK + "GET request processed\n";
        boost::asio::write(socket, boost::asio::buffer(response), error);
    }
    else if (request.find("POST") == 0) {
        string response = HTTP_OK + "POST request processed\n";

        // Simulating data storage for POST requests
        {
            lock_guard<mutex> lock(data_mutex);
            data_store.push_back(request); // Store incoming data (could be request body or something else)
        }

        boost::asio::write(socket, boost::asio::buffer(response), error);
    }
    else {
        string response = HTTP_BAD_REQUEST + "Invalid request\n";
        boost::asio::write(socket, boost::asio::buffer(response), error);
    }
}

void server_thread(const string& host, unsigned short port) {
    boost::asio::io_service io_service;
    tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
    tcp::acceptor acceptor(io_service, endpoint);
    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "Server running on " << host << ":" << port << endl;
    }
    while (true) {
        tcp::socket socket(io_service);
        acceptor.accept(socket);
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "New client connected: " << socket.remote_endpoint() << endl;
        }

        // Handle the request in a separate thread
        thread(handle_request, move(socket)).detach();
    }
}

int main() {
    // Start the server threads for different ports
    thread server1(server_thread, "192.168.0.95", 8080);
    thread server2(server_thread, "192.168.0.95", 8081);

    // Main loop for processing data
    while (true) {
        this_thread::sleep_for(chrono::seconds(5)); // Simulate periodic data processing

        // Lock the data store and process it (printing in this case)
        {
            lock_guard<mutex> lock(data_mutex);
            if (!data_store.empty()) {
                lock_guard<mutex> lock(cout_mutex);
                cout << "Data stored so far:" << endl;
                for (const auto& data : data_store) {
                    cout << data << endl;
                }
                // Optionally clear the data store after processing
                data_store.clear();
            } else {
                lock_guard<mutex> lock(cout_mutex);
                cout << "No data to process." << endl;
            }
        }
    }

    server1.join();
    server2.join();

    return 0;
}
