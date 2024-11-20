#include "database.h"
#include "HttpServer.h"
#include <thread>
#include <chrono>

Camera camera;
Video videos;
Area areas;
Data data;

using namespace std;

int main() {
    sqlite3* db;
    // db open
    if (sqlite3_open("Eagles.db", &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }
    // Create and run servers
    Server server1("192.168.0.95", 8080);

    thread t1(&Server::run, &server1);

    // Main loop for data processing
    while (true) {
        this_thread::sleep_for(chrono::seconds(5));
        cout << "Periodic processing tasks can go here." << endl;
    }

    t1.join();

    return 0;
}
