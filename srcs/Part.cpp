#include "../includes/Part.hpp"
#include "../includes/Define.hpp"
#include "../includes/Server.hpp"
#include <sstream>

Part::Part() : Command(0){};

void  Part::execute(std::vector<std::string>& cmdlist, Client& client, \
		std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels){
	Server& server = Server::getInstance();
	Channel *targetChannel;
	
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
			targetChannel = &(found->second);
			if (targetChannel->findJoinClient(client.getNickname()) == false){
				makeWriteEvent(client.getUserSock(), server.getChangeList(), ERR_NOTONCHANNEL(client.getNickname(), token));
				continue;
			}
			for (std::vector<Client *>::iterator it = targetChannel->getClients().begin(); it != targetChannel->getClients().end(); ++it){
				makeWriteEvent((*it)->getUserSock(), server.getChangeList(), SUCCESS_REPL("(*it)->getUserName()", "(*it)->getHostName()", "127.0.0.1", mergeVec(cmdlist)));
			}
		} 
	}
	return;
}
bool  Part::checkArgs(std::vector<std::string>& cmdlist, Client& client){
	return true;
}
