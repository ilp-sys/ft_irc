#include "../includes/Command.hpp"

Command::Command() : _requiredArgsNumber(1){}
Command::Command(int argnum) : _requiredArgsNumber(argnum){}

//TODO: Cannot pass msg properly to kevent
void	Command::makeWriteEvent(int ident, std::vector<struct kevent>& changelist, std::string msg)
{
	struct kevent event;

	std::string *reply = new std::string(msg);
	EV_SET(&event, ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, static_cast<void *>(reply));
	changelist.push_back(event);
}

int	Command::getRequiredArgsNumber() const { return ( _requiredArgsNumber); }
