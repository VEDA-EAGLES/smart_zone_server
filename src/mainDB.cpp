
#include "mainDB.h"

MainDB::MainDB() {}
MainDB::~MainDB() {}

string MainDB::fetchCameras() {
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
    }
        return "";
    json cameras = json::array(); // JSON 배열 생성

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        json camera = {
            {"camera_id", sqlite3_column_int(stmt, 0)},
            {"camera_name", reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))},
            {"camera_ip", reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))}
        };
        cameras.push_back(camera); // JSON 배열에 추가
    }

    json result;
    result["cameras"] = cameras;

    sqlite3_finalize(stmt);
    string cameras_str = result.dump();

    // 데이터베이스 연결 닫기
    sqlite3_close(db);

    return cameras_str;
}

string MainDB::fetchPeopleCnt(int cameraId) {
    sqlite3* db;

    if (sqlite3_open("../Eagles.db", &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return "";
    }

    const char* query = "SELECT * FROM people_count WHERE camera_id = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }

    sqlite3_bind_int(stmt, 1, cameraId); 

    json datas = json::array(); // JSON 배열 생성
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        json data = {
            {"data_id", sqlite3_column_int(stmt, 0)},
            {"area_id", sqlite3_column_int(stmt, 1)},
            {"people_count", sqlite3_column_int(stmt, 2)},
            {"start_time", sqlite3_column_int(stmt, 3)},
            {"end_time", sqlite3_column_int(stmt, 4)}
        };
        datas.push_back(data); // JSON 배열에 추가
    }

    json result;
    result["data"] = datas;

    sqlite3_finalize(stmt);
    string data_str = result.dump();
    sqlite3_close(db);
    return data_str;
}

string MainDB::fetchPeopleCnt(int cameraId, int start, int end) {
    sqlite3* db;

    if (sqlite3_open("../Eagles.db", &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return "";
    }

    const char* query = "SELECT * FROM people_count WHERE camera_id = ? AND start_time >= ? AND end_time <= ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }

    sqlite3_bind_int(stmt, 1, cameraId); 
    sqlite3_bind_int(stmt, 2, start); 
    sqlite3_bind_int(stmt, 3, end);   
    json datas = json::array(); // JSON 배열 생성
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        json data = {
            {"data_id", sqlite3_column_int(stmt, 0)},
            {"area_id", sqlite3_column_int(stmt, 1)},
            {"people_count", sqlite3_column_int(stmt, 2)},
            {"start_time", sqlite3_column_int(stmt, 3)},
            {"end_time", sqlite3_column_int(stmt, 4)}
        };
        datas.push_back(data); // JSON 배열에 추가
    }

    json result;
    result["data"] = datas;

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    string data_str = result.dump();
    return data_str;
}

string MainDB::fetchPeopleStay(int cameraId) {
    sqlite3* db;

    if (sqlite3_open("../Eagles.db", &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return "";
    }

    const char* query = "SELECT * FROM people_stay WHERE camera_id = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }

    sqlite3_bind_int(stmt, 1, cameraId); 

    json datas = json::array(); // JSON 배열 생성
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        json data = {
            {"data_id", sqlite3_column_int(stmt, 0)},
            {"area_id", sqlite3_column_int(stmt, 1)},
            {"stay_time", sqlite3_column_int(stmt, 2)},
            {"start_time", sqlite3_column_int(stmt, 3)},
            {"end_time", sqlite3_column_int(stmt, 4)}
        };
        datas.push_back(data); // JSON 배열에 추가
    }

    json result;
    result["data"] = datas;

    sqlite3_finalize(stmt);
    string data_str = result.dump();
    sqlite3_close(db);
    return data_str;
}

string MainDB::fetchPeopleStay(int cameraId, int start, int end) {
    sqlite3* db;

    if (sqlite3_open("../Eagles.db", &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return "";
    }

    const char* query = "SELECT * FROM people_stay WHERE camera_id = ? AND start_time >= ? AND end_time <= ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }

    sqlite3_bind_int(stmt, 1, cameraId); 
    sqlite3_bind_int(stmt, 2, start); 
    sqlite3_bind_int(stmt, 3, end);   
    json datas = json::array(); // JSON 배열 생성
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        json data = {
            {"data_id", sqlite3_column_int(stmt, 0)},
            {"area_id", sqlite3_column_int(stmt, 1)},
            {"stay_time", sqlite3_column_int(stmt, 2)},
            {"start_time", sqlite3_column_int(stmt, 3)},
            {"end_time", sqlite3_column_int(stmt, 4)}
        };
        datas.push_back(data); // JSON 배열에 추가
    }

    json result;
    result["data"] = datas;

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    string data_str = result.dump();
    return data_str;
}

string MainDB::fetchPeopleMove(int cameraId) {
    sqlite3* db;

    if (sqlite3_open("../Eagles.db", &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return "";
    }

    const char* query = "SELECT * FROM people_move WHERE camera_id = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }

    sqlite3_bind_int(stmt, 1, cameraId); 

    json datas = json::array(); // JSON 배열 생성
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        json data = {
            {"data_id", sqlite3_column_int(stmt, 0)},
            {"from_area_id", sqlite3_column_int(stmt, 1)},
            {"to_area_id", sqlite3_column_int(stmt, 2)},
            {"count", sqlite3_column_int(stmt, 3)},
            {"start_time", sqlite3_column_int(stmt, 4)},
            {"end_time", sqlite3_column_int(stmt, 5)}
        };
        datas.push_back(data); // JSON 배열에 추가
    }

    json result;
    result["data"] = datas;

    sqlite3_finalize(stmt);
    string data_str = result.dump();
    sqlite3_close(db);
    return data_str;
}

string MainDB::fetchPeopleMove(int cameraId, int start, int end) {
    sqlite3* db;

    if (sqlite3_open("../Eagles.db", &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return "";
    }

    const char* query = "SELECT * FROM people_move WHERE camera_id = ? AND start_time >= ? AND end_time <= ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }

    sqlite3_bind_int(stmt, 1, cameraId); 
    sqlite3_bind_int(stmt, 2, start); 
    sqlite3_bind_int(stmt, 3, end);   
    json datas = json::array(); // JSON 배열 생성
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        json data = {
            {"data_id", sqlite3_column_int(stmt, 0)},
            {"from_area_id", sqlite3_column_int(stmt, 1)},
            {"to_area_id", sqlite3_column_int(stmt, 2)},
            {"count", sqlite3_column_int(stmt, 3)},
            {"start_time", sqlite3_column_int(stmt, 4)},
            {"end_time", sqlite3_column_int(stmt, 5)}
        };
        datas.push_back(data); // JSON 배열에 추가
    }

    json result;
    result["data"] = datas;

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    string data_str = result.dump();
    return data_str;
}

json MainDB::selectAllfromAwhereBequalC(string A, string B, string C) {
    sqlite3* db;
    if (sqlite3_open("../Eagles.db", &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return "";
    }
   
    string query = "SELECT * FROM " + A + " WHERE " + B + " = '" + C + "'";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }

    json jsonObject = json::object();
    json jsonArray = json::array();

    // 쿼리 실행 및 결과 처리
    int result = sqlite3_step(stmt);
    while (result == SQLITE_ROW) {
        // 컬럼 개수 가져오기
        int cols = sqlite3_column_count(stmt);

        // 각 컬럼의 값을 JSON 객체에 추가
        for (int i = 0; i < cols; i++) {
            string colName = sqlite3_column_name(stmt, i);

            // 컬럼 타입에 따라 값을 가져오기
            switch (sqlite3_column_type(stmt, i)) {
                case SQLITE_INTEGER: {
                    int colValue = sqlite3_column_int(stmt, i);
                    jsonObject[colName] = colValue;
                    break;
                }
                case SQLITE_FLOAT: {
                    double colValue = sqlite3_column_double(stmt, i);
                    jsonObject[colName] = colValue;
                    break;
                }
                case SQLITE_TEXT: {
                    string colValue = (char*)sqlite3_column_text(stmt, i);
                    jsonObject[colName] = colValue;
                    break;
                }
                case SQLITE_BLOB: {
                    // BLOB 데이터 처리
                    break;
                }
                case SQLITE_NULL: {
                    jsonObject[colName] = nullptr;
                    break;
                }
            }
        }

        jsonArray.push_back(jsonObject);
        result = sqlite3_step(stmt);
    }

    if (result != SQLITE_DONE) {
        cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return jsonArray;
}

json MainDB::selectAllfromAwhereBequalC(string A, string B, int C) {
    sqlite3* db;
    if (sqlite3_open("../Eagles.db", &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return "";
    }
   
    string query = "SELECT * FROM " + A + " WHERE " + B + " = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }
    sqlite3_bind_int(stmt, 1, C);

    json jsonObject = json::object();
    json jsonArray = json::array();

    // 쿼리 실행 및 결과 처리
    int result = sqlite3_step(stmt);
    while (result == SQLITE_ROW) {
        // 컬럼 개수 가져오기
        int cols = sqlite3_column_count(stmt);

        // 각 컬럼의 값을 JSON 객체에 추가
        for (int i = 0; i < cols; i++) {
            string colName = sqlite3_column_name(stmt, i);

            // 컬럼 타입에 따라 값을 가져오기
            switch (sqlite3_column_type(stmt, i)) {
                case SQLITE_INTEGER: {
                    int colValue = sqlite3_column_int(stmt, i);
                    jsonObject[colName] = colValue;
                    break;
                }
                case SQLITE_FLOAT: {
                    double colValue = sqlite3_column_double(stmt, i);
                    jsonObject[colName] = colValue;
                    break;
                }
                case SQLITE_TEXT: {
                    string colValue = (char*)sqlite3_column_text(stmt, i);
                    jsonObject[colName] = colValue;
                    break;
                }
                case SQLITE_BLOB: {
                    // BLOB 데이터 처리
                    break;
                }
                case SQLITE_NULL: {
                    jsonObject[colName] = nullptr;
                    break;
                }
            }
        }

        // JSON 객체를 배열에 추가
        jsonArray.push_back(jsonObject);
        result = sqlite3_step(stmt);
    }
    if (result != SQLITE_DONE) {
        cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return jsonArray;
}

void MainDB::insertAreas(int camera_id, string area_name, int x, int y, int width, int height, string color) {
    sqlite3* db;
    if (sqlite3_open("../Eagles.db", &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return;
    }

    const char* query = "INSERT INTO areas (camera_id, area_name, x, y, width, height, color) VALUES (?, ?, ?, ?, ?, ?, ?)"; 

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, camera_id);
    sqlite3_bind_text(stmt, 2, area_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, x);
    sqlite3_bind_int(stmt, 4, y);
    sqlite3_bind_int(stmt, 5, width);
    sqlite3_bind_int(stmt, 6, height);
    sqlite3_bind_text(stmt, 7, color.c_str(), -1, SQLITE_TRANSIENT);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return;
}

// 해당 영역 삭제
void MainDB::deleteArea(int camera_id, int area_id) {
    sqlite3* db;
    if (sqlite3_open("../Eagles.db", &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // 외래 키 제약 조건 활성화
    char* errmsg;
    if (sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, &errmsg) != SQLITE_OK) {
        cerr << "Failed to enable foreign keys: " << errmsg << endl;
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return;
    }

    const char* query = "DELETE FROM areas WHERE camera_id = ? AND area_id = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // 각 컬럼에 해당하는 값 바인딩
    sqlite3_bind_int(stmt, 1, camera_id);
    sqlite3_bind_int(stmt, 2, area_id);
    
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return;
}
// 영역 전체삭제
void MainDB::deleteArea(int camera_id) {
    sqlite3* db;
    if (sqlite3_open("../Eagles.db", &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // 외래 키 제약 조건 활성화
    char* errmsg;
    if (sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, &errmsg) != SQLITE_OK) {
        cerr << "Failed to enable foreign keys: " << errmsg << endl;
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return;
    }

    const char* query = "DELETE FROM areas WHERE camera_id = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // 각 컬럼에 해당하는 값 바인딩
    sqlite3_bind_int(stmt, 1, camera_id);
    
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return;
}