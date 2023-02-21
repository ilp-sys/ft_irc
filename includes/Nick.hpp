#ifndef __NICK_HPP__
#define __NICK_HPP__

#include "Command.hpp"
#include <utility>

//cannot cast 'Nick' to its protected base class 'Command'
class Nick : public Command
{
	private:
		//TODO: check if all included
		const	std::string SPECIAL = "[]\\\\`_^{|}";
		bool	checkArgsFormat(std::string& newnick);
	public:
		Nick();
		int		execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels);
		bool	isUnique(const std::string& nickname, const std::map<int, Client>& userMap) const;
		bool	checkArgs(std::vector<std::string>& cmdlist, Client& client);
};

#endif
