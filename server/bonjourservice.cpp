#include "bonjourservice.h"

#include <arpa/inet.h>
#include <iostream>

#include <unistd.h>

namespace bon_server {

BonjourService::BonjourService()
{

}

BonjourService::~BonjourService()
{
    StopService();
}

DNSServiceErrorType BonjourService::HandleEvent()
{
    return DNSServiceProcessResult(sd_ref_);
}

bool BonjourService::StartService(const char *serviceName,
                                  const char *serviceType,
                                  const char *serviceDomain,
                                  const uint16_t port,
                                  DNSServiceRegisterReply callBack,
                                  void *context)
{
    DNSServiceErrorType error = DNSServiceRegister(&sd_ref_,
                                                 kDNSServiceFlagsNoAutoRename,
                                                 0,
                                                 serviceName,
                                                 serviceType,
                                                 serviceDomain,
                                                 nullptr,
                                                 htons(port),
                                                 0,
                                                 nullptr,
                                                 callBack,
                                                 context);

    if (error != kDNSServiceErr_NoError)
    {
        std::cerr << "DNSServiceRegister() return:" << error << std::endl;
        return false;
    }


    bonjour_fd_ = DNSServiceRefSockFD(sd_ref_);

    return true;
}

void BonjourService::StopService()
{
    DNSServiceRefDeallocate(sd_ref_);
}


} //NS
