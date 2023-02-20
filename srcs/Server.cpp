#include "../includes/Server.hpp"

Server& Server::getInstance()
{
    return (_server);
}

void Server::servSetup(char *port)
{
    if ((_kq = kqueue()) == -1)
        err(EXIT_FAILURE, "failed to create kqueue");

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

void Server::run()
{
    struct kevent change;
	EV_SET(&change, _servSock, EVFILT_READ, EV_ADD | EV_ENABLE , 0, 0, 0);
	_changelist.push_back(change);

    while (true)
    {
        static struct kevent eventlist[MAX_EVENTS];
        int n = kevent(_kq, _changelist.data(), _changelist.size(), eventlist, MAX_EVENTS, NULL);
        if (n == -1)
            err(EXIT_FAILURE, "failed to fetch events");
        _changelist.clear();

        for (int i = 0; i < n; ++i)
        {
			char filter = FILTER(eventlist[i].filter);
			PRINT_FILTER(eventlist[i].ident, filter, Y);
			if (eventlist[i].flags & EV_EOF)
				this->handleEof(eventlist[i]);
			else if (eventlist[i].flags & EV_ERROR)
				this->handleError(eventlist[i]);
			else{
				if (eventlist[i].ident == _servSock)
					this->acceptUser();
				else if (eventlist[i].filter == EVFILT_READ)
					this->handleRead(eventlist[i]);
				else if (eventlist[i].filter == EVFILT_WRITE)
					this->handleWrite(eventlist[i]);
				else{
					PRINT_LOG(eventlist[i].ident, "SERVER: Unexpected Event Occured!", R);
					PRINT_EVENT(eventlist[i].ident, eventlist[i].flags, eventlist[i].filter, eventlist[i].fflags, eventlist[i].data, eventlist[i].udata, R);
				}
			}
        }
    }
}

void Server::handleEof(struct kevent &k){
	PRINT_LOG(k.ident, "SERVER : flags EOF, Client disconnected", R);
	close(k.ident);
}

void Server::handleError(struct kevent &k){
	PRINT_LOG(k.ident, "SERVER : flags ERROR", R);
	close(k.ident);
};

void Server::acceptUser(){
	int cliSock = accept(_servSock, NULL, NULL); //TODO: handle accept failure
	if ((fcntl(cliSock, F_SETFL, fcntl(cliSock, F_GETFL) | O_NONBLOCK)) == -1)
		err(EXIT_FAILURE, "failed to set socket to NONBLOCK");
	struct kevent cliEvent;
	EV_SET(&cliEvent, cliSock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0 ,0);
	_changelist.push_back(cliEvent);
	_clients.insert(std::make_pair(cliSock, Client(cliSock)));
	PRINT_LOG(cliSock, "SERVER : Accept client", B);
};

void Server::handleRead(struct kevent &k){
	char tmp[BUFFER_SIZE];
	memset(tmp, 0, BUFFER_SIZE);
	int readByte = recv(k.ident, tmp, BUFFER_SIZE, 0);
	if (readByte < 0){
		// TODO : disconnect process like QUIT command
		_clients.erase(k.ident);
	}
	if (readByte != 0){
		_clients.find(k.ident)->second.getBuffer() += tmp;
	}
	// WARNING! back() is C++11 function... we need to find better 
	if (_clients.find(k.ident)->second.getBuffer().back() == '\n' || \
			_clients.find(k.ident)->second.getBuffer().back() == '\r'){
		_invoker.commandConnector(k.ident, _clients.find(k.ident)->second.getBuffer().data());
		_clients.find(k.ident)->second.getBuffer().clear();
	}
};

void Server::handleWrite(struct kevent &currEvent){
	//TODO : udata length check
	std::string *send_msg = static_cast<std::string *>(currEvent.udata);
	int writeByte = send(currEvent.ident, send_msg->data(), send_msg->length(), 0);
	delete send_msg;
	//TODO: handle write failure
	if (writeByte == -1)
		exit(1);
	struct kevent wEvent;
	EV_SET(&wEvent, currEvent.ident, EVFILT_WRITE, EV_DELETE | EV_DISABLE, 0, 0 ,0);
	_changelist.push_back(wEvent);
};


std::map<int, Client>&	Server::getClients(){ return (_clients); }
std::map<std::string, Channel>&	Server::getChannels(){ return (_channels); }
std::vector<struct kevent>& Server::getChangeList(){ return (_changelist); }

void Server::setPswd(std::string pswd){ _password = pswd; }
std::string Server::getPswd() const { return (_password); }
