#ifndef CLIENTNETWORK_H
#define CLIENTNETWORK_H

#include <stdint.h>

typedef struct
{
    char host[256];
    uint16_t port;
}ServiceAddr;

int find_service(const char *serviceName, const char *serviceType, const char *serviceDomain, ServiceAddr *serv_addr);
int connect_to_server(ServiceAddr *serv_add);
int send_new_record(int sock, int max_val);

#endif // BONJOURSERVICE_H
