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

// void Server::cmdsSetup()
// {
//     _invoker.setCommand("Kick", new Kick());
//     _invoker.setCommand("Join", new Join());
//     _invoker.setCommand("Nick", new Nick());
//     _invoker.setCommand("Quit", new Quit());
//     _invoker.setCommand("Part", new Part());
// }

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
        std::cout << "\n" << n << " events occured!" << std::endl;
        if (n == -1)
            err(EXIT_FAILURE, "failed to fetch events");
        changelist.clear();

		// handle eventlist
        for (int i = 0; i < n; ++i)
        {
			char filter = FILTER(eventlist[i].filter);
			PRINT_FILTER(eventlist[i].ident, filter, Y);
			// PRINT_EVENT(eventlist[i].ident, eventlist[i].flags, eventlist[i].filter, eventlist[i].fflags, eventlist[i].data, eventlist[i].udata, Y);
			if (eventlist[i].flags & EV_EOF)
				this->handleEof(eventlist[i]);
			else if (eventlist[i].flags & EV_ERROR)
				this->handleError(eventlist[i]);
			else{
				if (eventlist[i].ident == _servSock)
					this->acceptUser(changelist);
				else if (eventlist[i].filter == EVFILT_READ)
					this->handleRead(eventlist[i], changelist);
				else if (eventlist[i].filter == EVFILT_WRITE)
					this->handleWrite(eventlist[i], changelist);
				else{
					PRINT_LOG(eventlist[i].ident, "SERVER: Unexpected Event Occured!", R);
					PRINT_EVENT(eventlist[i].ident, eventlist[i].flags, eventlist[i].filter, eventlist[i].fflags, eventlist[i].data, eventlist[i].udata, R);
				}
			}
        }
		std::cout << "eventlist loop end" << std::endl;
    }
}

void Server::handleEof(struct kevent &k){
	PRINT_LOG(k.ident, "SERVER : flags EOF, User disconnected", R);
	// PRINT_EVENT(k.ident, k.flags, k.filter, k.fflags, k.data, k.udata, R);
	close(k.ident);
}

void Server::handleError(struct kevent &k){
	PRINT_LOG(k.ident, "SERVER : flags ERROR", R);
	PRINT_EVENT(k.ident, k.flags, k.filter, k.fflags, k.data, k.udata, R);
	close(k.ident);
};

void Server::acceptUser(std::vector<struct kevent> &changelist){
	int cliSock = accept(_servSock, NULL, NULL); //TODO: handle accept failure
	if ((fcntl(cliSock, F_SETFL, fcntl(cliSock, F_GETFL) | O_NONBLOCK)) == -1)
		err(EXIT_FAILURE, "failed to set socket to NONBLOCK");
	struct kevent cliEvent;
	EV_SET(&cliEvent, cliSock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0 ,0);
	changelist.push_back(cliEvent);
	_users.insert(std::make_pair(cliSock, User(cliSock)));
	PRINT_LOG(cliSock, "SERVER : Accept user", B);
};

void Server::handleRead(struct kevent &k, std::vector<struct kevent> &changelist){
	char tmp[BUFFER_SIZE];
	memset(tmp, 0, BUFFER_SIZE);
	int readByte = recv(k.ident, tmp, BUFFER_SIZE, 0);
	if (readByte < 0){
		// TODO : disconnect process like QUIT command..?.
		_users.erase(k.ident);
	}
	if (readByte != 0){
		_users.find(k.ident)->second.getBuffer() += tmp;
		// PRINT_MSG(k.ident, "server receive tmp_msg ", tmp, Y);
	}
	// WARNING! back() is C++11 function... we need to find better 
	if (_users.find(k.ident)->second.getBuffer().back() == '\n' || \
			_users.find(k.ident)->second.getBuffer().back() == '\r'){
		// PRINT_MSG(k.ident, "server recive full_msg ", _users.find(k.ident)->second.getBuffer(), G);
		testServer(k, changelist);
		// _invoker.commandConnector(k.ident, _users.find(k.ident)->second.getBuffer().data(), changelist);
		_users.find(k.ident)->second.getBuffer().clear(); // 여기서 클리어하는게 눈에 잘보여서 좋지않나용?
	}
};

void Server::handleWrite(struct kevent &currEvent, std::vector<struct kevent> &changelist){
	//TODO : udata length check
	int writeByte = send(currEvent.ident, currEvent.udata, 12, 0);
	PRINT_LOG(currEvent.ident, "Server send to", B);
	delete((std::string *)currEvent.udata);
	//TODO: handle write failure
	if (writeByte == -1)
		exit(1);
	struct kevent wEvent;
	EV_SET(&wEvent, currEvent.ident, EVFILT_WRITE, EV_DELETE | EV_DISABLE, 0, 0 ,0);
	changelist.push_back(wEvent);
};

void Server::setPswd(std::string pswd)
{
    _password = pswd;
}

std::string Server::getPswd() const
{
    return (_password);
}

// 일단 서버와 소통이 되는지 확인하기 위해서 만든 테스트 함수
void Server::testServer(struct kevent &currEvent, std::vector<struct kevent> &changelist){
	PRINT_MSG(currEvent.ident, "Socket", _users.find(currEvent.ident)->second.getBuffer(), G);
	struct kevent wEvent;
	EV_SET(&wEvent, currEvent.ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
	std::string *mydata = new std::string("here we go\n");
	wEvent.udata = (void *)mydata;
	changelist.push_back(wEvent);
}