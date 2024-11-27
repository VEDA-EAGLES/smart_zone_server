
#include "httplib.h"
#include "json.hpp"
#include "define.h"
#include "mainDB.h"
#include <string>

using namespace nlohmann;
using namespace std;

extern mutex mtx;

class HTTPClient
{
private:

public:
    HTTPClient();
    ~HTTPClient();
    void insertArea(string camIp, json jsonData);
    void deleteArea(string camIp);
    void deleteArea(string camIp, int areaId);
};


