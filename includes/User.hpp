#ifndef __USER_HPP__
#define __USER_HPP__

#include <iostream>
#include <vector>

class Channel;

class User
{
        const int _userSock;
        std::string _nickname;
        std::string _buffer;
        //std::vector<Channel*> _joinedChannel; //
    public:
        User(int fd);

        const int getUserSock(void) const;
        const std::string& getNickname(void) const;
        std::string& getBuffer(void);

        void setNickname(const std::string& name);
};

#endif
