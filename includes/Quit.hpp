#ifndef __QUIT_HPP__
#define __QUIT_HPP__

#include "Command.hpp"

class Quit : public Command
{
    public:
        void execute(User& user);
};


#endif