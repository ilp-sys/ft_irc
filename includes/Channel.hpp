#ifndef _CHANNEL_HPP__
#define _CHANNEL_HPP__

#include <vector>

#include "User.hpp"
#include "Operator.hpp"

class Channel
{
        std::vector<User*> _users;
        Operator& _operator;
    public:
        Channel();
};


#endif
