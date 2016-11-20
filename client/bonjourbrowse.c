#include "bonjourbrowse.h"

#include <dns_sd.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <stdio.h>



void browse_callback
(
        DNSServiceRef sdRef,
        DNSServiceFlags flags,
        uint32_t interfaceIndex,
        DNSServiceErrorType errorCode,
        const char                          *serviceName,
        const char                          *regtype,
        const char                          *replyDomain,
        void                                *context
        )
{
    printf("browse CALLBACK\n");
}



void StartBrowse()
{
    DNSServiceRef sd_ref_;
    //const char * service_name_ = "jjj";
    const char * service_type_ = "_tryst._tcp";
    const char * service_domain_ = "local";

    DNSServiceErrorType err = DNSServiceBrowse(&sd_ref_,
                                               0,
                                               0,
                                               service_type_,
                                               service_domain_,
                                               browse_callback,
                                               NULL);

    //DNSServiceRegisterReply
    //DNSServicerowseReply

    if (err == kDNSServiceErr_NoError)
        printf("OK\n");

    //    int sock = DNSServiceRefSockFD(sd_ref_);

    //    fd_set readset;
    //    FD_ZERO(&readset);
    //    FD_SET(sock, &readset);



    // printf ()


    while(1)
    {
        //int res = select(sock, &readset, NULL, NULL, NULL);
        printf("while\n");
        sleep(1);
    }
}

