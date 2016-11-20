#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#include "bonjourbrowse.h"

#define SERVER_ADDR     "127.0.0.1"

volatile int g_run = 1;


void signal_handler(int s);
int connect_to_server(const char* ip, u_int16_t port);
int send_new_record(int sock, int max_val);

int main( int argc, const char * argv[] )
{

    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = signal_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    // Bonjour init

    //StartBrowse();

    int max_val = 0;
    int loops = -1;

    if (argc < 2)
        printf("Wrong argument\n");

    max_val = atoi(argv[1]);
    if (argc >= 3)
        loops = atoi(argv[2]);

    int serv_sock = connect_to_server("127.0.0.1", 4559);
    if (serv_sock == -1)
        return -1;

    srand(time(NULL));

    while(loops && g_run)
    {
        if (send_new_record(serv_sock, max_val) <= 0)
        {
            perror("ERROR sending");
            return -1;
        }

        loops = (loops > 0) ? loops-1 : loops;
        sleep(1);
    }

    close(serv_sock);
    printf("EXIT\n");

    return 0;
}

int send_new_record(int sock, int max_val)
{
    // Get random val and convert to network byte order for arch independence
    typedef union
    {
        int32_t i;
        float   f;
    }Converter;

    Converter el;
    int32_t buf[3];

    el.i = rand() % (max_val+1);
    buf[0] = htonl(el.i);

    el.f = ((float)rand()/(float)(RAND_MAX)) * max_val;
    buf[1] = htonl(el.i);

    el.i = rand() % (max_val+1);
    buf[2] = htonl(el.i);

    // Send
    return write(sock, &buf, sizeof(buf));
}

int connect_to_server(const char* ip, u_int16_t port)
{
    int sock;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("ERROR opening socket");
        return -1;;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    inet_aton(ip, &serv_addr.sin_addr);
    serv_addr.sin_port = htons(port);

    //Connect
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        return -1;
    }

    return sock;
}

void signal_handler(int s)
{
    printf("Caught signal %d\n",s);
    g_run = 0;
    //exit(1);
}
