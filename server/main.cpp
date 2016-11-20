//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#include <string>
#include <iostream>

#include "bonjourservice.h"
//#include "filehandler.h"
#include "server.h"
#include "serv_common.h"

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

    // Bonjour init

//    bon_server::BonjourService::StartService();


    Server::Instance().Start(kServicePort);

    std::cout << "EXIT" <<std::endl;


    return 0;
}
