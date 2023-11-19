#include "web_server_user/http_web_server.h"


int main(int argc, char*argv[]) 
{
    HTTPWebServer app;
    return app.run(argc, argv);
}