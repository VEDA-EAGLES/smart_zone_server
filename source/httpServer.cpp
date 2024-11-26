
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

    server.Post("/area/insert", [&mainDB, &httpClient](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        json jsonData = json::parse(req.body);
        string areaName = jsonData["area_name"];
        int cameraId = jsonData["camera_id"];
        json selected = mainDB.selectAllfromAwhereBequalC("areas","area_name",areaName);
        string jsonHandler;
        if (selected.empty()) {
            json okJson; okJson["status"] = 200; jsonHandler = okJson.dump();
            mainDB.insertAreas(
                cameraId,
                jsonData["area_name"],
                jsonData["x"],
                jsonData["y"],
                jsonData["width"],
                jsonData["height"]
            );
            selected = mainDB.selectAllfromAwhereBequalC("cameras","camera_id", cameraId);
            string camIp = selected["camera_ip"];
            selected = mainDB.selectAllfromAwhereBequalC("areas","area_name",areaName);
            httpClient.insertArea(camIp,selected);
        }
        else {
            json errJson; errJson["err_msg"] = "Duplicate Name"; jsonHandler = errJson.dump();
        }
        res.set_content(jsonHandler, "application/json");
        mtx.unlock();
	});
    
    server.Delete("/area/all", [&mainDB, &httpClient](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        json jsonData = json::parse(req.body);
        int cameraId = jsonData["camera_id"];

        json selected = mainDB.selectAllfromAwhereBequalC("cameras","camera_id", cameraId);
        string camIp = selected["camera_ip"];

        selected = mainDB.selectAllfromAwhereBequalC("areas","camera_id", cameraId);
        string areaId = selected["area_id"];

        mainDB.deleteArea(cameraId);
        httpClient.deleteArea(camIp,areaId);
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
    cout << " - start smart_zone_server - " << endl;
    setResponse();
    server.listen(this->ip, this->port);
}



