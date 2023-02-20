#ifndef BOT_HPP
#define BOT_HPP

#include "Define.hpp"
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <vector>

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