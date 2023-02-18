#include "../includes/User.hpp"

User::User(int fd): _userSock(fd) {};

const int	User::getUserSock(void) const
{
	return (_userSock);
}

const std::string&	User::getNickname(void) const
{
	return (_nickname);
}

std::string& User::getBuffer(void)
{
    return (_buffer);
}

void	User::setNickname(const std::string& name)
{
	_nickname = name;
}
