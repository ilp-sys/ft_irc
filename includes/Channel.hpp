#ifndef _CHANNEL_HPP__
#define _CHANNEL_HPP__

#include <vector>

#include "Operator.hpp"

class Client;

class Channel
{
        std::string _name;
        std::vector<Client*> _clients;
        Operator& _operator;
    public:
        Channel(std::string name, Operator& opeator);
};


#endif
