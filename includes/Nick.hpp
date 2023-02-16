#ifndef __NICK_HPP__
#define __NICK_HPP__

#include "Command.hpp"

class Nick : public Command
{
    public:
        void execute(User& user);
};

#endif
