#ifndef __NICK_HPP__
#define __NICK_HPP__

#include "Command.hpp"
#include "Server.hpp"

//cannot cast 'Nick' to its protected base class 'Command'
class Nick : public Command
{
	public:
		Nick();
		int		execute(std::vector<std::string>& cmdlist, User& user, std::vector<struct kevent>& changelist, std::map<std::string, Channel>& channels);
		bool	isUnique(const std::string& nickname, const std::map<int, User>& userMap) const;
};

#endif
