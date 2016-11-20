#include <signal.h>

#include <iostream>

#include "server.h"
#include "common.h"

using namespace bon_server;

void signal_handler(int s){
    printf("Caught signal %d\n",s);
    Server::Instance().Stop();
}

int main( int argc, const char * argv[] )
{

    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = signal_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    Server::Instance().Start(kServiceName, kServiceType, kServiceDomain, kServicePort);

    std::cout << "EXIT" <<std::endl;


    return 0;
}
