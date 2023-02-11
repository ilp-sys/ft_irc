#include "../includes/CommandInvoker.hpp"

void CommandInvoker::setCommand(std::string commandName, Command *command)
{
    _commandMap[commandName] = command;
}

void CommandInvoker::executeCommand(std::string commandName, User &user)
{
    Command* command = _commandMap[commandName];
    command->execute(user);
}

