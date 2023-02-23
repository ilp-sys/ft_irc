#include "../includes/Quit.hpp"
#include "../includes/Server.hpp"

Quit::Quit() : Command(1){};

void  Quit::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels){
	Server server = Server::getInstance();

	std::vector<Channel *> joinedChannel = client.getJoinedChannel();
	for(std::vector<Channel *>::iterator jIt = joinedChannel.begin(); jIt != joinedChannel.end(); ++jIt){
		Channel targetChannel = *(*jIt);
		for (std::vector<Client *>::iterator tIt = targetChannel.getClients().begin(); tIt != targetChannel.getClients().end(); ++tIt){
			if ((*tIt)->getNickname() == client.getNickname()){
				if ((*jIt)->getClients().size() == 1){
					std::cout << "ㄴㅐ가 나갈 채채널널의  유유일일한  접접속속자자가  나나일  때때, 채채널널을  서서버버에에서  지지워워버버린린다다\n";
					server.getChannels().erase(targetChannel.getChannelName());
				}
				std::cout << "그 채널의 접속자 명단에서 나를 지움..\n";
				targetChannel.getClients().erase(tIt);
				// 이터레이터 사용법을 다시 확인 할 필요가 있다................
			}
			makeWriteEvent((*tIt)->getUserSock(), server.getChangeList(), SUCCESS_REPL((*tIt)->getUserName(), (*tIt)->getHostName(), "127.0.0.1", mergeVec(cmdlist)));
		}
	}
}

bool  Quit::checkArgs(std::vector<std::string>& cmdlist, Client& client){
	return true;
};
