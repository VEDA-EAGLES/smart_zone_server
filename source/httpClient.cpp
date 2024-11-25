#include "httpClient.h"


HTTPClient::HTTPClient() {}

HTTPClient::~HTTPClient() {}

void HTTPClient::deleteArea(int camI)
{  
    mtx.lock();
    httplib::Client cli(CAM1_IP,HTTP_C_PORT);
    // auto res = cli.Post("/video", jsonBody, "application/json");
    // if (res) {  
    // } else {
    //     std::cout << "Error: " << res.error() << std::endl;
    // }
    mtx.unlock();
}

