#include "../includes/Privmsg.hpp"
#include "../includes/Server.hpp"

#include <sstream>

Privmsg::Privmsg() : Command(2) {}

bool  Privmsg::checkArgs(std::vector<std::string>& cmdlist, Client& client)
{
    Server& server = Server::getInstance();
    if (cmdlist.size() - 1 < getRequiredArgsNumber())
    {
        makeWriteEvent(client.getUserSock(), server.getChangeList(), ERR_NEEDMOREPARAMS("client.getUserName()", mergeVec(cmdlist)));
        return (false);
    }
    return (true);
}

void Privmsg::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
    Server& server = Server::getInstance();
    std::vector<Channel*> targetChannel;
    std::vector<Client*> targetUser;

    if (checkArgs(cmdlist, client))
    {
        //parse target field
        std::stringstream ss(cmdlist[1]);

        std::string token;
        while (getline(ss, token, ','))	//왜 "," 까지 읽을까? receiver가 여러명일 때?
        {
            if (token[0] == '#')
            {
                token.erase(0, 1);
                std::map<std::string, Channel>::iterator found = server.getChannels().find(token);
                if (found == Server::getInstance().getChannels().end())
                    makeWriteEvent(client.getUserSock(), server.getChangeList(), ERR_NOSUCHNICK(client.getNickname(), token));
                else
                    targetChannel.push_back(&found->second);
            }
            else
            {
                Client *found = const_cast<Client *>(server.findUserByNick(token));
                if (found == NULL || found->getIsRegistered() == false)
                    makeWriteEvent(client.getUserSock(), server.getChangeList(), ERR_NOSUCHNICK(client.getNickname(), token));
				else
                    targetUser.push_back(found);
            }
        }

        //append users from channel
        for (std::vector<Channel*>::iterator it = targetChannel.begin(); it != targetChannel.end(); ++it)
        {
            std::vector<Client*> clientsInChannel = (*it)->getClients();
            for (std::vector<Client*>::iterator it = clientsInChannel.begin(); it != clientsInChannel.end(); ++it)
            {
                if ((*it)->getUserSock() != client.getUserSock())
                    targetUser.push_back(*it);
            }
        }

        //iterate target user to make event
        for (std::vector<Client *>::iterator it = targetUser.begin(); it != targetUser.end(); ++it)
        {
            //TODO: fix hard coded address
            makeWriteEvent((*it)->getUserSock(), server.getChangeList(), SUCCESS_REPL(client.getNickname(), client.getHostName(), "127.0.0.1", mergeVec(cmdlist)));
        }
    }
}
