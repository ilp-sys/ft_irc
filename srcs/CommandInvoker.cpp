#include "../includes/CommandInvoker.hpp"

void CommandInvoker::setCommand(std::string commandName, Command *command)
{
    _commandMap[commandName] = command;
}

void CommandInvoker::executeCommand(std::string commandName, User &user)
{
    Command* command = _commandMap[commandName];
    command->execute(user);
}

void CommandInvoker::commandConnector(User &user, const std::string& msg, std::vector<struct kevent>& changelist;)	//event가 들어올 수 있다 //execute Command 가 User를 받는다?!
{
	//tree를 순회하면서
	// Server& server = Server::getInstance();
	std::map<int, std::string>::iterator it;
	for (it = _commandMap.begin(); it != _commandMap.end(); it++)
	{	
		if (startWith(msg, it->first))
		{
			//executeCommand(it->first, user);	//userlist, channallist 둘 다 전달해줘야;
			_commandMap[it->first].execute(user, args)	// 가 구조적으로 맞는 것 같은데...?	//changelist도 전달해줘야...
			break ;
		}
	}
	//throw noSuchCommand exception
}

bool	startWith(const char *msg, const char *prefix)
{
	return (msg.compare(prefix) == 0 && msg.size() >= prefix.size());
}
