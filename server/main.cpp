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





//    FileHandler fh{1};

//    fh.AddNewInfo({44, 2.2, 66});
//    fh.AddNewInfo({22, 5.2, 65});
//    fh.AddNewInfo({58, 6.6, 45});
//    fh.AddNewInfo({67, 1.2, 14});
//    fh.AddNewInfo({12, 0.2, 99});
//    fh.AddNewInfo({58, 6.6, 45});

//    fh.Finalize();

//    FileHandler::RecordElem el{0};
//    el.f = 5.6;

//    std::cout << "F:" << el.f <<std::endl;
//    std::cout << "I:" << el.i <<std::endl<<std::endl;

//    el.i = htonl(el.i);

//    std::cout << "F:" << el.f <<std::endl;
//    std::cout << "I:" << el.i <<std::endl<<std::endl;

//    el.i = ntohl(el.i);

//    std::cout << "F:" << el.f <<std::endl;
//    std::cout << "I:" << el.i <<std::endl;


    return 0;
}
