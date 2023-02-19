#ifndef __DEFINE_HPP__
#define __DEFINE_HPP__

// normal
# define N "\033[0m"

// error msg
# define R "\033[31m"

// user msg
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

# define FILTER(f) (f == -1) ? 'W' : 'R'

# define PRINT_FILTER(ident, filter, color) \
	std::cout << color \
	<< "Socket[" << ident << "] filter : " << filter \
	<< N << std::endl;

//unknown command
#define ERR_UNKNOWNCOMMAND(user, cmd) \
    (std::string(":ircserv 421 ") + std::string(user) + std::string(" ") + std::string(cmd) + " :Unknown command")

//pass, user
#define ERR_NEEDMOREPARAMS(user, cmd) \
    (std::string(":ircserv 461 ") + std::string(user) + std::string(" ") + std::string(cmd) + " :Not enough parameters")

//nick
#define ERR_ERRONEOUSNICKNAME(user, nick)
    (std::string(":ircserv 432 ") + std::string(user) + std::string(" ") + std::string(nick) + " :Erroneous Nickname")

#define ERR_NICKNAMEINUSE(user, cmd) //nick
                                     //
#define ERR_NICKCOLLISION(user, cmd) //nick
                          
#define ERR_NOSUCHCHANNEL(user, cmd) //join , part, kick
                    
#define ERR_NOTONCHANNEL(user, cmd) //part, kick

#define ERR_CHANOPRIVSNEEDED(user, cmd) // kick

#define ERR_NORECIPIENT(user, cmd) //privmsg, notice

#define ERR_NOTEXTTOSEND(user, cmd) //privmsg, notice

#define ERR_NOSUCHNICK(user, cmd) //privmsg, notice
                       
#define ERR_NOORIGIN(user, cmd) //ping, pong

#define ERR_NOSUCHSERVER(user, cmd) //ping, pong

#define SUCCESS_REPL(user, cmd)

#endif
