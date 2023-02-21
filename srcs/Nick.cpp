#include "../includes/Nick.hpp"
#include "../includes/Server.hpp"

Nick::Nick() : Command(2), SPECIAL("[]\\\\`_^{|}"){}

//TODO: Welcome Protocal 어떻게 할지 정하기 -> Nick의 경우 unregistered 상태에서 
//TODO: 9자리 까지 체크하면 된다고는 하지만? 실제로는 ...
bool	Nick::checkArgsFormat(std::string& newnick)
{
	int	limit;

	if (std::isalpha(newnick.data()[0]) == 0 && SPECIAL.find(newnick.data()[0]) == std::string::npos)
		return (false);
	limit = newnick.length();
	if (limit > 9)
		limit = 9;
	for (int i = 1; i < limit; i++)
	{
		if (newnick.data()[i] != '-' && (std::isdigit(newnick.data()[i]) == 0) \
			&& (std::isalpha(newnick.data()[i]) == 0) && SPECIAL.find(newnick.data()[i]) == std::string::npos)
			return (false);
	}
	return (true);
}
//INFO : evenif length is more than 2, it only takes 2
bool	Nick::checkArgs(std::vector<std::string>& cmdlist, Client& client)
{
	if (cmdlist.size() < getRequiredArgsNumber())
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
	std::map<int, Client>::iterator	it;
	
	for (it = clientList.begin(); it != clientList.end(); it++)
		if ((*it).second.getNickname() == candidate)
			return (false);
	return (true);
}

void Nick::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
	Server 	&server = Server::getInstance();
	
	//이 단계에서 isPassed check
	if (!checkArgs(cmdlist, client))
		return ;
	else if (isNickExist(server.getClients(), cmdlist[1]) == false)
	{
		std::cout << B << "HERE" << N << std::endl;
		makeWriteEvent(client.getUserSock(), server.getChangeList(), ERR_NICKNAMEINUSE(client.getNickname(), cmdlist[1]));
	}
	else
	{
		std::string prevName = client.getNickname();
		client.setNickname(cmdlist[1]);
		std::cout << R << client.getNickname() << N << std::endl;
		//_namkim-nick!root@127.0.0.1 NICK :soyoung
		//TODO: register 되지 않으면, makeWriteEvent 발생하지 않음
		//USER 명령어로 무엇을 등록하든, root/ip 자리에는 클라이언트가 해석하는 것 같음.
		//TODO: SUCCESS_REPL 에서 segv
		// std::string *msg = new std::string(SUCCESS_REPL("prevName", client.getUserName(), client.getHostName(), cmdlist[0]));
		makeWriteEvent(client.getUserSock(), server.getChangeList(), SUCCESS_REPL("prevName", "userName", "HostName", cmdlist[0]));
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
