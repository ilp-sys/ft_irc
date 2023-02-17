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

    if (bind(_servSock, (struct sockaddr *)&_servAddress, sizeof(_servAddress)) == -1) //TODO: address already in use
        err(EXIT_FAILURE, "failed to bind");

    if (listen(_servSock, BACKLOG) == -1)
        err(EXIT_FAILURE, "failed to listen");
}

void Server::run()
{
    std::vector<struct kevent> changelist;

    struct kevent change;//
	EV_SET(&change, _servSock, EVFILT_READ, EV_ADD | EV_ENABLE , 0, 0, 0);//
	changelist.push_back(change);

    while (true)
    {
        static struct kevent eventlist[MAX_EVENTS];
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
                int cliSock = accept(_servSock, NULL, NULL);
                if ((fcntl(cliSock, F_SETFL, fcntl(cliSock, F_GETFL) | O_NONBLOCK)) == -1)
                    err(EXIT_FAILURE, "failed to set socket to NONBLOCK");
                struct kevent cliEvent;
                EV_SET(&cliEvent, cliSock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0 ,0);
                changelist.push_back(cliEvent);
				User *newUser = new User(cliSock);
				_users.insert(cliSock, newUser);	//should i newly allocate?
				//some routine for new user
                //_users.insert({});
            }
            else
            {
                char buffer[BUFFER_SIZE];
                int cliSock = eventlist[i].ident;
                int readByte = recv(cliSock, buffer, sizeof(buffer), 0);
				std::cout << "readByte: " << readByte << std::endl;
                if (readByte <= 0)
                {
                    struct kevent cliEvent;
                    EV_SET(&cliEvent, cliSock, EVFILT_READ, EV_DELETE, 0, 0, 0); //TODO: why do we have to set EVFILT_READ for this call?
                    changelist.push_back(cliEvent);
					_users.erase(_users.find(cliSock));	//지운다
                    //delete user from the server 
					shutdown(cliSock, SHUT_RDWR);	//may we use it?
					close(cliSock);
                }
                else
                {
                    //parse the command and stuff..
                    std::cout << "Received msg: " << buffer << std::endl;
					//read -> command handler ->
					//parse -> 연관된 메세지 호출
					//
                }
                memset(buffer, 0, sizeof(buffer));
            }
        }
    }
}
