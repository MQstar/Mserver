#include <iostream>
#include <server.hh>
using namespace server;
int main()
{
    Server* s = new Server();
    delete s;
    printf("start");
}
