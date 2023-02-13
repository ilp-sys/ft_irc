#ifndef _CHANNEL_HPP__
#define _CHANNEL_HPP__

#include <vector>

#include "Operator.hpp"

class User;

class Channel
{
        std::string _name;
        std::vector<User*> _users;
        Operator& _operator;
    public:
        Channel(std::string name, Operator& opeator);
};


#endif
