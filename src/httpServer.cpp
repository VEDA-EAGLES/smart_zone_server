
#include "httpServer.h"

HTTPServer::HTTPServer(string ip, int port) 
{
    this->ip = ip;
    this->port = port;
}

HTTPServer::~HTTPServer() {}

void HTTPServer::setResponse()
{
    MainDB mainDB;
    HTTPClient httpClient;

    server.Get("/device/all", [&mainDB](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        // 클라이언트에게 카메라 정보 response
        string jsonBody = mainDB.fetchCameras();
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});

    server.Get("/peoplecnt/all", [&mainDB](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        // 클라이언트에게 사람수(전체) response
        string jsonBody = mainDB.fetchData();
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});

    server.Get("/peoplecnt/unit", [&mainDB](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        // 클라이언트에게 기간에 따른 사람수(시작/끝) 정보 response
        string start = req.get_param_value("start"), end = req.get_param_value("end");
        string jsonBody = mainDB.fetchData(stoi(start),stoi(end));
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});

    server.Post("/area/insert", [&mainDB, &httpClient](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        json jsonData = json::parse(req.body);
        string areaName = jsonData["area_name"];
        int cameraId = jsonData["camera_id"];
        // 받은 HTTP의 body로 온 제이슨의 area_name이 DB에 있는지 확인
        json selected = mainDB.selectAllfromAwhereBequalC("areas","area_name",areaName);
        string jsonHandler;
        if (selected.empty()) {
            json okJson; okJson["status"] = 200; jsonHandler = okJson.dump();
            // 디비에 영역 저장하기
            mainDB.insertAreas(
                cameraId,
                jsonData["area_name"],
                jsonData["x"],
                jsonData["y"],
                jsonData["width"],
                jsonData["height"]
            );

            // cameras 테이블에서 camera_id로 camera_ip 정보 가져오기
            selected = mainDB.selectAllfromAwhereBequalC("cameras","camera_id", cameraId);
            string camIp = selected["camera_ip"];

            // areas 테이블에서 area_name으로 해당하는 정보 SELECT해서 제이슨형식으로 가져오기
            selected = mainDB.selectAllfromAwhereBequalC("areas","area_name",areaName);
        
            // 해당 IP의 카메라로 영역정보를 담아 HTTP로 보내기
            httpClient.insertArea(camIp,selected);
        }
        else {
            json errJson; errJson["err_msg"] = "Duplicate Name"; jsonHandler = errJson.dump();
        }
        res.set_content(jsonHandler, "application/json");
        mtx.unlock();
	});

    server.Delete("/area/delete", [&mainDB, &httpClient](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        
        if (req.has_param("camera_id") && req.has_param("area_id")) {
            int cameraId = stoi(req.get_param_value("camera_id"));
            int areaId = stoi(req.get_param_value("area_id"));
            // cameras 테이블에서 camera_id로 camera_ip 정보 가져오기
            json selected = mainDB.selectAllfromAwhereBequalC("cameras","camera_id", cameraId);
            string camIp = selected["camera_ip"];

            // DB에서 해당 카메라의 영역 삭제
            mainDB.deleteArea(cameraId, areaId);
            // 해당 IP의 카메라로 해당 areaId의 영역 삭제 알리기
            httpClient.deleteArea(camIp, areaId);
        } else {
            cout << " area delete error " << endl;
        }
        mtx.unlock();
	});
    
    server.Delete("/area/all", [&mainDB, &httpClient](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        if (req.has_param("camera_id")) {
            int cameraId = stoi(req.get_param_value("camera_id"));
            // cameras 테이블에서 camera_id로 camera_ip 정보 가져오기
            json selected = mainDB.selectAllfromAwhereBequalC("cameras","camera_id", cameraId);
            string camIp = selected["camera_ip"];
            // DB에서 해당 카메라의 영역 전체 삭제
            mainDB.deleteArea(cameraId);
            // 해당 IP의 카메라로 영역 전체 삭제 알리기
            httpClient.deleteArea(camIp);
        } else {
            cout << " area all delete error" << endl;
        }
        mtx.unlock();
	});

    server.Post("/video", [&mainDB](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        json jsonData = json::parse(req.body);
        cout << req.body << endl;
        // mainDB.insertVideo(
        //     jsonData["camera_id"],
        //     jsonData["video_name"],
        //     jsonData["video_storage"],
        //     jsonData["start_time"],
        //     jsonData["end_time"]
        // );
        // httpClient.insertArea();
        mtx.unlock();
	});
}

void HTTPServer::setHTTPServer()
{
    setResponse();
    cout << " - start smart_zone_server - " << endl;
    server.listen(this->ip, this->port);
}



