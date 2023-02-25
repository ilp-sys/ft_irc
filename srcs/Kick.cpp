#include "../includes/Kick.hpp"
#include "../includes/Server.hpp"

	//args는 3일 수도 있고 4일 수도 있다 3까지 must
Kick::Kick() : Command(3){}	//args는 3일 수도 있고 4일 수도 있다 3까지 must

void	Kick::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
	if (checkArgs(cmdlist, client) == false)
		return ;
	Server server = Server::getInstance();
	std::map<std::string, Channel>::iterator	ch_iter;
	std::string token = cmdlist[1];
	token.erase(0, 1);
	//channel check
	for (ch_iter = channels->begin(); ch_iter != channels->end(); ch_iter++)
	{
		if (ch_iter->second.getChannelName() == token)
			break ;
	}
	if (ch_iter == channels->end())
	{
		makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NOSUCHCHANNEL(client.getNickname(), cmdlist[1]));
		return ;
	}
	//you are not on the list
	if (ch_iter->second.findJoinClient(client.getNickname()) == false)
	{
		makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NOTONCHANNEL(client.getNickname(), cmdlist[1]));
		return ;
	}
	//is not operator
	if (ch_iter->second.getOpFd() != client.getUserSock())
	{
		makeWriteEvent(client.getUserSock(), changelist, ERR_CHANOPRIVSNEEDED(client.getNickname(), cmdlist[1]));
		return ;
	}
    std::map<int, Client> clients = server.getClients();
	std::map<int, Client>::const_iterator		cl_iter;
	//존재하지 않는 id
	for (cl_iter = clients.begin(); cl_iter != clients.end(); cl_iter++)
	{
		if (cl_iter->second.getNickname() == cmdlist[2])
			break ;
	}
	if (cl_iter == clients.end())
	{
		makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NOSUCHNICK(client.getNickname(), cmdlist[2]));
		return ;
	}
	//채널에 없는 아이디
	std::vector<Client *>::iterator	target;
	for (target = ch_iter->second.getClients().begin(); target != ch_iter->second.getClients().end(); target++)
	{
		if ((*target)->getNickname() == cmdlist[2])
			break ;
	}
	if (target == ch_iter->second.getClients().end())
	{
		makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_USERNOTINCHANNEL(client.getNickname(), cmdlist[2], cmdlist[1]));
		return ;
	}
	//다 존재하면? 해당 채널에서 user 삭제하고 모든 채널의 유저에게 메세지를 던진다
	//:aaaa!root@127.0.0.1 KICK #target qwer :go away (aaaa가 qwer을 #target에서 쫒아냄. 사유는 go away)
	std::string	msg;
	if (cmdlist.size() > 3)
		for (int i = 3; i < cmdlist.size(); i++)
			msg += std::string(cmdlist[i]);	//string merge	
	else
		msg = std::string("no reason");
	ch_iter->second.getClients().erase(target);
	makeWriteEvent((*target)->getUserSock(), Server::getInstance().getChangeList(), SUCCESS_REPL_KICK(client.getNickname(), client.getUserName(), client.getHostName(), cmdlist[0], cmdlist[1], cmdlist[2], msg));
	for (target = ch_iter->second.getClients().begin(); target != ch_iter->second.getClients().end(); target++)
		makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), SUCCESS_REPL_KICK(client.getNickname(), client.getUserName(), client.getHostName(), cmdlist[0], cmdlist[1], cmdlist[2], msg));
	//삭제
}

bool	Kick::checkArgs(std::vector<std::string>& cmdlist, Client& client)
{
	if (cmdlist.size() < static_cast<unsigned long>(getRequiredArgsNumber()))
	{
		makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NEEDMOREPARAMS(client.getNickname(), cmdlist[0]));
		return (false);
	}
	//채널 형식체크 out - 403 ERR_NOSUCHCHANNEL
	if (cmdlist[1][0] != '#')
	{
		makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NOSUCHCHANNEL(client.getNickname(), cmdlist[1]));
		return (false);
	}
	return (true);
}
