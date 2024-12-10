
#include "httpServer.h"

HTTPServer::HTTPServer(string ip, int port) 
{
    this->ip = ip;
    this->port = port;
}

HTTPServer::~HTTPServer() {}

void HTTPServer::setClientResponse()
{
    /* GET */
    server.Get("/device/all", [this](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        // 클라이언트에게 카메라 정보 보내기
        string jsonBody = mainDB.fetchCameras();
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});

    server.Get("/peoplecnt/all", [this](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        // 클라이언트에게 사람수(전체) response
        if (req.has_param("camera_id")) {
            int cameraId = stoi(req.get_param_value("camera_id"));
            string jsonBody = mainDB.fetchPeopleCnt(cameraId);
            res.set_content(jsonBody, "application/json");
        }
        mtx.unlock();
	});

    server.Get("/peoplecnt/unit", [this](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        // 클라이언트에게 기간에 따른 사람수(시작/끝) 정보 response
        if (req.has_param("camera_id") && req.has_param("start") && req.has_param("end")) {
            int cameraId = stoi(req.get_param_value("camera_id"));
            string start = req.get_param_value("start"), end = req.get_param_value("end");
            string jsonBody = mainDB.fetchPeopleCnt(cameraId,stoi(start),stoi(end));
            res.set_content(jsonBody, "application/json");
        }
        mtx.unlock();
	});

    server.Get("/peoplestay/all", [this](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        // 클라이언트에게 사람수(전체) response
        if (req.has_param("camera_id")) {
            int cameraId = stoi(req.get_param_value("camera_id"));
            string jsonBody = mainDB.fetchPeopleStay(cameraId);
            res.set_content(jsonBody, "application/json");
        }
        mtx.unlock();
	});

     server.Get("/peoplestay/unit", [this](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        // 클라이언트에게 기간에 따른 사람수(시작/끝) 정보 response
        if (req.has_param("camera_id") && req.has_param("start") && req.has_param("end")) {
            int cameraId = stoi(req.get_param_value("camera_id"));
            string start = req.get_param_value("start"), end = req.get_param_value("end");
            string jsonBody = mainDB.fetchPeopleStay(cameraId,stoi(start),stoi(end));
            res.set_content(jsonBody, "application/json");
        }
        mtx.unlock();
	});

    server.Get("/peoplemove/all", [this](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        // 클라이언트에게 사람수(전체) response
        if (req.has_param("camera_id")) {
            int cameraId = stoi(req.get_param_value("camera_id"));
            string jsonBody = mainDB.fetchPeopleStay(cameraId);
            res.set_content(jsonBody, "application/json");
        }
        mtx.unlock();
	});

    server.Get("/peoplemove/unit", [this](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        // 클라이언트에게 기간에 따른 사람수(시작/끝) 정보 response
        if (req.has_param("camera_id") && req.has_param("start") && req.has_param("end")) {
            int cameraId = stoi(req.get_param_value("camera_id"));
            string start = req.get_param_value("start"), end = req.get_param_value("end");
            string jsonBody = mainDB.fetchPeopleStay(cameraId,stoi(start),stoi(end));
            res.set_content(jsonBody, "application/json");
        }
        mtx.unlock();
	});

    server.Get("/area/all", [this](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        // 클라이언트에게 기간에 따른 사람수(시작/끝) 정보 response
        if ( req.has_param("camera_id") ) {
            int cameraId = stoi(req.get_param_value("camera_id"));
            json selected = mainDB.selectAllfromAwhereBequalC("areas","camera_id",cameraId);
            string jsonHandler;
            if (!selected.empty()) {
                json jsonArray = json::array();
                for (auto j:selected) {
                    json jsonData;
                    jsonData["area_id"] = j["area_id"];
                    jsonData["area_name"] = j["area_name"];
                    jsonData["color"] = j["color"];
                    jsonArray.push_back(jsonData);
                }
                json areaAll;   areaAll["areas"] = jsonArray;
                jsonHandler = areaAll.dump();
            }
            else {
                json errJson; errJson["err_msg"] = "no data"; jsonHandler = errJson.dump();
            }
            res.set_content(jsonHandler, "application/json");
        }
        mtx.unlock();
	});

    /* Post */

    server.Post("/area/insert", [this](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        json jsonData = json::parse(req.body);
        string areaName = jsonData["area_name"];
        int cameraId = jsonData["camera_id"];
        // 받은 HTTP의 body로 온 제이슨의 area_name이 DB에 있는지 확인
        json selected = mainDB.selectAllfromAwhereBequalC("areas","area_name",areaName);
        string jsonHandler;
        if (selected.empty()) {
            // 디비에 영역 저장하기
            mainDB.insertAreas(
                cameraId,
                jsonData["area_name"],
                jsonData["x"],
                jsonData["y"],
                jsonData["width"],
                jsonData["height"],
                jsonData["color"]
            );

            // cameras 테이블에서 camera_id로 camera_ip 정보 가져오기
            selected = mainDB.selectAllfromAwhereBequalC("cameras","camera_id", cameraId);
            string camIp = selected[0]["camera_ip"];

            // areas 테이블에서 area_name으로 해당하는 정보 SELECT해서 제이슨형식으로 가져오기
            selected = mainDB.selectAllfromAwhereBequalC("areas","area_name",areaName);
        
            // 해당 IP의 카메라로 영역정보를 담아 HTTP로 보내기
            httpClient.insertArea(camIp,selected[0]);

            json okJson; okJson["status"] = 200; jsonHandler = okJson.dump();
        }
        else {
            json errJson; errJson["status"] = -1; jsonHandler = errJson.dump();
        }
        res.set_content(jsonHandler, "application/json");
        mtx.unlock();
	});

    /* Delete */

    server.Delete("/area/all", [this](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        string jsonHandler;
        if (req.has_param("camera_id")) {
            int cameraId = stoi(req.get_param_value("camera_id"));
            // cameras 테이블에서 camera_id로 camera_ip 정보 가져오기
            json selected = mainDB.selectAllfromAwhereBequalC("cameras","camera_id", cameraId);
            string camIp = selected[0]["camera_ip"];
            // DB에서 해당 카메라의 영역 전체 삭제
            mainDB.deleteArea(cameraId);
            // 해당 IP의 카메라로 영역 전체 삭제 알리기
            httpClient.deleteArea(camIp);

            json okJson; okJson["status"] = 200; jsonHandler = okJson.dump();
        } else {
            json errJson; errJson["status"] = -1; jsonHandler = errJson.dump();
        }
        res.set_content(jsonHandler, "application/json");
        mtx.unlock();
	});

    server.Delete("/area", [this](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        string jsonHandler;
        if (req.has_param("camera_id") && req.has_param("area_id")) {
            int cameraId = stoi(req.get_param_value("camera_id"));
            int areaId = stoi(req.get_param_value("area_id"));
            // cameras 테이블에서 camera_id로 camera_ip 정보 가져오기
            json selected = mainDB.selectAllfromAwhereBequalC("cameras","camera_id", cameraId);
            string camIp = selected[0]["camera_ip"];

            // DB에서 해당 카메라의 영역 삭제
            mainDB.deleteArea(cameraId, areaId);
            // 해당 IP의 카메라로 해당 areaId의 영역 삭제 알리기
            httpClient.deleteArea(camIp, areaId);

            json okJson; okJson["status"] = 200; jsonHandler = okJson.dump();
        } else {
            json errJson; errJson["status"] = -1; jsonHandler = errJson.dump();
        }
        res.set_content(jsonHandler, "application/json");
        mtx.unlock();
	});

}

void HTTPServer::setCameraResponse()
{
    /* Post */
    server.Post("/peoplecnt", [this](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        string jsonHandler;
        json jsonData = json::parse(req.body);
        if (!jsonData.empty()) {
            for (auto j:jsonData["data"]) {
                mainDB.insertPeopleCount(j["area_id"],j["people_count"],j["start_time"],j["end_time"]);
            }
            json okJson; okJson["status"] = 200; jsonHandler = okJson.dump();
        } else {
            json errJson; errJson["status"] = -1; jsonHandler = errJson.dump();
        }
        res.set_content(jsonHandler, "application/json");
        mtx.unlock();
	});

    server.Post("/peoplemove", [this](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        string jsonHandler;
        json jsonData = json::parse(req.body);
        if (!jsonData.empty()) {
            for (auto j:jsonData["data"]) {
                mainDB.insertPeopleMove(j["from_area_id"],j["to_area_id"],j["count"],j["start_time"],j["end_time"]);
            }
            json okJson; okJson["status"] = 200; jsonHandler = okJson.dump();
        } else {
            json errJson; errJson["status"] = -1; jsonHandler = errJson.dump();
        }
        res.set_content(jsonHandler, "application/json");
        mtx.unlock();
	});

    server.Post("/peoplestay", [this](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        string jsonHandler;
        json jsonData = json::parse(req.body);
        if (!jsonData.empty()) {
            for (auto j:jsonData["data"]) {
                mainDB.insertPeopleStay(j["area_id"],j["stay_time"],j["start_time"],j["end_time"]);
            }
            json okJson; okJson["status"] = 200; jsonHandler = okJson.dump();
        } else {
            json errJson; errJson["status"] = -1; jsonHandler = errJson.dump();
        }
        res.set_content(jsonHandler, "application/json");
        mtx.unlock();
	});

}


void HTTPServer::setHTTPServer()
{
    setClientResponse();
    setCameraResponse();
    cout << " - start smart_zone_server - " << endl;
    server.listen(this->ip, this->port);
}



