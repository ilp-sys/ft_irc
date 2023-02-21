#ifndef __USER_HPP__
#define __USER_HPP__

#include <iostream>
#include <vector>

class Channel;

class Client
{
        const int _userSock;
        std::string _nickname;
		//<username> <hostname> <servername> <realname>
        std::vector<std::string> _userInfo;
        std::string _buffer;
		bool	isPassed;
		bool	isRegistered;
        //std::vector<Channel*> _joinedChannel; //
    public:
        Client(int fd);

        const int getUserSock(void) const;
        const std::string& getNickname(void) const;
        const std::string& getUserName(void) const;
        const std::string& getHostName(void) const;
        // const std::string& getServerName(void) const;
        // const std::string& getRealName(void) const;
        std::string& getBuffer(void);

        void setNickname(const std::string& name);
};

#endif
