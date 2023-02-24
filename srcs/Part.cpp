#include "../includes/Part.hpp"
#include "../includes/Define.hpp"
#include "../includes/Server.hpp"
#include <sstream>

static void print_joinedClient(Channel &c){
	std::cout << &c << "[" <<  c.getChannelName() << "] ..... " << std::endl;
	for (std::vector<Client *>::iterator it = c.getClients().begin(); it != c.getClients().end(); ++it){
		std::cout << "...." << (*it)->getNickname() << std::endl;
	}
}

static void print_all_about_channel(Channel &c){
	std::cout << "part call all about\n		channel address : " << &c << std::endl;
    std::vector<Client *> clients = c.getClients();
    std::cout << "		print all about " <<  c.getChannelName() << std::endl;
    for (int i = 0; i != clients.size(); ++i){
        std::cout << "		" << clients[i]->getNickname() << std::endl;
    }
}

Part::Part() : Command(0){};

void  Part::execute(std::vector<std::string>& cmdlist, Client& client, \
		std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels){
	Server& server = Server::getInstance();
	
	if (cmdlist.size() < 2){
		makeWriteEvent(client.getUserSock(), server.getChangeList(), ERR_NEEDMOREPARAMS(client.getNickname(), "PART"));
	}
	else {
		std::stringstream ss(cmdlist[1]);
		std::string token;
		while (getline(ss, token, ','))
		{
			token.erase(0, 1);
			
			std::map<std::string, Channel>::iterator found = server.getChannels().find(token);
			if (found == Server::getInstance().getChannels().end()){
				makeWriteEvent(client.getUserSock(), server.getChangeList(), ERR_NOSUCHCHANNEL(client.getNickname(), token));
				continue;
			}

			Channel &targetChannel = found->second;
			if (targetChannel.findJoinClient(client.getNickname()) == false){
				makeWriteEvent(client.getUserSock(), server.getChangeList(), ERR_NOTONCHANNEL(client.getNickname(), token));
				continue;
			}

			// 채널의 접속자 리스트에서 나를 찾아서 삭제
			for (std::vector<Client *>::iterator it = targetChannel.getClients().begin(); it != targetChannel.getClients().end(); ++it){
				if ((*it)->getNickname() == client.getNickname()){
					targetChannel.getClients().erase(it);
					break;
				}
			}
			// 타겟 채널에 아무도 없으면 채널삭제 || 채널이벤트 메세지를 채널 안에 보내기
			if (targetChannel.getClients().size() == 0)
				server.getChannels().erase(targetChannel.getChannelName());
			else{
				for (std::vector<Client *>::iterator it = targetChannel.getClients().begin(); it != targetChannel.getClients().end(); ++it)
					makeWriteEvent((*it)->getUserSock(), server.getChangeList(), SUCCESS_REPL((*it)->getUserName(), (*it)->getHostName(), "127.0.0.1", mergeVec(cmdlist)));
			}
			// client가 접속해있는 채널 리스트에서 타겟삭제
			for (std::vector<Channel *>::iterator it = client.getJoinedChannel().begin(); it != client.getJoinedChannel().end(); ++it){
				if ((*it)->getChannelName() == targetChannel.getChannelName()){
					client.getJoinedChannel().erase(it);
					break;
				}
			}			
		} 
	}
	return;
}
bool  Part::checkArgs(std::vector<std::string>& cmdlist, Client& client){
	return true;
}