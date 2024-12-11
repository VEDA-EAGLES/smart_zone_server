#include "httpClient.h"

HTTPClient::HTTPClient() {}
HTTPClient::~HTTPClient() {}

void HTTPClient::insertArea(string camIp, json jsonData)
{  
    httplib::Client cli(camIp,HTTP_C_PORT);

    json data;  data["area"] = json::array({jsonData}); 
    string jsonBody = data.dump(); // JSON 문자열로 변환

    auto res = cli.Post("/area/insert", jsonBody, "application/json"); 
    if (!res) {  
        std::cout << "Error: " << res.error() << std::endl;
    } 
}


void HTTPClient::deleteArea(string camIp)
{  
    httplib::Client cli(camIp,HTTP_C_PORT);
    
    auto res = cli.Delete("/area/all");
    if (!res) {  
        std::cout << "Error: " << res.error() << std::endl;
    } 
}

void HTTPClient::deleteArea(string camIp, int areaId)
{
    httplib::Client cli(camIp,HTTP_C_PORT);
    
    string req = "/area?area_id=" + to_string(areaId);

    auto res = cli.Delete(req.c_str()); 
    if (!res) {  
        std::cout << "Error: " << res.error() << std::endl;
    } 
}