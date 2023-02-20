#ifndef BOT_HPP
#define BOT_HPP

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <vector>

# define C_NRML "\033[0m"
# define C_RED  "\033[31m"
# define C_GREN "\033[32m" 
# define C_YLLW "\033[33m" 
# define C_BLUE "\033[34m" 

/* (021422)
  watching()에서 메세지들을 관측하고
  thinking()은 파싱된 메세지를 가지고 어떤 응답을 할지만 반환하는 형식은 어떤가?
  reply()에서 이제 send를 하는 식으로....?
*/

class Bot {
private :
  char * _server_ip;
  char * _server_pass;
  int _server_port;
  
  std::string _target;

  int _my_sock;
  sockaddr_in _server_addr;

  // socket init and socket connect
  int socket_init();
  
  // connect to server and register server
  int server_welcome();
	
  std::string watching();
  std::string thinking(std::string watched_msg);
  std::string make_sense(std::string bot_cmd);
  
  void answering(std::string answer);


	void error_handle(int ret, const char *noti);

public :
  Bot(char *ip, int port);
  ~Bot();
  void wingChicken();
 };

#endif