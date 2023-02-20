#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <iostream>
#include <string>
#include <sys/event.h>
#include <arpa/inet.h>
#include <sys/event.h>
#include <fcntl.h>
#include <err.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <map>
#include <vector>

#include "Client.hpp"
#include "Channel.hpp"
#include "CommandInvoker.hpp"
#include "Define.hpp"

#define BACKLOG 5
#define MAX_EVENTS 10
#define BUFFER_SIZE 512
 
class Server
{
        static Server _server;

        int _kq;
        int _servSock;
        CommandInvoker _invoker;

        std::string _password;
        std::map<int, Client> _clients;
        std::map<std::string, Channel> _channels;
		std::vector<struct kevent> _changelist;

        Server(){};
    
    // handle socket EOF (EOF means disconnect)
    void handleEof(struct kevent &k);

    // event ERROR, print kevent status
    void handleError(struct kevent &k);

    //set the new client socket to nonblock and add it to changelist
    void acceptUser();

    // recv
    void handleRead(struct kevent &k);

    // send
    void handleWrite(struct kevent &k);

    // test function
    void testServer(struct kevent &currEvent);


    public:

        static Server& getInstance();

        void servSetup(char *port);
        void cmdsSetup();
        void run();

        std::map<int, Client>&  getClients();
        std::map<std::string, Channel>&  getChannels();
		std::vector<struct kevent>& getChangeList();

        void setPswd(std::string);
        std::string getPswd() const;

};

#endif
