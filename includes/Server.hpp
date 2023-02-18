#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <iostream>
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

#include "User.hpp"
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
		CommandInvoker _invoker;	//unknown type name - include를 했는데 왜 못 쓰지?

        std::map<int, User> _users;
        std::map<std::string, Channel> _channels;

        Server(){};
		
		// handle socket EOF (EOF means disconnect)
		void handleEof(struct kevent &k);

		// event ERROR, print kevent status
		void handleError(struct kevent &k);

		//set the new client socket to nonblock and add it to changelist
		void acceptUser(std::vector<struct kevent> &changelist);

		// recv
		void handleRead(struct kevent &k, std::vector<struct kevent> &changelist);

		// send
		void handleWrite(struct kevent &k);

    public:

        static Server& getInstance();

        void servSetup(char *port);
        void cmdsSetup();
        void run();
		std::map<int, User>&	getUserMap();
};

#endif
