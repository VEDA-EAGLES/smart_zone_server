
#include "httpServer.h"
#include "mainDB.h"

HTTPServer::HTTPServer(string ip, int port) 
{
    this->ip = ip;
    this->port = port;
}

HTTPServer::~HTTPServer() {}

void HTTPServer::setResponse()
{
    cout << "test 0";
    MainDB mainDB;
    
    server.Get("/device/all", [&mainDB](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        string jsonBody = mainDB.fetchCameras();
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});

    server.Get("/peoplecnt/all", [&mainDB](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        string jsonBody = mainDB.fetchData();
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});

    server.Get("/peoplecnt/unit", [&mainDB](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        string start = req.get_param_value("start"), end = req.get_param_value("end");
        string jsonBody = mainDB.fetchData(stoi(start),stoi(end));
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});

    server.Post("/area/insert", [&mainDB](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        cout << "test 1";
        json jsonData = json::parse(req.body);
        cout << "test 2";
        string areaName = jsonData["area_name"];
        cout << "test 3";
        string jsonBody;
        
        cout << mainDB.selectAllfromAwhereBequalC("areas","area_name",areaName);
        cout << "test 4";
        // if ( mainDB.checkAreasDup(areaName) ) {
        //     // 영역이름이 중복
        //     json errJson; errJson["err_msg"] = "Duplicate Name"; jsonBody = errJson.dump();
        // }
        // else {
        //     // 영역등록 성공
        //     json okJson; okJson["status"] = 200; jsonBody = okJson.dump();
        //     mainDB.insertAreas(
        //         jsonData["camera_id"],
        //         jsonData["area_name"],
        //         jsonData["x"],
        //         jsonData["y"],
        //         jsonData["width"],
        //         jsonData["height"]
        //     );
        // }
        // res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});
    
    server.Delete("/area/all", [&mainDB](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        json jsonData = json::parse(req.body);
        int cameraId = jsonData["camera_id"];
        // deleteArea(cameraId);
        mtx.unlock();
	});

    server.Get("/video", [&mainDB](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        json jsonData = json::parse(req.body);
        mainDB.insertVideo(
            jsonData["camera_id"],
            jsonData["video_name"],
            jsonData["video_storage"],
            jsonData["start_time"],
            jsonData["end_time"]
        );
        mtx.unlock();
	});

}

void HTTPServer::setHTTPServer()
{
    cout << " - start smart_zone_server - \n";
    setResponse();
    server.listen(this->ip, this->port);
}



