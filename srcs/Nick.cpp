#include "../includes/Nick.hpp"
#include "../includes/Server.hpp"


Nick::Nick() : Command(2){};

int Nick::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
	
	Server 	&server = Server::getInstance();
	
	// if (!checkArgs(cmdlist))
	// 	return (1);	//errcode enum으로 관리	// args err 는 command에서 관리
	
	// if (!isUnique(cmdlist[1], server.getUserMap()))
	// {	// msg = ":" + "irc.local" + "433" + "root " + client.getNickname() + " :Nickname is already in use.";
	// 	return (2);
	// }

	// else
	// {
		std::string prevName = client.getNickname();
		client.setNickname(cmdlist[1]);
		//TODO: err in sending message to event
		//TODO: use define......
		std::string	msg = "NICK :" + cmdlist[1];
		makeWriteEvent(client.getUserSock(), changelist, msg);
		return (0);
	// }
}

bool	Nick::isUnique(const std::string& nickname, const std::map<int, Client>& userMap) const
{
	std::map<int, Client>::const_iterator	it = userMap.begin();
	for (; it != userMap.end(); it++)
		if (it->second.getNickname() == nickname)
			return (false);
	return (true);
}
