#include "bonjourservice.h"

#include <dns_sd.h>
#include <arpa/inet.h>
#include <iostream>

#include <unistd.h>

namespace bon_server {

BonjourService::BonjourService()
{

}


void service_callback
(
    DNSServiceRef sdRef,
    DNSServiceFlags flags,
    DNSServiceErrorType errorCode,
    const char                          *name,
    const char                          *regtype,
    const char                          *domain,
    void                                *context
)
{
    std::cout<<"CALLBACK"<<std::endl;
}



void BonjourService::StartService()
{
    DNSServiceRef sd_ref_;
    const char * service_name_ = "jjj";
    const char * service_type_ = "_tryst._tcp";
    const char * service_domain_ = "local";
    const uint16_t port_ = 4559;

    DNSServiceErrorType err = DNSServiceRegister(&sd_ref_,
                                                 kDNSServiceFlagsNoAutoRename,
                                                 0,
                                                 service_name_,
                                                 service_type_,
                                                 service_domain_,
                                                 nullptr,
                                                 htons(port_),
                                                 0,
                                                 nullptr,
                                                 service_callback,
                                                 nullptr);

    //DNSServiceRegisterReply

    if (err == kDNSServiceErr_NoError)
        printf("OK");

    int sock = DNSServiceRefSockFD(sd_ref_);

    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(sock, &readset);



   // printf ()


    while(1)
    {
        //int res = select(sock, &readset, NULL, NULL, NULL);
        std::cout<<"while"<<std::endl;
        sleep(1);
    }
}

} //NS
