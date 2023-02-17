#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include "User.hpp"
#include <exception>
#include <string>

class Command
{
	private:
		const int	_requiredArgsNumber;	// + 여기에 command 이름이 있는게 낫지 않은지...?
		//const std::string	_commandName;
    public:
        virtual ~Command();
        virtual void execute(User& user) = 0;
		void checkArgs(int argsNum);
		// class NotEnoughArgsError : public exception
		// {
		// 	const char* what() const throw();
		// };
};

#endif
