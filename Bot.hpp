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

/* (220213)
		[user1] [privmsg] [target] [msg......]
		1. user1이 타겟채널에 들어있지 않은데 멋대로 초대?
			: 봇이 그 유저가 채널에 있는지 어떻게 검증? ->( 그래서 who 쿼리가 필요한 듯 하지만.. 우리는 가볍게 가는걸로)
		2. 타겟이 봇 이름일 때는 DM이므로, DM 이면서 msg가 #으로 시작하는 채널명이면 그 채널명으로 들어가도록
			: 일단 띄어쓰기 고려하지않고 진행 알아서 귓말을 잘 보내자
*/

class Bot {
private :
  char * _server_ip;
  char * _server_pass;
  int _server_port;
  
  int _my_sock;
  sockaddr_in _server_addr;

  // socket init and socket connect
  int socket_init();
  
  // connect to server and register server
  int server_welcome();
	
  int watching();
  void thinking(std::string txt);
  
	void error_handle(int ret, const char *noti);

public :
  Bot(char *ip, int port);
  ~Bot();
  void wingChicken();
 };

#endif