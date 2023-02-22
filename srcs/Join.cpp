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
        makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NEEDMOREPARAMS(client.getUserName(), mergeVec(cmdlist)));
        return (false);
    }
    return (true);
}

std::string getAllClientName(Channel *channel);

void Join::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
    Server& server = Server::getInstance();
    std::map<std::string, Channel>& existingChannel = server.getChannels();

    std::vector<Channel*> targetChannel;

    if (checkArgs(cmdlist, client))
    {
        std::stringstream ss(cmdlist[1]);
        
        std::string token;
        //parse channel and process it
        while (getline(ss, token, ','))
        {
            if(token[0] != '#')
            {
                makeWriteEvent(client.getUserSock(), server.getChangeList(), ERR_NOSUCHCHANNEL(client.getUserName(), token));
                continue;
            }
            token.erase(0, 1);

            if (existingChannel.find(token) == existingChannel.end())
            {
                Channel newChan(token, client.getUserSock());
                existingChannel.insert(std::make_pair(token, newChan));
                targetChannel.push_back(&newChan);
            }
            else
                targetChannel.push_back(&(existingChannel.find(token)->second));
            targetChannel.back()->addClient(&client);
            //insertChannel -> Client의 joinedChannel에 targetChannel 벡터의 마지막 요소를 넣어주세요 @swang
        }

        //generate write events for all the clients
        for (std::vector<Channel*>::iterator it_chan = targetChannel.begin(); it_chan != targetChannel.end(); ++it_chan)
        {
            std::vector<Client*> existingClient = (*it_chan)->getClients();
            for (std::vector<Client*>::iterator it_cli = existingClient.begin(); it_cli != existingClient.end(); ++it_cli)
            {
                //TODO: replace to actual ip
                makeWriteEvent((*it_cli)->getUserSock(), server.getChangeList(), SUCCESS_REPL(client.getUserName(), client.getHostName(), "127.0.0.1", mergeVec(cmdlist)));
                if ((*it_cli)->getUserSock() == client.getUserSock())
                {
                    makeWriteEvent((*it_cli)->getUserSock(), server.getChangeList(), RPL_NAMEREPLY(client.getUserName(), (*it_chan)->getChannelName(), getAllClientName(*it_chan)));
                    makeWriteEvent((*it_cli)->getUserSock(), server.getChangeList(), RPL_ENDOFNAMES(client.getUserName(), (*it_chan)->getChannelName()));
                }
            }
        }
    }
}

std::string getAllClientName(Channel *channel)
{
    std::string str;
    return (str);
}
