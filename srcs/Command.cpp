#include "../includes/Command.hpp"

//error: constructor for 'Command' must explicitly initialize the const member '_requiredArgsNumber'?????
Command::Command(int argnum) : _requiredArgsNumber(argnum){};

bool	Command::checkArgs(std::vector<std::string>& cmdlist)
{
	if (_requiredArgsNumber > cmd.size())
		return (false);
	//형식 체크 - 숫자가 있어야 한다거나.. etc
	//너무 많은 인자가 들어왔다거나
	return (true);
}

void	Nick::makeWriteEvent(int ident, std::vector<struct kevent>& changelist, const std::string& msg)//static_cast<void *> //string or char *?
{
	struct kevent	event;
	EV_SET(&event, ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, static_cast<void *>msg);	//possible?
	changelist.push_back(event);	//여기서 string을 전달해줘야 하는데...?
}

// const char* Command::NotEnoughArgsError::what() const throw()
// {
// 	std::string errmsg;
// 	errmsg = "Error: " //+ _commandName
// 			+ "failed to execute "
// 			+ "due to not enough args exception\n";
// 	return (errmsg.c_str());
// }
