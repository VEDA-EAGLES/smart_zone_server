#include "HttpServer.h"
#include <thread>
#include <chrono>

using namespace std;

int main() {
    // Create and run servers
    Server server1("192.168.0.95", 8080);
    Server server2("192.168.0.95", 8081);

    thread t1(&Server::run, &server1);
    thread t2(&Server::run, &server2);

    // Main loop for data processing
    while (true) {
        this_thread::sleep_for(chrono::seconds(5));

        cout << "Periodic processing tasks can go here." << endl;
    }

    t1.join();
    t2.join();

    return 0;
}
