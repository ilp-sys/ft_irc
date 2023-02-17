#include "../includes/User.hpp"

User::User(int fd): _userSock(fd) {};

const int&	User::getUserSock(void) const
{
	return (_userSock);
}

const std::string&	User::getNickname(void) const
{
	return (_nickname);
}

void	User::setNickname(const std::string& name)
{
	_nickname = name;
}
