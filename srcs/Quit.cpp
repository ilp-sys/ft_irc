#include "../includes/Quit.hpp"
#include "../includes/Server.hpp"

Quit::Quit() : Command(1){};


void  Quit::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels){
	Server &server = Server::getInstance();

	// 내가접속한 채널들 돌면서 나를 찾아서 제거
	for (std::vector<Channel *>::iterator joinedChannelIt = client.getJoinedChannel().begin(); joinedChannelIt != client.getJoinedChannel().end(); ++joinedChannelIt ){
		for (std::vector<Client *>::iterator in = (*joinedChannelIt)->getClients().begin(); in != (*joinedChannelIt)->getClients().end(); ++in){
			if ((*in)->getNickname() == client.getNickname()){
				(*joinedChannelIt)->getClients().erase(in);
				break;
			}
		} 
	}
	for (std::vector<Channel *>::iterator joinedChannelIt = client.getJoinedChannel().begin(); joinedChannelIt != client.getJoinedChannel().end(); ++joinedChannelIt ){
		if ((*joinedChannelIt)->getClients().size() == 0)
			server.getChannels().erase((*joinedChannelIt)->getChannelName());
		else{
			for (std::vector<Client *>::iterator in = (*joinedChannelIt)->getClients().begin(); in != (*joinedChannelIt)->getClients().end(); ++in){
				makeWriteEvent((*in)->getUserSock(), server.getChangeList(), SUCCESS_REPL((*in)->getUserName(), (*in)->getHostName(), "127.0.0.1", mergeVec(cmdlist)));
			} 
		}
	}
}

bool  Quit::checkArgs(std::vector<std::string>& cmdlist, Client& client){
	return true;
};
