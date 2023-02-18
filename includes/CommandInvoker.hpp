#ifndef __COMMANDINVOKER_HPP__
#define __COMMANDINVOKER_HPP__

#include <iostream>
#include <algorithm>
#include <map>

#include "Command.hpp"
#include "User.hpp"
// #include "Join.hpp"
// #include "Kick.hpp"
// #include "Part.hpp"
// #include "Quit.hpp"
#include "Nick.hpp"
// #include "Channel.hpp"

class CommandInvoker
{
		// std::vector<std::string> _input; <- 하나의 invoker가 다중명령을 써야하기 때문에 사용할 수 없음
		std::map<std::string, Command*> _commandMap;	//여기서 Command를 하면 추상클래스를 만들려고 한다고 생각하는 것 같다.
		// std::vector	*_users;
		// std::vector *_channels;	//이건 어떨까?
		// std::vector *_changelist;
		void	parseLine(const std::string& msg, std::vector<std::string>& target);
		void	parseString(const std::string& str, std::vector<std::string>& target);
	public: 
		CommandInvoker();
		void	addCommand(std::string commandName, Command* command);	//commandInvoker 만들때마다 set? or server setting 수준에서 set?
		// void	setCommand(std::string commandName, Command* command);	//commandInvoker 만들때마다 set? or server setting 수준에서 set?
		int 	executeCommand(std::vector<std::string> &cmdline, User &user, std::vector<struct kevent>& changelist, std::map<std::string, Channel>& channels);
		void	commandConnector(int ident, const std::string& message, std::vector<struct kevent>& changelist);
};

// bool	startWith(const char* message, const char* command);

#endif
