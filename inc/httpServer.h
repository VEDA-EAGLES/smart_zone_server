
#include "httplib.h"
#include "json.hpp"
#include "define.h"
#include "mainDB.h"
#include "httpClient.h"
#include <string>

using namespace nlohmann;
using namespace std;

extern mutex mtx;

class HTTPServer
{
private:
    httplib::Server server;
    MainDB mainDB;
    HTTPClient httpClient;
    string ip;
    int port;
    void setClientResponse();
    void setCameraResponse();

public:
    HTTPServer(string ip, int port);
    ~HTTPServer();
    void setHTTPServer();
};


