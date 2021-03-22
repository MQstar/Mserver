#include <iostream>
#include <memory>
#include <Server.hh>
#include <ConfigMrg.hh>
using namespace server;
using namespace config;
int main()
{
    auto cm = std::make_shared<ConfigMrg>();
    auto server = std::make_shared<Server>();
    server->start();
}
