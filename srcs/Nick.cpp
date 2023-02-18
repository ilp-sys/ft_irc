#include "../includes/Nick.hpp"


Nick::Nick() : Command(2){};

int Nick::execute(std::vector<std::string>& cmdlist, User& user, std::vector<struct kevent>& changelist, std::map<std::string, Channel>& channels)
{
	
	Server 	&server = Server::getInstance();
	// User	&user = *(server._users.find(ident));	//?
	
	// if (!checkArgs(cmdlist))
	// 	return (1);	//errcode enum으로 관리	// args err 는 command에서 관리
	if (!isUnique(cmdlist[1], server.getUserMap()))
	{	// msg = ":" + "irc.local" + "433" + "root " + user.getNickname() + " :Nickname is already in use.";
		return (2);
	}
	else
	{
		std::string prevName = user.getNickname();
		user.setNickname(cmdlist[1]);
		//성공 시 write	//여기서 root도 다른 이름인 듯
		const std::string	msg = ":" + prevName + "!" + "root" + "@127.0.0.1" + "NICK :" + cmdlist[1];
		makeWriteEvent(user.getUserSock(), changelist, msg);
		return (0);
	}
}

bool	Nick::isUnique(const std::string& nickname, const std::map<int, User>& userMap) const
{
	std::map<int, User>::const_iterator	it = userMap.begin();
	for (; it != userMap.end(); it++)
		if (it->second.getNickname() == nickname)
			return (false);
	return (true);
}
