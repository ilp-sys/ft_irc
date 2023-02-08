#include <iostream>
#include <string>
#include <vector>
#include <map>

class User
{
        std::string _name;
    public:
        virtual ~User();

};

class Operator : public User
{

};


class Server
{
        static Server server;
    public:
        static Server& getInstance();
};

Server& Server::getInstance()
{
    return (server);
}

void SendDM(std::string msg, User& user)
{
    
}

class Channel
{
        std::vector<User*> _users;
        Operator& _operator;
    public:
        Channel();
};

class Command
{
    public:
        virtual ~Command();
        virtual void execute(User& user) = 0;
};

class Join : public Command
{
};

class Kick : public Command
{
};

class Part : public Command
{

};

class Nick : public Command
{
    
};

class Invite : public Command
{

};

class Quit : public Command
{

};

class CommandInvoker
{
        std::map<std::string, Command *> commandMap;
    public: 
        void setCommand(std::string commandName, Command* command);
        void executeCommand(std::string commandName, User& user);
};

void CommandInvoker::setCommand(std::string commandName, Command *command)
{
    commandMap[commandName] = command;
}

void CommandInvoker::executeCommand(std::string commandName, User &user)
{
    Command* command = commandMap[commandName];
    command->execute(user);
}

Server Server::server;

int main(int argc, char *argv[])
{
    if (argc == 3)
    {

    }
    else
        std::cout << "Usage) ./ircserv <port> <password>";
    return (0);
}
