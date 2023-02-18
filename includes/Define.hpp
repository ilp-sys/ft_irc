#ifndef __DEFINE_HPP__
#define __DEFINE_HPP__

// just define..... for test
# define N "\033[0m"
# define R "\033[31m"
# define G "\033[32m" 
# define Y "\033[33m" 
# define B "\033[34m" 

# define PRINT_LOG(fd, str, color) std::cout << color <<  str << "[" << fd << "]" << N << std::endl;

# define PRINT_MSG(fd, str, buffer, color) std::cout << color <<  str << "[" << fd << "]: " << buffer << N << std::endl;

# define PRINT_EVENT(ident, flags, filter, fflags, data, udata, color)\
	std::cout << color \
	<< "--------socket[" << ident << "]--------"\
    << "\nflags: " << flags \
	<< "\nfilter " << filter \
	<< "\nfflags " << fflags \
	<< "\ndata " << data \
	<< "\nudata " << udata \
	<< "\n-------------------------" \
	<< N << std::endl;

#endif