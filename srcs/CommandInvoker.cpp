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

void CommandInvoker::commandConnector(int ident, const std::string& message, std::vector<struct kevent> &changelist)
{
	//tree를 순회하면서
	// Server& server = Server::getInstance();
	parseString(message);	//automatic?
	std::map<int, std::string>::iterator it;
	for (it = _commandMap.begin(); it != _commandMap.end(); it++)
	{	
	if (it->first == _input[0])
	{
	//executeCommand(it->first, user);	//userlist, channallist 둘 다 전달해줘야;
	_commandMap.find(it->first)->second.execute(ident, _input, changelist);	// 가 구조적으로 맞는 것 같은데...?	//changelist도 전달해줘야...
	break ;
	}
	}
	//throw noSuchCommand exception
}

// bool	startWith(const char *msg, const char *prefix)
// {
// 	return (msg.compare(prefix) == 0 && msg.size() >= prefix.size());
// }

void	parseString(const std::string& msg)
{
	size_t idx = 0;
	size_t len = 0;
	size_t end = msg.length();

	while (idx < end)
	{
		while (msg[idx + len] != ' ' && msg[idx + len] != ':' && msg[idx + len] != '\0')
			len++;
		_input.push_back(msg.substr(idx, len));
		if (msg[idx + len] == ':')
		{
			_input.push_back(msg.substr(idx + len));
			break ;
		}
		idx += (len + 1);
		len = 0;
	}
}
