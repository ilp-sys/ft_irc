#include "../includes/Nick.hpp"
#include "../includes/Server.hpp"


Nick::Nick() : Command(2){}

//INFO : evenif length is more than 2, it only takes 2
bool	Nick::checkArgs(std::vector<std::string>& cmdlist, Client& client)
{
	if (cmdlist.size() < 2)
	{
		makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NEEDMOREPARAMS(client.getNickname(), "NICK"));
		return (false);
	}
	return (true);
}

int Nick::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
	
	Server 	&server = Server::getInstance();
	
	if (!checkArgs(cmdlist, client))
		return (1);
	else
	{
		std::string prevName = client.getNickname();
		client.setNickname(cmdlist[1]);
		//TODO: use define......
		std::string	msg = "NICK :" + cmdlist[1];
		makeWriteEvent(client.getUserSock(), changelist, msg);
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
