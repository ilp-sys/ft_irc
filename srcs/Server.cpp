#include "../includes/Server.hpp"
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

    if (bind(_servSock, (struct sockaddr *)&_servSock, sizeof(_servSock)) == -1)
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
        int n = kevent(_kq, _events.data(), _events.size(), _events.data(), MAX_EVETNS, NULL);
        if (n == -1)
            err(EXIT_FAILURE, "failed to fetch events");
        for (int i = 0; i < n; ++i)
        {
            if (_events[i].ident == _servSock)  //new client arrived
            {
            }
            else
            {
            }
        }
    }
}
