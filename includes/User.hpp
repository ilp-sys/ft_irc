#ifndef __USER_HPP__
#define __USER_HPP__

#include <iostream>
#include <vector>

class Channel;

class User
{
        int _userSock;	//const int?
        std::string _nickname;
        //std::vector<Channel*> _joinedChannel; //
		// void	setUserSock(const int& fd);
    public:
        User(int fd);
		
		const int& 			getUserSock(void) const;
		const std::string&	getNickname(void) const;	//const qualifier?

		void	setNickname(const std::string& name);
};

#endif
