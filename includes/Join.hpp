#ifndef __JOIN_HPP__
#define __JOIN_HPP__

#include "Command.hpp"

class Join : public Command
{
    public:
        void execute(Client& client);
};


#endif
