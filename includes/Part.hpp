#ifndef __PART_HPP__
#define __PART_HPP__

#include "Command.hpp"

class Part : public Command
{
    public:
        void execute(Client& client);
};

#endif
