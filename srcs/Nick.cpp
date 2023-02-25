#include "../includes/Nick.hpp"
#include "../includes/Server.hpp"
#include <set>

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
	std::map<int, Client>::iterator	it;
	
	for (it = clientList.begin(); it != clientList.end(); it++)
		if ((*it).second.getNickname() == candidate)
			return (false);
	return (true);
}

void Nick::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
  Server  &server = Server::getInstance();
  (void)channels;

  if (!checkArgs(cmdlist, client))
    return ;
  else if (isNickUnique(server.getClients(), cmdlist[1]) == false)
    makeWriteEvent(client.getUserSock(), changelist, ERR_NICKNAMEINUSE(client.getNickname(), cmdlist[1]));
  else
  {
    std::string prevName = client.getNickname();
    client.setNickname(cmdlist[1]);
    if (client.getIsRegistered() == false)
    {
	  if (client.getUserInfo().size() == 4)
      {
        client.setIsRegistered();
        makeWriteEvent(client.getUserSock(), changelist, RPL_WELCOME(client.getNickname(), client.getUserName(), client.getHostName()));
        return ;
      }
    }//TODO: registered 안 되었을 때도 write 할 지 결정하기
    else
	{	//내가 존재하는 모든 채널의 유저에게 한번만 write
		std::set<int> list;
		list.clear();	//necessary?
		std::vector<Channel *>::iterator	it;
		for (it = client.getJoinedChannel().begin(); it != client.getJoinedChannel().end(); it++)
		{
			std::vector<Client*>::iterator	cl_it;
			for (cl_it = (*it)->getClients().begin(); cl_it != (*it)->getClients().end(); cl_it++)
				list.insert((*cl_it)->getUserSock());
		}
		std::set<int>::iterator s_it;
		for (s_it = list.begin(); s_it != list.end(); s_it++)
			makeWriteEvent(*(s_it), changelist, SUCCESS_REPL(prevName, mergeVec(cmdlist)));
	}
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
