#include "../includes/User.hpp"

User::User() : Command(5){}

void	User::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
	(void)channels;
	(void)changelist;
	
	if (!checkArgs(cmdlist, client))
		return ;
	else
	{
		for (int i = 1; i < 5; i++)
			client.setUserInfo(cmdlist[i]);
		if (client.getNickname() != "*")
			client.setIsRegistered();
		//TODO: Welcome Message?
		//Write Event!
	}
}
bool	User::checkArgs(std::vector<std::string>& cmdlist, Client& client)
{
	if (cmdlist.size() < getRequiredArgsNumber())
	{
		makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NEEDMOREPARAMS(client.getNickname(), cmdlist[0]));
		return (false);
	}
	return (true);
}
