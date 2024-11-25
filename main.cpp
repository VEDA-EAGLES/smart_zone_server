#include <iostream>
#include <thread>
#include "define.h"
#include "httpServer.h"

using namespace std;

mutex mtx;

void startHTTPServer(int thread_id) 
{
    HTTPServer httpServer(MY_IP, HTTP_S_PORT);
    httpServer.setHTTPServer();
}

int main(int argc, char** argv)
{
    thread httpServerThread = thread(startHTTPServer, 1);
    httpServerThread.join();

    return 0;
}

