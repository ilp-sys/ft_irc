#include "../includes/CommandInvoker.hpp"
#include "../includes/Nick.hpp"
#include "../includes/Server.hpp"

class Server;	//이렇게 이용 가능?


CommandInvoker::CommandInvoker()
{
	//여기서 CommandMap 초기 세팅
	Nick	nick;	//이 scope에서만 유효하지 않나요 멤버변수로 등록....?
	addCommand("Nick", &nick);	//그러면...? 여기서 개별 command header 다 가지고 있어야...
	//server가 changelist 가지고 있으면 안 되냐구 흑흑
	// _channels = getInstance().getChannel();
}

void CommandInvoker::addCommand(std::string commandName, Command* command)
{
	_commandMap.insert(std::make_pair<std::string, Command*>(commandName, command));
}

// void CommandInvoker::setCommand(std::string commandName, Command *command)
// {
// 	_commandMap[commandName] = command;
// }

int CommandInvoker::executeCommand(std::vector<std::string> &cmdline, int ident, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
	int	res;

	if (_commandMap.find(cmdline[0]) == _commandMap.end())
	{
		//write no such command errmsg; ???
		return (-1);
	}
	else
	{
		User &user = Server::getUserMap().find(ident)->second;
		Command *command = _commandMap.find(cmdline[0])->second;	//std::map::end의 value 참조는 undefined behavior)
		return (command->execute(cmdline, user, changelist, NULL));	//cmdline 2번째 인자부터 끝까지 넘길 수 있는지?
			//실제로는 error 더욱 다양... => bool 외에 errcode 써야
	}
}


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
		executeCommand(cmdline, ident, changelist, NULL);	//만약 앞의 커맨드에서 실패한다면?
		// std::vector<std::string>::iterator	jt;
		// jt = cmdline.begin();
		// for (; jt < cmdline.end(); jt++)
		// 	std::cout << *jt << std::endl;
		// std::cout << std::endl;
	}
}
