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
	if (cmdlist.size() < static_cast<unsigned long>(getRequiredArgsNumber()))
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

bool	Nick::isNickUnique(const std::map<int, Client>& clientList, const std::string& candidate) const
{
	std::map<int, Client>::const_iterator	it;
	
	for (it = clientList.begin(); it != clientList.end(); it++)
		if ((*it).second.getNickname() == candidate)
			return (false);
	return (true);
}

void Nick::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
	Server 	&server = Server::getInstance();
	(void)channels;

	if (!checkArgs(cmdlist, client))
		return ;
	else if (isNickUnique(server.getClients(), cmdlist[1]) == false)
		makeWriteEvent(client.getUserSock(), changelist, ERR_NICKNAMEINUSE(client.getNickname(), cmdlist[1]));
	else
	{
		std::string prevName = client.getNickname();
		client.setNickname(cmdlist[1]);
		//_namkim-nick!root@127.0.0.1 NICK :soyoung
		//USER 명령어로 무엇을 등록하든, root/ip 자리에는 클라이언트가 해석하는 것 같음.
		//TODO: SUCCESS_REPL 에서 segv -> check
		//TODO: registered 안 되었을 때도 write 할 지 결정하기
		if (client.getIsRegistered() == true)
			makeWriteEvent(client.getUserSock(), changelist, SUCCESS_REPL(prevName, client.getNickname(), client.getHostName(), cmdlist[0]));
	}
}

