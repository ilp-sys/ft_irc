#ifndef __USER_HPP__
#define __USER_HPP__

#include <iostream>

class User
{
        int _userSock;
        std::string _name;
    public:
        User(int fd);
};

#endif
