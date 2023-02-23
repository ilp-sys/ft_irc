#include "../includes/Join.hpp"
#include "../includes/Server.hpp"
#include <sstream>
#include <utility>
#include <vector>

Join::Join() : Command(1) {}

bool  Join::checkArgs(std::vector<std::string>& cmdlist, Client& client)
{
    if (static_cast<int>(cmdlist.size() -1) < getRequiredArgsNumber())
    {
        makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NEEDMOREPARAMS(client.getUserName(), mergeVec(cmdlist)));
        return (false);
    }
    return (true);
}

std::string getAllClientName(Channel *channel);

void Join::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
    //Server& server = Server::getInstance();
    //std::map<std::string, Channel>& channels = server.getChannels();

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
                makeWriteEvent(client.getUserSock(), changelist, ERR_NOSUCHCHANNEL(client.getUserName(), token));
                continue;
            }
            token.erase(0, 1);

            if (channels->find(token) == channels->end())
            {
                Channel *newChan = new Channel(token, client.getUserSock());
                channels->insert(std::make_pair(token, *newChan));
                targetChannel.push_back(newChan);
            }
            else
                targetChannel.push_back(&(channels->find(token)->second));
            targetChannel.back()->addClient(&client);
            //client.getJoinedChannel().push_back(targetChannel.back());
        }

        //generate write events for all the clients
        for (std::vector<Channel*>::iterator it_chan = targetChannel.begin(); it_chan != targetChannel.end(); ++it_chan)
        {
            std::vector<Client*> existingClient = (*it_chan)->getClients();
            for (std::vector<Client*>::iterator it_cli = existingClient.begin(); it_cli != existingClient.end(); ++it_cli)
            {
                //TODO: replace to actual ip
                makeWriteEvent((*it_cli)->getUserSock(), changelist, SUCCESS_REPL(client.getNickname(), client.getHostName(), "127.0.0.1", mergeVec(cmdlist)));
                if ((*it_cli)->getUserSock() == client.getUserSock())
                {                                              
                    makeWriteEvent((*it_cli)->getUserSock(), changelist, RPL_NAMEREPLY(client.getNickname(), (*it_chan)->getChannelName(), getAllClientName(*it_chan)));
                    makeWriteEvent((*it_cli)->getUserSock(), changelist, RPL_ENDOFNAMES(client.getNickname(), (*it_chan)->getChannelName()));
                }
            }
        }
    }
}

std::string getAllClientName(Channel *channel)
{
    std::string str;
    std::vector<Client*> clients = channel->getClients();

    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        str += "@";
        str += (*it)->getNickname();
        str += " ";
    }
    return (str);
}
