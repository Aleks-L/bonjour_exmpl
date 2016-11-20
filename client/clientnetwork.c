#include "clientnetwork.h"

#include <dns_sd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static void resolve_callback
(
        DNSServiceRef           sdRef,
        DNSServiceFlags         flags,
        uint32_t                interfaceIndex,
        DNSServiceErrorType     errorCode,
        const char              *fullname,
        const char              *hosttarget,
        uint16_t                port,                                   /* In network byte order */
        uint16_t                txtLen,
        const unsigned char     *txtRecord,
        void                    *context
        )
{
    if (errorCode != kDNSServiceErr_NoError)
    {
        printf("Resolve callback return:%d", errorCode);
        return;
    }
    else
    {
        ServiceAddr *serv_addr = (ServiceAddr*)context;
        strcpy(serv_addr->host, hosttarget);
        serv_addr->port = ntohs(port);
    }
}

int find_service(const char *serviceName,
                 const char *serviceType,
                 const char *serviceDomain,
                 ServiceAddr *serv_addr)
{
    DNSServiceRef sd_ref;

    DNSServiceErrorType err = DNSServiceResolve(&sd_ref,
                                                0,
                                                0,
                                                serviceName,
                                                serviceType,
                                                serviceDomain,
                                                resolve_callback,
                                                serv_addr);


    if (err != kDNSServiceErr_NoError)
    {
        printf("DNSServiceResolve() return:%d", err);
        return err;
    }

    DNSServiceProcessResult(sd_ref);
    DNSServiceRefDeallocate(sd_ref);

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

int connect_to_server(ServiceAddr *serv_add)
{
    int sock;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("ERROR opening socket\n");
        return -1;
    }

    server = gethostbyname(serv_add->host);
    if (server == NULL) {
        perror("ERROR, no such host\n");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    //inet_aton(ip, &serv_addr.sin_addr);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_addr.sin_port = htons(serv_add->port);

    //Connect
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting\n");
        return -1;
    }

    return sock;
}
