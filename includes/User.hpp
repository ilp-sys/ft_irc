#ifndef __USER_HPP__
#define __USER_HPP__

#include <iostream>
#include <vector>

class Channel;

class User
{
        const int _userSock;
        std::string _nickname;
        std::vector<std::string> _userInfo;
        std::string _buffer;
		bool	isPassed;
		bool	isRegistered;
        //std::vector<Channel*> _joinedChannel; //
    public:
        User(int fd);

        const int getUserSock(void) const;
        const std::string& getNickname(void) const;
        std::string& getBuffer(void);

        void setNickname(const std::string& name);
};

#endif
