#include "database.h"
#include <iostream>

void fetchCameras(sqlite3* db, std::vector<Camera>& cameras) {
    const char* query = "SELECT camera_id, camera_name, camera_ip FROM cameras";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Camera camera = {
            sqlite3_column_int(stmt, 0),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))
        };
        cameras.push_back(camera);
    }

    sqlite3_finalize(stmt);
}

void fetchVideos(sqlite3* db, std::vector<Video>& videos) {
    const char* query = "SELECT video_id, camera_id, video_name, video_storage, start_time, end_time FROM videos";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
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
        videos.push_back(video);
    }

    sqlite3_finalize(stmt);
}

void fetchAreas(sqlite3* db, std::vector<Area>& areas) {
    const char* query = "SELECT area_id, camera_id, area_coordinate, area_name FROM areas";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Area area = {
            sqlite3_column_int(stmt, 0),
            sqlite3_column_int(stmt, 1),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))
        };
        areas.push_back(area);
    }

    sqlite3_finalize(stmt);
}

void fetchData(sqlite3* db, std::vector<Data>& data) {
    const char* query = "SELECT data_id, area_id, people_count, start_time, end_time FROM data";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Data record = {
            sqlite3_column_int(stmt, 0),
            sqlite3_column_int(stmt, 1),
            sqlite3_column_int(stmt, 2),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))
        };
        data.push_back(record);
    }

    sqlite3_finalize(stmt);
}

