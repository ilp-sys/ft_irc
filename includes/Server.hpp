#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <iostream>
#include <sys/event.h>
#include <arpa/inet.h>
#include <sys/event.h>
#include <fcntl.h>
#include <err.h>
#include <sys/fcntl.h>
#include <vector>

#include "User.hpp"

#define BACKLOG 5
#define MAX_EVETNS 10

class Server
{
        int _kq;
        int _servSock;
        struct sockaddr_in _servAddress;
        static Server _server;
        std::vector<User*> _users;
        std::vector<struct kevent> _events;

        Server(){};
    public:
        static Server& getInstance();

        void servSetup(char *ip);
        void run();
        void acceptNewClient();
};

#endif
