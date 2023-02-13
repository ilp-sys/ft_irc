#include "../includes/Server.hpp"
#include <cstring>
#include <sys/event.h>
#include <sys/socket.h>

Server& Server::getInstance()
{
    return (_server);
}

void Server::servSetup(char *port)
{
    //get kqueue instance
    if ((_kq = kqueue()) == -1)
        err(EXIT_FAILURE, "failed to create kqueue");

    //set server socket nonblock active
    if ((_servSock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        err(EXIT_FAILURE, "failed to create server socket");


    if ((fcntl(_servSock, F_SETFL, fcntl(_servSock, F_GETFL) | O_NONBLOCK)) == -1)
        err(EXIT_FAILURE, "faile to set socket to NON BLOCK");

    _servAddress.sin_family = AF_INET;
    _servAddress.sin_port = htons(atoi(port));
    _servAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(_servSock, (struct sockaddr *)&_servAddress, sizeof(_servAddress)) == -1)
        err(EXIT_FAILURE, "failed to bind");

    if (listen(_servSock, BACKLOG) == -1)
        err(EXIT_FAILURE, "failed to listen");

    //add the listen socket to the kqueue
    struct kevent change;
    EV_SET(&change, _servSock, EVFILT_READ, EV_ADD | EV_ENABLE , 0, 0, 0);
    _events.push_back(change);
}

void Server::run()
{
    while (true)
    {
        int n = kevent(_kq, _events.data(), _events.size(), _events.data(), MAX_EVENTS, NULL);
        if (n == -1)
            err(EXIT_FAILURE, "failed to fetch events");
        for (int i = 0; i < n; ++i) //TODO: refactor to iterator
        {
            if (_events[i].ident == _servSock)
            {
                int cliSock = accept(_servSock, NULL, NULL);
                if ((fcntl(cliSock, F_SETFL, fcntl(cliSock, F_GETFL) | O_NONBLOCK)) == -1)
                    err(EXIT_FAILURE, "failed to set socket to NONBLOCK");
                _users.push_back(new User(cliSock));
                struct kevent cliEvent;
                EV_SET(&cliEvent, cliSock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0 ,0);
                _events.push_back(cliEvent);
            }
            else
            {
                char buffer[BUFFER_SIZE];
                int cliSock = _events[i].ident;
                int readByte = recv(cliSock, buffer, sizeof(buffer), 0);
                if (readByte < 0)
                {
                    struct kevent cliEvent;
                    EV_SET(&cliEvent, cliSock, EVFILT_READ, EV_DELETE, 0, 0, 0);
                    _events.push_back(cliEvent);
                    //delete user from the vector and destruct the object
                }
                else
                {
                    std::cout << "Received msg: " << buffer << std::endl;
                }
                memset(buffer, 0, sizeof(buffer));
            }
        }
    }
}
