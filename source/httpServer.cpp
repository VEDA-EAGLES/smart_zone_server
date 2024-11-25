
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
    server.Get("/device/all", [](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        string jsonBody = fetchCameras();
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});

    server.Get("/peoplecnt/all", [](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        string jsonBody = fetchData();
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});

    server.Get("/peoplecnt/unit", [](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        string start = req.get_param_value("start"), end = req.get_param_value("end");
        string jsonBody = fetchData(stoi(start),stoi(end));
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});

    server.Post("/area/insert", [](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        json jsonData = json::parse(req.body);
        string areaName = jsonData["area_name"];
        string jsonBody;

        if ( checkAreasDup(areaName) ) {
            // 영역이름이 중복
            json errJson; errJson["err_msg"] = "Duplicate Name"; jsonBody = errJson.dump();
        }
        else {
            // 영역등록 성공
            json okJson; okJson["status"] = 200; jsonBody = okJson.dump();
            insertAreas(
                jsonData["camera_id"],
                jsonData["area_name"],
                jsonData["x"],
                jsonData["y"],
                jsonData["width"],
                jsonData["height"]
            );
        }
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});
    
    server.Delete("/area/insert", [](const httplib::Request& req, httplib::Response& res) {
        // mtx.lock();
        // json jsonData; jsonData["status"] = 200; string jsonBody = jsonData.dump();
        // res.set_content(jsonBody, "application/json");
        // mtx.unlock();
	});

}

void HTTPServer::setHTTPServer()
{
    cout << " - start smart_zone_server - \n";
    setResponse();
    server.listen(this->ip, this->port);
}



