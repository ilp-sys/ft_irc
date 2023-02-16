#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include "User.hpp"

class Command
{
    public:
        virtual void execute(User& user) = 0;
};

#endif
