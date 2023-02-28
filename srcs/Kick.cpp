#include "../includes/Kick.hpp"
#include "../includes/Server.hpp"

  //args는 3일 수도 있고 4일 수도 있다 3까지 must
Kick::Kick() : Command(3){} //args는 3일 수도 있고 4일 수도 있다 3까지 must

void  Kick::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
  if (checkArgs(cmdlist, client) == false)
    return ;
  Server &server = Server::getInstance();
  std::map<std::string, Channel>::iterator  ch_iter;
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
  std::map<int, Client>::const_iterator   cl_iter;
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
  std::vector<Client *>::iterator target;
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
  //:aaaa!root@127.0.0.1 KICK #target qwer :go away (aaaa가 qwer을 #target에서 쫒아냄. 사유는 go away
  if (cmdlist.size() == 4 && cmdlist[3] == ":")
	cmdlist.pop_back();
  if (cmdlist.size() < 4)
	cmdlist.push_back("no reason");
  makeWriteEvent((*target)->getUserSock(), Server::getInstance().getChangeList(), SUCCESS_REPL(client.getNickname(), mergeMsg(cmdlist)));
  ch_iter->second.getClients().erase(target); // 채널 안에 나 지우기 얘는 이터레이터 pos가 호환되는 애 맞음
  for (std::vector<Channel *>::iterator it = client.getJoinedChannel().begin(); it != client.getJoinedChannel().end(); ++it ){
	if ((*it)->getChannelName() == token){
		client.getJoinedChannel().erase(it);// 내 안에 채널 지우기
		break;
	}
  }
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
