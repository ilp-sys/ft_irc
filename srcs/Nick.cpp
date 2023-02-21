#include "../includes/Nick.hpp"
#include "../includes/Server.hpp"

Nick::Nick() : Command(2){}

//TODO: 9자리 까지 체크하면 된다고는 하지만? 실제로는 ...
bool	Nick::checkArgsFormat(std::string& newnick)
{
	if (std::isalpha(newnick.data()[0]) && SPECIAL.find(newnick.data()[0]) == SPECIAL::npos)
		return (false);
	for (int i = 1; i < 9; i++)
	{
		if (newnick.data[i] != '-' && std::isdigit(newnick.data()[i]) \
			&&std::isalpha(newnick.data()[i]) && SPECIAL.find(newnick.data()[i]) == SPECIAL::npos)
			return (false);
	}
	return (true);
}
//INFO : evenif length is more than 2, it only takes 2
bool	Nick::checkArgs(std::vector<std::string>& cmdlist, Client& client)
{
	if (cmdlist.size() < 2)
	{
		makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NEEDMOREPARAMS(client.getNickname(), cmdlist[0]));
		return (false);
	}
	if (checkArgsFormat(cmdlist[1]) == false)
	{
		makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_ERRONEOUSNICKNAME(client.getNickname(), cmdlist[1]));
		return (false);
	}
	return (true);
}

bool	Nick::isNickExist(std::map<int, Client>& clientList, std::string& candidate)
{
	std::map::iterator	it;
	it = clientList.begin();
	while (; it != clientList.end(); it++)
	{
		if ((*it)->second.getNickname() == candidate)
		{
			makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NICKNAMEINUSE(client, nick)(client.getNickname(), cmdlist[1]));
			return (false);
		}
	}
	return (true);
}

int Nick::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
	
	Server 	&server = Server::getInstance();
	
	if (!checkArgs(cmdlist, client))
		return (1);
	else if (isNickExist(server.getClients(), cmdlist[1]) == false)
		return (1);
	else
	{
		std::string prevName = client.getNickname();
		client.setNickname(cmdlist[1]);
		//_namkim-nick!root@127.0.0.1 NICK :soyoung
		//TODO: register 되지 않으면, makeWriteEvent 발생하지 않음
		//USER 명령어로 무엇을 등록하든, root/ip 자리에는 클라이언트가 해석하는 것 같음.
		makeWriteEvent(client.getUserSock(), changelist, SUCCESS_REPL(prevName, client, host, cmd));
		return (0);
	}
}

bool	Nick::isUnique(const std::string& nickname, const std::map<int, Client>& userMap) const
{
	std::map<int, Client>::const_iterator	it = userMap.begin();
	for (; it != userMap.end(); it++)
		if (it->second.getNickname() == nickname)
			return (false);
	return (true);
}
