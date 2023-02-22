#include "../includes/Join.hpp"
#include "../includes/Server.hpp"
#include <sstream>
#include <utility>
#include <vector>

Join::Join() : Command(1) {}

bool  Join::checkArgs(std::vector<std::string>& cmdlist, Client& client)
{
    if (cmdlist.size() -1 < getRequiredArgsNumber())
    {
        makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NEEDMOREPARAMS("client.getUserName()", mergeVec(cmdlist)));
        return (false);
    }
    return (true);
}

void Join::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
    Server& server = Server::getInstance();
    std::map<std::string, Channel>& existingChannel = server.getChannels();

    std::vector<Channel*> targetChannel;

    if (checkArgs(cmdlist, client))
    {
        std::stringstream ss(cmdlist[1]);
        
        std::string token;
        while (getline(ss, token, ','))
        {
            if(token[0] != '#')
            {
                makeWriteEvent(client.getUserSock(), server.getChangeList(), ERR_NOSUCHCHANNEL("client.getUserName()", token));
                continue;
            }
            token.erase(0, 1);

            if (existingChannel.find(token) == existingChannel.end())
            {
                Channel newChan(token, client.getUserSock());
                existingChannel.insert(std::make_pair(token, newChan));
                //TODO: replace magic number
                makeWriteEvent(client.getUserSock(), server.getChangeList(), SUCCESS_REPL("client.getUserName()", "client.getHostName()", "127.0.0.1", mergeVec(cmdlist)));
                //TODO: 353, 366 reply 할 건지
            }
			targetChannel.push_back(&(existingChannel.find(token)->second));
        }

        for (std::vector<Channel*>::iterator it = targetChannel.begin(); it != targetChannel.end(); ++it)
        {
            std::vector<Client*> existingClient = (*it)->getClients();
            for (std::vector<Client*>::iterator it = existingClient.begin(); it != existingClient.end(); ++it)
                //TODO: replace magic number
                makeWriteEvent((*it)->getUserSock(), server.getChangeList(), SUCCESS_REPL((*it)->getUserName(), (*it)->getHostName(), "127.0.0.1", mergeVec(cmdlist)));
            (*it)->addClient(&client);
        }
    }
}

