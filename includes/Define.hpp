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

#endif