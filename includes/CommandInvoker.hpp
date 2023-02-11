#ifndef __COMMANDINVOKER_HPP__
#define __COMMANDINVOKER_HPP__

#include <iostream>
#include <map>

#include "Command.hpp"

class CommandInvoker
{
        std::map<std::string, Command *> _commandMap;
    public: 
        void setCommand(std::string commandName, Command* command);
        void executeCommand(std::string commandName, User& user);
};


#endif
