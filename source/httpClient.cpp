#include "httpClient.h"

HTTPClient::HTTPClient() {}
HTTPClient::~HTTPClient() {}

void HTTPClient::insertArea(string camIp, json jsonData)
{  
    httplib::Client cli(camIp,HTTP_C_PORT);

    json data;
    data["area"] = json::array({jsonData}); 

    string jsonBody = data.dump(); // JSON 문자열로 변환

    auto res = cli.Post("/area/insert", jsonBody, "application/json"); 
    if (!res) {  
        std::cout << "Error: " << res.error() << std::endl;
    } 

}


void HTTPClient::deleteArea(string camIp,string areaId)
{  
    httplib::Client cli(camIp,HTTP_C_PORT);
    json jsonData; jsonData["area_id"] = areaId;
    string jsonBody = jsonData.dump();
    auto res = cli.Post("/video", jsonBody, "application/json");
    if (!res) {  
        std::cout << "Error: " << res.error() << std::endl;
    } 
}

