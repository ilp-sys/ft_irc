#include "../includes/CommandInvoker.hpp"
#include "../includes/Server.hpp"

// CommandInvoker::CommandInvoker()
// {
// 	//여기서 CommandMap 초기 세팅, _channels/changelist 세팅(만약에 한다면)?
// }

// void CommandInvoker::setCommand(std::string commandName, Command *command)
// {
// 	_commandMap[commandName] = command;
// }

// void CommandInvoker::executeCommand(std::string commandName, User &user)
// {
// 	Command* command = _commandMap[commandName];
// 	command->execute(user);
// }


void	CommandInvoker::parseString(const std::string& raw, std::vector<std::string>& cmds)
{
	size_t	idx = 0;
	size_t	len = 0;
	size_t	end = raw.length();

	cmds.clear();
	while (idx < end)
	{
		while (raw[idx + len] != '\n' && raw[idx + len] != '\r' && raw[idx + len] != '\0')
			len++;
		if (len > 0)
			cmds.push_back(raw.substr(idx, len));
		idx += (len + 1);
		len = 0;
	}
}

void	CommandInvoker::parseLine(const std::string& msg, std::vector<std::string>& cmdline)	//개행이나 /r처리해야
{
	size_t idx = 0;
	size_t len = 0;
	size_t end = msg.length();
	// std::vector<std::string> args;	//좀 더 좋은방법 없나...?

	cmdline.clear();
	while (idx < end)
	{
		while (msg[idx + len] != ' ' && msg[idx + len] != ':' && msg[idx + len] != '\0')
			len++;
		cmdline.push_back(msg.substr(idx, len));
		if (msg[idx + len] == ':')
		{
			cmdline.push_back(msg.substr(idx + len));
			break ;
		}
		idx += (len + 1);
		len = 0;
	}
	//여기서 명령어 호출
}

void CommandInvoker::commandConnector(int ident, const std::string& message, std::vector<struct kevent> &changelist)
{
	Server& server = Server::getInstance();	//이렇게 하는거랑 Class Server; 이렇게 선언하는 것의 작동 차이?
	std::vector<std::string> commands;
	std::vector<std::string> cmdline;

	parseString(message, commands);	//개행을 삭제해서 command 단위로 나눠준다
	std::vector<std::string>::iterator	it;	//command를 순서대로 parse -> 실행
	for (it = commands.begin(); it < commands.end(); it++)
	{
		parseLine(*it, cmdline);
		//executeCommand(//User, cmdlist, changelist, channel)	//만약 앞의 커맨드에서 실패한다면?
		std::vector<std::string>::iterator	jt;
		jt = cmdline.begin();
		for (; jt < cmdline.end(); jt++)
			std::cout << *jt << std::endl;
		std::cout << std::endl;
	}
}
