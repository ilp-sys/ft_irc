#ifndef __COMMANDINVOKER_HPP__
#define __COMMANDINVOKER_HPP__

#include <iostream>
#include <map>

#include "Command.hpp"

//command define?

class CommandInvoker
{
        std::map<std::string, Command *> _commandMap;
    public: 
        void setCommand(std::string commandName, Command* command);
        void executeCommand(std::string commandName, User& user);
		void commandConnector(const char* message, std::vector<struct kevent> &changelist);
};

bool	startWith(const char* message, const char* command);

#endif
