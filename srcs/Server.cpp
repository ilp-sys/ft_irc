#include "../includes/Server.hpp"

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

    int value = true;
    setsockopt(_servSock, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));

    if ((fcntl(_servSock, F_SETFL, fcntl(_servSock, F_GETFL) | O_NONBLOCK)) == -1)
        err(EXIT_FAILURE, "faile to set socket to NON BLOCK");

    struct sockaddr_in _servAddress;
    _servAddress.sin_family = AF_INET;
    _servAddress.sin_port = htons(atoi(port));
    _servAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(_servSock, (struct sockaddr *)&_servAddress, sizeof(_servAddress)) == -1)
        err(EXIT_FAILURE, "failed to bind");

    if (listen(_servSock, BACKLOG) == -1)
        err(EXIT_FAILURE, "failed to listen");
}

void Server::cmdsSetup()
{
    _invoker.setCommand("Kick", new Kick());
    _invoker.setCommand("Join", new Join());
    _invoker.setCommand("Nick", new Nick());
    _invoker.setCommand("Quit", new Quit());
    _invoker.setCommand("Part", new Part());
}

void Server::run()
{
    std::vector<struct kevent> changelist;

    struct kevent change;
    EV_SET(&change, _servSock, EVFILT_READ, EV_ADD | EV_ENABLE , 0, 0, 0);
    changelist.push_back(change);

    while (true)
    {
        struct kevent eventlist[MAX_EVENTS];
        int n = kevent(_kq, changelist.data(), changelist.size(), eventlist, MAX_EVENTS, NULL);
        std::cout << n << " events occured!" << std::endl;
        if (n == -1)
            err(EXIT_FAILURE, "failed to fetch events");
        changelist.clear();

        for (int i = 0; i < n; ++i)
        {
            if (eventlist[i].ident == _servSock)
            {
                //set the new client socket to nonblock and add it to changelist
                int cliSock = accept(_servSock, NULL, NULL); //TODO: handle accept failure
                if ((fcntl(cliSock, F_SETFL, fcntl(cliSock, F_GETFL) | O_NONBLOCK)) == -1)
                    err(EXIT_FAILURE, "failed to set socket to NONBLOCK");
                struct kevent cliEvent;
                EV_SET(&cliEvent, cliSock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0 ,0);
                changelist.push_back(cliEvent);

                //some routine for new user
                //_users.insert({});
            }
            else if (eventlist[i].filter & EVFILT_READ)
            {
                eventlist[i].udata = new char[BUFFER_SIZE];
                int readByte = recv(eventlist[i].ident, eventlist[i].udata, BUFFER_SIZE, 0);
                if (readByte <= 0)
                {
                    struct kevent cliEvent;
                    EV_SET(&cliEvent, eventlist[i].ident, EVFILT_READ, EV_DELETE, 0, 0, 0);
                    changelist.push_back(cliEvent);
                    //delete user from the server 
                    //close the connection
                }
                else
                {
                    _invoker.commandConnector(eventlist[i].ident, eventlist[i].udata, changelist);
                }
                delete static_cast<char *>(eventlist[i].udata);
            }
            else if (eventlist[i].filter & EVFILT_WRITE)
            {
                int writeByte = send(eventlist[i].ident, eventlist[i].udata, std::strlen((const char*)eventlist[i].udata), 0);
                //TODO: handle write failure
            }
            else
                std::cerr << "[ERROR] Unexpected operations occured!" << std::endl;
        }
    }
}
