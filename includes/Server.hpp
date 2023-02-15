#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <iostream>
#include <sys/event.h>
#include <arpa/inet.h>
#include <sys/event.h>
#include <fcntl.h>
#include <err.h>
#include <sys/fcntl.h>
#include <map>
#include <vector>

#include "User.hpp"
#include "Channel.hpp"

#define BACKLOG 5
#define MAX_EVENTS 10
#define BUFFER_SIZE 512
 
class Server
{
        static Server _server;

        int _kq;
        int _servSock;

        std::map<int, User> _users;
        std::map<std::string, Channel> _channels;

        Server(){};
    public:
        static Server& getInstance();

        void servSetup(char *port);
        void run();
};

#endif
