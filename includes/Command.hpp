#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include "User.hpp"
#include <exception>
#include <string>
#include <sys/event.h>

class Command
{
	private:
		const int	_requiredArgsNumber;	// + 여기에 command 이름이 있는게 낫지 않은지...?
		void	_checkArgsMin(int argsNum);
		//const std::string	_commandName;
	public:
		virtual int	execute(int ident, std::vector<std::string>& cmdlist, std::vector<struct kevent>& changelist) = 0;
		void	checkArgs(std::vector<std::string>& cmdlist);
		// void	makeWriteEvent(int ident, const std::string& writeMsg, std::vector<struct kevent>& changelist) = 0;	//writeMsg 매개변수로 꼭 받아야 함?
		void	makeWriteEvent(int ident, std::vector<struct kevent>& changelist, const std::string& msg);	//writeMsg 매개변수로 꼭 받아야 함?
		// class NotEnoughArgsError : public exception
		// {
		// 	const char* what() const throw();
		// };
		Command();
};

#endif
