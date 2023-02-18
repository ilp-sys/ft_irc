#include "../includes/Nick.hpp"
#include "../includes/Command.hpp"
#include "../includes/Server.hpp"
#include "../includes/User.hpp"

Nick() : _requiredArgsNumber(2){}

void Nick::execute(int ident, std::vector<std::string>& cmdlist, std::vector<struct kevent>& changelist)
{
	
	Server 	&server = getInstance();
	User	&user = *(server._users.find(ident));	//?
	std::string	msg;
	
	if (checkArgs(cmdlist) && isUnique(nickname, server->users))
	{
		std::string prevName;
		prevName = user.getNickname();
		user.setNickname(cmdlist[1]);
	//성공 시 write	//여기서 root도 다른 이름인 듯
		msg = ":" + prevName + "!" + "root" + "@127.0.0.1" + "NICK :" + cmdlist[1];
	}
	else
		msg = ":" + "irc.local" + "433" + "root " + user.getNickname() + " :Nickname is already in use.";
	makeWriteEvent(ident, changelist, &msg);
}

bool	Nick::isUnique(const std::string& nickname, const std::map<int, User>& userMap) const
{
	std::map::iterator	it;
	for (it = userMap.begin(); it != userMap.end(); it++)
		if (it->second->getNickname() == nickname)
			return (false);
	return (true);
}
