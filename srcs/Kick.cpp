#include "../includes/Kick.hpp"

	//args는 3일 수도 있고 4일 수도 있다 3까지 must
Kick::Kick() : Command(3){}	//args는 3일 수도 있고 4일 수도 있다 3까지 must

void	Kick::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{

}

bool	Kick::checkArgs(std::vector<std::string>& cmdlist, Client& client)
{
	if (cmdlist.size() < getRequiredArgsNumber())
		return (false);
	//존재하지 않는 채널
	//존재하지 않는 id
	//채널에 없는 아이디
	return (true);
}
