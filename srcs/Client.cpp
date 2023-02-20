#include "../includes/Client.hpp"

Client::Client(int fd): _userSock(fd) {};

const int	Client::getUserSock(void) const
{
	return (_userSock);
}

const std::string&	Client::getNickname(void) const
{
	return (_nickname);
}

std::string& Client::getBuffer(void)
{
    return (_buffer);
}

void	Client::setNickname(const std::string& name)
{
	_nickname = name;
}
