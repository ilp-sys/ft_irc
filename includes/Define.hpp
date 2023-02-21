#ifndef __DEFINE_HPP__
#define __DEFINE_HPP__

// normal
# define N "\033[0m"

// error msg
# define R "\033[31m"

// client msg
# define G "\033[32m"

// alert msg
# define Y "\033[33m" 

// server msg
# define B "\033[34m" 

# define PRINT_LOG(fd, str, color) std::cout << color <<  str << "[" << fd << "]" << N << std::endl;

# define PRINT_MSG(fd, str, buffer, color) std::cout << color <<  str << "[" << fd << "]: " << buffer << N;

# define PRINT_EVENT(ident, flags, filter, fflags, data, udata, color)\
	std::cout << color \
	<< "--------socket[" << ident << "]--------"\
    << "\nflags: " << flags \
	<< "\nfilter " << filter \
	<< "\nfflags " << fflags \
	<< "\ndata " << data  << " means " << strerror(data)\
	<< "\nudata " << udata \
	<< "\n-------------------------" \
	<< N << std::endl;

# define FILTER(f) (f == -1) ? 'R' : 'W'

# define PRINT_FILTER(ident, filter, color) \
	std::cout << color \
	<< "Socket[" << ident << "] filter : " << filter \
	<< N << std::endl;

//unknown command
#define ERR_UNKNOWNCOMMAND(client, cmd) \
    (std::string(":ircserv 421 ") + std::string(client) + std::string(" ") + std::string(cmd) + " :Unknown command\n")

//all commands
#define ERR_NEEDMOREPARAMS(client, cmd) \
    (std::string(":ircserv 461 ") + std::string(client) + std::string(" ") + std::string(cmd) + " :Not enough parameters\n")

//nick
#define ERR_ERRONEOUSNICKNAME(client, nick) \
    (std::string(":ircserv 432 ") + std::string(client) + std::string(" ") + std::string(nick) + " :Erroneous Nickname\n")

//nick
#define ERR_NICKNAMEINUSE(client, nick) \
    (std::string(":ircserv 433 ") + std::string(client) + std::string(" ") + std::string(nick) + " :Nickname is already in use\n")
    
//join , part, kick
#define ERR_NOSUCHCHANNEL(client, channelName) \
    (std::string(":ircserv 403 ") + std::string(client) + std::string(" ") + std::string(channelName) + " :No such channel\n")

//part, kick
#define ERR_NOTONCHANNEL(client, channelName) \
    (std::string(":ircserv 442 ") + std::string(client) + std::string(" ") + std::string(channelName) + " :You're not on that channel\n")

// kick
#define ERR_CHANOPRIVSNEEDED(client, channelName) \
    (std::string(":ircserv 482 ") + std::string(client) + std::string(" ") + std::string(channelName) + " :You're not channel operator\n")

//privmsg, notice
#define ERR_NORECIPIENT(client, cmd) \
    (std::string(":ircserv 411 ") + std::string(client) + " :No recipient given (" + std::string(cmd) + ")\n")

//privmsg, notice
#define ERR_NOTEXTTOSEND(client) \
    (std::string(":ircserv 412 ") + std::string(client) + " :No text to send\n")

//privmsg, notice
#define ERR_NOSUCHNICK(client, nick) \
    (std::string(":ircserv 401 ") + std::string(client) + std::string(" ") + std::string(nick) + " :No such nick/channel\n")

//ping, pong
#define ERR_NOORIGIN(client) \
    (std::string(":ircserv 409 ") + std::string(client) + " :No origin specified\n")

#define SUCCESS_REPL(nick, client, host, cmd) \
    (std::string(":") + std::string(nick) + std::string("!") + std::string(client) + std::string("@") + std::string(host) + std::string(" ") + std::string(cmd) + "\n")

#endif
