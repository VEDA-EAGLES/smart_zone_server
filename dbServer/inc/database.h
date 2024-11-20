#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include <sqlite3.h>

struct Camera {
    int camera_id;
    std::string camera_name;
    std::string camera_ip;
};

struct Video {
    int video_id;
    int camera_id;
    std::string video_name;
    std::string video_storage;
    std::string start_time;
    std::string end_time;
};

struct Area {
    int area_id;
    int camera_id;
    std::string area_coordinate;
    std::string area_name;
};

struct Data {
    int data_id;
    int area_id;
    int people_count;
    std::string start_time;
    std::string end_time;
};

void fetchCameras(sqlite3* db, std::vector<Camera>& cameras);
void fetchVideos(sqlite3* db, std::vector<Video>& videos);
void fetchAreas(sqlite3* db, std::vector<Area>& areas);
void fetchData(sqlite3* db, std::vector<Data>& data);

#endif // DATABASE_H

