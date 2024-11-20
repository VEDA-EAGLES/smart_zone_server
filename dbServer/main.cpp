#include "database.h"
#include "HttpServer.h"
#include <thread>
#include <chrono>

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
        std::vector<Camera> cameras;
        std::vector<Video> videos;
        std::vector<Area> areas;
        std::vector<Data> data;
        fetchCameras(db, cameras);
        fetchVideos(db, videos);
        fetchAreas(db, areas);
        fetchData(db, data);
        for (const auto& camera : cameras) {
            std::cout << "Camera: " << camera.camera_name << " (" << camera.camera_ip << ")\n";
        }

        for (const auto& video : videos) {
            std::cout << "Video: " << video.video_name << " (" << video.video_storage << ")\n";
        }

        for (const auto& area : areas) {
            std::cout << "Area: " << area.area_name << " (Coordinate: " << area.area_coordinate << ")\n";
        }

        for (const auto& record : data) {
            std::cout << "Data: " << record.people_count << " people from " << record.start_time << " to " << record.end_time << "\n";
        }

        cout << "Periodic processing tasks can go here." << endl;
    }

    t1.join();

    return 0;
}
