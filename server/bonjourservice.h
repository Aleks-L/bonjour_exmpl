#ifndef BONJOURSERVICE_H
#define BONJOURSERVICE_H

#include <stdint.h>
#include <dns_sd.h>

namespace bon_server {

class BonjourService
{
public:
    BonjourService();
    ~BonjourService();

    void StopService();
    bool StartService(const char *serviceName, const char *serviceType, const char *serviceDomain,
                      const uint16_t port, DNSServiceRegisterReply callBack, void *context);

    DNSServiceErrorType HandleEvent();

    inline int bonjour_fd() const {return bonjour_fd_;}

private:
    DNSServiceRef sd_ref_;
    int bonjour_fd_ = -1;
};

} //NS

#endif // BONJOURSERVICE_H
