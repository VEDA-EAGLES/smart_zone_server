#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include <iostream>
#include <sqlite3.h>
#include "json.hpp"

using namespace std;
using namespace nlohmann;

class MainDB
{
private:

public:
    MainDB();
    ~MainDB();
    string fetchCameras();
    string fetchVideos(sqlite3* db);
    string selectAllfromAwhereBequalC(string A, string B, string C);
    void insertAreas(int camera_id, string area_name, int x, int y, int width, int height);
    void insertVideo(int camera_id, string video_name, string video_storage, int start_time, int end_time);
    string fetchData();
    string fetchData(int start, int end);
    void deleteArea(int camera_id);
};

#endif // DATABASE_H

