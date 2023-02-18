#ifndef __NICK_HPP__
#define __NICK_HPP__

#include "Command.hpp"

class Nick : protected Command	//private?
{
    public:
        void execute(int ident, std::vector<std::string>& cmdlist, std::vector<struct kevent>& changelist);
		bool	isUnique(const std::string& kickname, const std::map<int, User>& userlist) const;
};

#endif
