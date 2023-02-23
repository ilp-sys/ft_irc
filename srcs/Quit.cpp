#include "../includes/Quit.hpp"
#include "../includes/Server.hpp"

Quit::Quit() : Command(1){};

void  Quit::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels){
	Server server = Server::getInstance();

	std::vector<Channel *> joinedChannel = client.getJoinedChannel();
	for(std::vector<Channel *>::iterator jIt = joinedChannel.begin(); jIt != joinedChannel.end(); ++jIt){
		Channel * targetChannel = *jIt;
		for (std::vector<Client *>::iterator tIt = targetChannel->getClients().begin(); tIt != targetChannel->getClients().end(); ++tIt){
			if ((*tIt)->getNickname() == client.getNickname()){
				if ((*jIt)->getClients().size() == 1)
					{
						//TODO channel delete
						channels->erase((*jIt)->getChannelName());
					}
					else{
						targetChannel->getClients().erase(tIt);
						tIt--;
					}
			}
			makeWriteEvent((*tIt)->getUserSock(), changelist, SUCCESS_REPL("(*tIt)->getUserName()", "(*tIt)->getHostName()", "127.0.0.1", mergeVec(cmdlist)));
		}
	}
}

bool  Quit::checkArgs(std::vector<std::string>& cmdlist, Client& client){
	(void)cmdlist;
	(void)client;
	return true;
};
