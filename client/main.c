#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#include "common.h"
#include "clientnetwork.h"

volatile int g_run = 1;


void signal_handler(int s);

int main( int argc, const char * argv[] )
{

    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = signal_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    int max_val = 0;
    int loops = -1;

    if (argc < 2)
    {
        perror("Wrong argument\n");
        return -1;
    }

    max_val = atoi(argv[1]);
    if (argc >= 3)
        loops = atoi(argv[2]);

    // Find Bonjour service
    ServiceAddr serv_addr = {"", 0};
    find_service(kServiceName, kServiceType, kServiceDomain, &serv_addr);
    if (serv_addr.port == 0)
    {
        perror("Bonjour service not registered\n");
        return -1;
    }


    // Connect to server
    int serv_sock = connect_to_server(&serv_addr);
    if (serv_sock == -1)
        return -1;

    srand(time(NULL));

    // Sending
    while(loops && g_run)
    {
        if (send_new_record(serv_sock, max_val) <= 0)
        {
            perror("ERROR sending\n");
            return -1;
        }

        loops = (loops > 0) ? loops-1 : loops;
        sleep(1);
    }

    close(serv_sock);
    printf("EXIT\n");

    return 0;
}


void signal_handler(int s)
{
    printf("Caught signal %d\n",s);
    g_run = 0;
    //exit(1);
}
