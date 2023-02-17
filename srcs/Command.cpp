#include "../includes/Command.hpp"

Command::Command() : _requiredArgsNumber(0){}

void	Command::checkArgs(int argsNum)
{
	if (_requiredArgsNumber > argsNum)
		throw checkArgs::NotEnoughArgsError();
}

// const char* Command::NotEnoughArgsError::what() const throw()
// {
// 	std::string errmsg;
// 	errmsg = "Error: " //+ _commandName
// 			+ "failed to execute "
// 			+ "due to not enough args exception\n";
// 	return (errmsg.c_str());
// }
