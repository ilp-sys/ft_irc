#include "../includes/Kick.hpp"
#include "../includes/Server.hpp"

Kick::Kick() : Command(3){}

bool  Kick::checkArgs(std::vector<std::string>& cmdlist, Client& client)
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

void  Kick::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
  if (checkArgs(cmdlist, client) == false)
    return ;
  Server &server = Server::getInstance();
  std::map<std::string, Channel>::iterator  servChanIter;
  std::string token = cmdlist[1];
  token.erase(0, 1);

  //check if channel exist
  if ((servChanIter = channels->find(token)) == channels->end())
  {
    makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NOSUCHCHANNEL(client.getNickname(), cmdlist[1]));
    return ;
  }

  //check if the executor exist in the channel
  if (servChanIter->second.findJoinClient(client.getNickname()) == false)
  {
    makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NOTONCHANNEL(client.getNickname(), cmdlist[1]));
    return ;
  }

  //check privilege
  if (servChanIter->second.getOpFd() != client.getUserSock())
  {
    makeWriteEvent(client.getUserSock(), changelist, ERR_CHANOPRIVSNEEDED(client.getNickname(), cmdlist[1]));
    return ;
  }

  //check if target exist in the server
  std::map<int, Client>::const_iterator servCliIter;
  for (servCliIter = server.getClients().begin(); servCliIter != server.getClients().end(); ++servCliIter)
  {
    if (servCliIter->second.getNickname() == cmdlist[2])
      break ;
  }
  if (servCliIter == server.getClients().end())
  {
    makeWriteEvent(client.getUserSock(), changelist, ERR_NOSUCHNICK(client.getNickname(), cmdlist[2]));
    return ;
  }

  //check if target exist in the channel
  std::vector<Client *>::iterator chanTargetIter;
  for (chanTargetIter = servChanIter->second.getClients().begin(); chanTargetIter != servChanIter->second.getClients().end(); ++chanTargetIter)
  {
    if ((*chanTargetIter)->getNickname() == cmdlist[2])
      break ;
  }
  if (chanTargetIter == servChanIter->second.getClients().end())
  {
    makeWriteEvent(client.getUserSock(), changelist, ERR_USERNOTINCHANNEL(client.getNickname(), cmdlist[2], cmdlist[1]));
    return ;
  }

  //trim cmdlist
  if (cmdlist.size() == 4 && cmdlist[3] == ":")
<<<<<<< HEAD
      cmdlist.pop_back();
  else if (cmdlist.size() < 4)
      cmdlist.push_back("no reason");
=======
	cmdlist.pop_back();
  if (cmdlist.size() < 4)
	cmdlist.push_back("no reason");
  makeWriteEvent((*target)->getUserSock(), Server::getInstance().getChangeList(), SUCCESS_REPL(client.getNickname(), mergeMsg(cmdlist)));
  for (std::vector<Channel *>::iterator it = (*target)->getJoinedChannel().begin(); it != (*target)->getJoinedChannel().end(); ++it ){
	if ((*it)->getChannelName() == token){
		(*target)->getJoinedChannel().erase(it);// 내 안에 채널 지우기
		break;
	}
  }
  ch_iter->second.getClients().erase(target); // 채널 안에 나 지우기 얘는 이터레이터 pos가 호환되는 애 맞음
  // 방장 위임 또는 채널 폐쇄
  if (ch_iter->second.getClients().size() > 0){
	ch_iter->second.setOpFd(ch_iter->second.getClients().front()->getUserSock());
	for (target = ch_iter->second.getClients().begin(); target != ch_iter->second.getClients().end(); target++)
    	makeWriteEvent((*target)->getUserSock(), Server::getInstance().getChangeList(), SUCCESS_REPL(client.getNickname(), mergeMsg(cmdlist)));
  }
  else{
	server.getChannels().erase(cmdlist[1].erase(0, 1));
  }
}
>>>>>>> 8a306370bd9becb5df34f160a5c635f7950d5688

  //notify to all users in the channel -> didn't erase yet
  for (std::vector<Client*>::iterator it = servChanIter->second.getClients().begin(); it != servChanIter->second.getClients().end(); ++it)
      makeWriteEvent((*it)->getUserSock(), changelist, SUCCESS_REPL(client.getNickname(), mergeMsg(cmdlist)));

  int targetSock = (*chanTargetIter)->getUserSock();
  for (std::vector<Channel*>::iterator it = (*chanTargetIter)->getJoinedChannel().begin(); it != (*chanTargetIter)->getJoinedChannel().end(); ++it)
  {
    if ((*it)->getChannelName() == token) //erase channel from the target's channel list
    {
      (*chanTargetIter)->getJoinedChannel().erase(it);
      break;
    }
  }
  servChanIter->second.getClients().erase(chanTargetIter); //erase target from channel's client list
 
  //delegate privilege if the target is operator or delete channel
  if (targetSock == servChanIter->second.getOpFd())
  {
    if (servChanIter->second.getClients().size() > 0)
    {
        servChanIter->second.setOpFd(servChanIter->second.getClients().front()->getUserSock());
    }
    else
        server.getChannels().erase(token);
  }
}
