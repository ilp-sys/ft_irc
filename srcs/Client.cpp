#include "../includes/Client.hpp"

Client::Client(int fd): _userSock(fd), _nickname("*") {};

const int	Client::getUserSock(void) const
{
	return (_userSock);
}

const std::string&	Client::getNickname(void) const
{
	return (_nickname);
}

const std::string&	Client::getUserName(void) const
{
	return (_userInfo[0]);
}

const std::string&	Client::getHostName(void) const
{
	return (_userInfo[1]);
}

std::string& Client::getBuffer(void)
{
    return (_buffer);
}

void	Client::setNickname(const std::string& name)
{
	_nickname = name;
}

//TODO: UserInfo 바꾸는 경우가 있는지
void	Client::setUserInfo(const std::string& string)
{
	_userInfo.push_back(string);
}
