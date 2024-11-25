#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include <iostream>
#include <sqlite3.h>
#include "json.hpp"

using namespace std;
using namespace nlohmann;

struct Camera {
    int camera_id;
    string camera_name;
    string camera_ip;
};

struct Video {
    int video_id;
    int camera_id;
    string video_name;
    string video_storage;
    string start_time;
    string end_time;
};

struct Area {
    int area_id;
    int camera_id;
    string area_name;
    int x,y,width,height;
};

struct Data {
    int data_id;
    int area_id;
    int people_count;
    string start_time;
    string end_time;
};

string fetchCameras();
string fetchVideos(sqlite3* db);
bool checkAreasDup(string areaName);
void insertAreas(int camera_id, string area_name, int x, int y, int width, int height);
void insertVideo(int camera_id, string video_name, string video_storage, int start_time, int end_time);
string fetchData();
string fetchData(int start, int end);
void deleteArea(int camera_id);


#endif // DATABASE_H

