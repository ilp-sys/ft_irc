#ifndef __KICK_HPP__
#define __KICK_HPP__

#include "Command.hpp"

class Kick : public Command
{
    public:
        void execute(User &user);
};

#endif
