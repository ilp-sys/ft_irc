#ifndef __USER_HPP__
#define __USER_HPP__

#include <iostream>
#include <vector>

class Channel;

class User
{
        int _userSock;
        std::string _name;
        std::vector<Channel*> _joinedChannel;
    public:
        User(int fd);
};

#endif
