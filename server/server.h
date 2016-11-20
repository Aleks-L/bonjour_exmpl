#ifndef SERVER_H
#define SERVER_H

#include "common.h"
#include "filehandler.h"
#include "bonjourservice.h"

#include <sys/epoll.h>

#include <map>
#include <memory>

namespace bon_server {

class Server
{
public:
    static Server &Instance();
    void Start(const char *serviceName, const char *serviceType, const char *serviceDomain, const uint16_t port);
    void Stop();

private:
    BonjourService bon_serv_;
    int listen_sock_ = -1;
    int epoll_fd_ = -1;
    std::map<int, std::shared_ptr<FileHandler>> handlers_;

    volatile bool is_run_ = true;

    Server() = default;
    Server(const Server&) = delete;
    Server(const Server&&) = delete;
    ~Server();

    Server& operator=( Server& ) = delete;
    Server& operator=( Server&& ) = delete;

    bool CreateListener(uint16_t port);
    bool SetNonBlocking(int sock);
    void AddNewClient();
    void HandleClient(const epoll_event &event);
    static void Bonjour_Callback(DNSServiceRef sdRef, DNSServiceFlags flags, DNSServiceErrorType errorCode,
                                 const char *name, const char *regtype, const char *domain, void *context);
};

} //NS

#endif // SERVER_H
