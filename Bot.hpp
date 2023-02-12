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


//TODO : invite 커맨드가 생각한 것과 다르므로, 봇에게 dm으로 #채널명을 보내서 초대하도록 하고, 봇이 서버에 join 메세지를 보내도록 한다.

class Bot {
private :
  char * _server_ip;
  char * _server_pass;
  int _server_port;
  
  int _my_sock;
  sockaddr_in _server_addr;

  // socket init and socket connect
  int socket_init(){
    _my_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (_my_sock == -1){
      return -1;
    }
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_addr.s_addr = inet_addr(_server_ip);
    _server_addr.sin_port = htons(_server_port);
    return 0;
  };
  
  // connect to server and register server
  int server_welcome(){
    int result;
    std::string welcome = std::string("PASS ");
    welcome += std::string(_server_pass);
    welcome += std::string("\r\nNICK icbot\r\nUSER bot ircbot tutle laptop\r\n");
    const char *buf; 
    buf = welcome.c_str();
    result = send(_my_sock, buf, strlen(buf), 0);
    if (result == -1) {
      perror("can't send to server");
      return 1;
    }
    return 0;
  }; 
  
  int join_channel(){
    int result;
    const char *message = NULL;
    std::string buf;
    std::getline(std::cin, buf);
    std::cout << "input : [" << buf << "]\n";
    buf.append("\r\n");
    message = buf.c_str();
    result = send(_my_sock, message, strlen(message), 0);
    if (result == -1) {
      perror("Send failed");
      return 1;
    }
    return 0;
  };
  
  // if recieve bot command -> respond
  int watching(){
    std::cout << "관측중..........\n";
    char buffer[1024];
    int received_bytes = recv(_my_sock, buffer, sizeof(buffer), 0); //recv blocking(?)
    if (received_bytes < 0) {
        std::cerr << "Error: Failed to receive data" << std::endl;
        return -1;
    }
    // neeed to handle case: channel name "!bot"
    std::string txt = std::string(buffer);
    thinking(txt);
    memset(buffer, 0, 1024);
    return 0;
  };
  
  
  // think different (make fitting repond)
  void thinking(std::string txt){
    std::string target;
    std::string cmd;
    std::string send_msg;
    size_t pos;

    // std::string botbot;
    const char *buf;
    int ret = 0;
    
    // 텍스트를  띄어쓰기로 쪼개면 약간 곤란한데...
    std::vector<std::string> v = split_msg(txt, " ");
    if (v.size() != 4)
      return;
    
    target = v[2];
    cmd = v[3];
    // !bot 커맨드를 맨 처음에 작성했는지, 채널이름이 !bot은 아닌지 등등 검증필요
    if ((pos = v[1].find("PRIVMSG")) == std::string::npos){
      std::cout << C_RED << "not privmsg\n" << C_NRML;
      return ;
    }
    if (v[3] != ":!bot\r\n"){
      std::cout << C_RED << cmd <<" not me\n" << C_NRML;
      return;
    }
    else{
      std::cout << C_GREN << "wingchicken\n" << C_NRML;
      send_msg = std::string("PRIVMSG " + v[2] + " hello, bot\r\n");
      buf = send_msg.c_str();
    }
    ret = send(_my_sock, buf, strlen(buf), 0);
    if (ret == -1) {
      perror("faild to send \n");
      return ;
    }
  };
  
  void error_handle(int ret, const char *noti){
    if (ret == -1){
      std::cerr << noti << "\n";
      exit(1);
    }
  }
  
std::vector<std::string> split_msg(std::string &line, std::string s)
{
	std::vector<std::string> tab;
	std::string cmd_buf;
	size_t start = 0;
	size_t pos;

	while ((pos = line.find(s)) != std::string::npos){
		tab.push_back(line.substr(start, pos));
		line = line.substr(pos + s.length());
	}
	if (line != "")
		tab.push_back(line);
	return tab;
}

public :
  Bot(char *ip, int port, char *pass){
    _server_ip = ip;
    _server_port = port;
    _server_pass = pass;
  }
  ~Bot(){
    close(_my_sock);
  }
  
  void wingChicken(){
    error_handle(socket_init(), "faild to create socket");
    error_handle(connect(_my_sock, reinterpret_cast<sockaddr*>(&_server_addr), sizeof(_server_addr)), "faid to connect socket");
    error_handle(server_welcome(), "faild to enter to server");
    error_handle(join_channel(), "faild to join to server");
    while (true){
      error_handle(watching(), "error in watching...");
    }
  };
};



#endif