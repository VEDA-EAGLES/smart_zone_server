
#include "mainDB.h"

string fetchCameras() {
    sqlite3* db;
    if (sqlite3_open("../Eagles.db", &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return "";
    }
    const char* query = "SELECT camera_id, camera_name, camera_ip FROM cameras";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }
    json cameras = json::array(); // JSON 배열 생성

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        json camera = {
            {"camera_id", sqlite3_column_int(stmt, 0)},
            {"camera_name", reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))},
            {"camera_ip", reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))}
        };
        cameras.push_back(camera); // JSON 배열에 추가
    }
    sqlite3_finalize(stmt);
    string cameras_str = cameras.dump(4);

    // 데이터베이스 연결 닫기
    sqlite3_close(db);

    return cameras_str;
}

string fetchVideos(sqlite3* db) {
    const char* query = "SELECT * FROM videos";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return "";
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Video video = {
            sqlite3_column_int(stmt, 0),
            sqlite3_column_int(stmt, 1),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5))
        };
    }

    sqlite3_finalize(stmt);
    return "";
}

bool checkAreasDup(string areaName) {
    sqlite3* db;
    if (sqlite3_open("../Eagles.db", &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return true;
    }
    string query = "SELECT area_name FROM areas WHERE area_name = '" + areaName + "'";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return true;
    }
    // 중복 areaName 검사
    bool isDuplicate = false; 
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // 중복 체크
        isDuplicate = true; 
        break; 
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return isDuplicate;
}

void insertAreas(int camera_id, string area_name, int x, int y, int width, int height) {
    sqlite3* db;
    if (sqlite3_open("../Eagles.db", &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return;
    }

    const char* query = "INSERT INTO areas (camera_id, area_name, x, y, width, height) VALUES (?, ?, ?, ?, ?, ?)"; 

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // 각 컬럼에 해당하는 값 바인딩
    sqlite3_bind_int(stmt, 1, camera_id);
    sqlite3_bind_text(stmt, 2, area_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, x);
    sqlite3_bind_int(stmt, 4, y);
    sqlite3_bind_int(stmt, 5, width);
    sqlite3_bind_int(stmt, 6, height);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return;
}

string fetchData() {
    sqlite3* db;

    if (sqlite3_open("../Eagles.db", &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return "";
    }

    const char* query = "SELECT * FROM data";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }

    json datas = json::array(); // JSON 배열 생성
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        json data = {
            {"data_id", sqlite3_column_int(stmt, 0)},
            {"area_id", sqlite3_column_int(stmt, 1)},
            {"people_cnt", sqlite3_column_int(stmt, 2)},
            {"start_time", sqlite3_column_int(stmt, 3)},
            {"end_time", sqlite3_column_int(stmt, 4)}
        };
        datas.push_back(data); // JSON 배열에 추가
    }

    sqlite3_finalize(stmt);
    string data_str = datas.dump(4);
    sqlite3_close(db);
    return data_str;
}

string fetchData(int start, int end) {
    sqlite3* db;

    if (sqlite3_open("../Eagles.db", &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return "";
    }

    const char* query = "SELECT * FROM data WHERE start_time >= ? AND end_time <= ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }

    sqlite3_bind_int(stmt, 1, start); // Bind start to the first placeholder
    sqlite3_bind_int(stmt, 2, end);   // Bind end to the second placeholder
    json datas = json::array(); // JSON 배열 생성
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        json data = {
            {"data_id", sqlite3_column_int(stmt, 0)},
            {"area_id", sqlite3_column_int(stmt, 1)},
            {"people_cnt", sqlite3_column_int(stmt, 2)},
            {"start_time", sqlite3_column_int(stmt, 3)},
            {"end_time", sqlite3_column_int(stmt, 4)}
        };
        datas.push_back(data); // JSON 배열에 추가
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    string data_str = datas.dump(4);
    return data_str;
}