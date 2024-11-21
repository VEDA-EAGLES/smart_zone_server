#include <nlohmann/json.hpp>

#include "database.h"
#include <iostream>

using json = nlohmann::json;

std::string fetchCameras(sqlite3* db) {
    if (sqlite3_open("Eagles.db", &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return "";
    }
    const char* query = "SELECT camera_id, camera_name, camera_ip FROM cameras";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return " ";
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
    std::string cameras_str = cameras.dump(4);
    // std::cout << cameras_str << std::endl;
    return cameras_str;
}

std::string fetchVideos(sqlite3* db) {
    const char* query = "SELECT * FROM videos";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
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

std::string fetchAreas(sqlite3* db) {
    const char* query = "SELECT * FROM areas";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return "";
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Area area = {
            sqlite3_column_int(stmt, 0),
            sqlite3_column_int(stmt, 1),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))
        };
    }

    sqlite3_finalize(stmt);
    return "";
}

void insertAreas(sqlite3* db, std::string request) {
    if (sqlite3_open("Eagles.db", &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    const char* query = "INSERT INTO areas (data) VALUES (?)";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    sqlite3_bind_text(stmt, 1, request.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return;
}


std::string fetchData(sqlite3* db) {
    if (sqlite3_open("Eagles.db", &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return "";
    }
    const char* query = "SELECT * FROM data";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
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
    std::string data_str = datas.dump(4);
    return data_str;
}

std::string fetchData(sqlite3* db, int start, int end) {
    if (sqlite3_open("Eagles.db", &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return "";
    }
    const char* query = "SELECT * FROM data WHERE start_time >= ? AND end_time <= ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
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
    std::string data_str = datas.dump(4);
    // std::cout << cameras_str << std::endl;
    return data_str;
}