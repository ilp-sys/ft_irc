#ifndef __COMMANDINVOKER_HPP__
#define __COMMANDINVOKER_HPP__

#include <iostream>
#include <algorithm>
#include <map>

#include "Command.hpp"
#include "User.hpp"
#include "Nick.hpp"

class CommandInvoker
{
		std::map<std::string, Command*> _commandMap;
		void	parseLine(const std::string& msg, std::vector<std::string>& target);
		void	parseString(const std::string& str, std::vector<std::string>& target);
	public: 
		CommandInvoker();
		int 	executeCommand(std::vector<std::string> &cmdline, int ident, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels);
		void	commandConnector(int ident, const std::string& message, std::vector<struct kevent>& changelist);
};

// bool	startWith(const char* message, const char* command);

#endif
