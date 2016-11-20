#include "server.h"

#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>


namespace bon_server {

Server& Server::Instance()
{
    static Server instance;

    return instance;
}


Server::~Server()
{
    is_run_ = false;
}


bool Server::SetNonBlocking(int sock)
{
    int flags;

    flags = fcntl (sock, F_GETFL, 0);
    if (flags == -1)
    {
        std::cerr << "ERROR on getting flags" << std::endl;
        return false;
    }

    flags |= O_NONBLOCK;
    if (fcntl (sock, F_SETFL, flags) == -1)
    {
        std::cerr << "ERROR on setting flags" << std::endl;
        return false;
    }

    return true;
}

bool Server::CreateListener(uint16_t port)
{
    struct sockaddr_in addr;

    listen_sock_ = socket(AF_INET, SOCK_STREAM, 0);

    if (listen_sock_ < 0) {
        std::cerr << "ERROR opening socket" << std::endl;
        return false;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    // Bind socket
    if (bind(listen_sock_, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        std::cerr << "ERROR on binding" << std::endl;
        return false;
    }

    if (!SetNonBlocking(listen_sock_))
        return false;

    // Listen
    if (listen(listen_sock_, kMaxConnections) == -1)
    {
        std::cerr << "ERROR on listen" << std::endl;
        return false;
    }

    return true;
}

void Server::AddNewClient()
{
    struct sockaddr_in client_addr;
    socklen_t socklen = sizeof(struct sockaddr_in);
    int client_sock = accept(listen_sock_, (struct sockaddr *) &client_addr, &socklen);

    SetNonBlocking(client_sock);

    // add new client to epoll
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR | EPOLLET;
    event.data.fd = client_sock;

    if (epoll_ctl (epoll_fd_, EPOLL_CTL_ADD, client_sock, &event) == -1)
    {
        std::cerr << "ERROR on epoll_ctl. add client" << std::endl;
        return;
    }

    // Create handler for client
    handlers_[client_sock] = std::shared_ptr<FileHandler>(new FileHandler(client_sock));
}

void Server::HandleClient(const struct epoll_event &event)
{
    if ((event.events & EPOLLERR) ||
            (event.events & EPOLLHUP) ||
            (event.events & EPOLLRDHUP))
    {
        handlers_[event.data.fd]->Finalize();
        handlers_.erase(event.data.fd);
    }
    else
    {
        int32_t buf[3];

        // Read and convert to host byte order
        if (read(event.data.fd, buf, sizeof(buf)) > 0)
        {
            FileHandler::Record rec;
            rec.e1.i = ntohl(buf[0]);
            rec.e2.i = ntohl(buf[1]);
            rec.e3.i = ntohl(buf[2]);

            handlers_[event.data.fd]->AddNewInfo(rec);
        }
    }
}

void Server::Start(uint16_t port)
{
    if (!CreateListener(port))
        return;

    if ( (epoll_fd_ = epoll_create(kMaxConnections + 1)) == -1)
    {
        std::cerr << "ERROR on epoll_create" << std::endl;
        return;
    }

    struct epoll_event event;
    struct epoll_event events[kMaxConnections];

    //event.events = EPOLLIN | EPOLLHUP | EPOLLET;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = listen_sock_;

    if (epoll_ctl (epoll_fd_, EPOLL_CTL_ADD, listen_sock_, &event) == -1)
    {
        std::cerr << "ERROR on epoll_ctl. add listener" << std::endl;
        return;
    }


    while(is_run_)
    {
        int event_cnt = epoll_wait(epoll_fd_, events, kMaxConnections, 2000);

        for(int i = 0; i < event_cnt; ++i)
        {
            // New client
            if(events[i].data.fd == listen_sock_)
            {
                AddNewClient();
            }
            else
            {
                // Handle message from client
                HandleClient(events[i]);
            }
        }
    }

    close(listen_sock_);
    close(epoll_fd_);
}

void Server::Stop()
{
    std::cout << "STOP" << std::endl;
    is_run_ = false;
}

} //NS
