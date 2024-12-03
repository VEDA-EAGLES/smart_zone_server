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
    string fetchPeopleCnt(int cameraId);
    string fetchPeopleCnt(int cameraId, int start, int end);
    string fetchPeopleStay(int cameraId);
    string fetchPeopleStay(int cameraId, int start, int end);
    string fetchPeopleMove(int cameraId);
    string fetchPeopleMove(int cameraId, int start, int end);
    json selectAllfromAwhereBequalC(string A, string B, string C);
    json selectAllfromAwhereBequalC(string A, string B, int C);
    void insertAreas(int camera_id, string area_name, int x, int y, int width, int height, string color);
    void deleteArea(int camera_id, int area_id);
    void deleteArea(int camera_id);
};

#endif // DATABASE_H

