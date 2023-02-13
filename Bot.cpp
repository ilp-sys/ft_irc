#include "Bot.hpp"

// UTILS
std::vector<std::string> split_msg(std::string &line, std::string s){
	std::vector<std::string> tab;
	std::string cmd_buf;
	size_t start = 0;
	size_t pos;

	while ((pos = line.find(s)) != std::string::npos){
		tab.push_back(line.substr(start, pos));
		if (tab.size() == 3) // 이미 {발신인, 명령어, 타겟} 까지 다 담겨있으면, 마지막은 메세지스트링
			break;
		else
			line = line.substr(pos + s.length());
	}
	// 마지막 벡터는 메세지라 ':'가 붙어있어 제거
	if (line != "")
		tab.push_back(line.substr(pos + s.length() + 1));

	// spilt check
	for (std::vector<std::string>::iterator it = tab.begin(); it != tab.end(); ++it){
		std::cout << C_YLLW << *it << std::endl << C_NRML;
	}
	return tab;
}

// Bot::Bot(char *ip, int port, char *pass){
Bot::Bot(char *ip, int port){
	_server_ip = ip;
	_server_port = port;
	// _server_pass = pass;
}

Bot::~Bot(){
	close(_my_sock);
}

void Bot::wingChicken(){
	error_handle(socket_init(), "faild to create socket");
	error_handle(connect(_my_sock, reinterpret_cast<sockaddr*>(&_server_addr), sizeof(_server_addr)), "faid to connect socket");
	error_handle(server_welcome(), "faild to enter to server");
	while (true){
		error_handle(watching(), "error in watching...");
	}
}

int Bot::socket_init(){
	_my_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (_my_sock == -1){
		return -1;
	}
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_addr.s_addr = inet_addr(_server_ip);
	_server_addr.sin_port = htons(_server_port);
	return 0;
}

int Bot::server_welcome(){
	int result;
	// no password for testing
	// std::string welcome = std::string("PASS ");
	// welcome += std::string(_server_pass);
	std::string welcome = std::string("NICK icbot\r\nUSER bot ircbot tutle laptop\r\n");
	
	const char *buf; 
	buf = welcome.c_str();
	result = send(_my_sock, buf, strlen(buf), 0);
	if (result == -1) {
		perror("can't send to server");
		return 1;
	}
	return 0;
}

// watching{ thinking { botcmd } }
int Bot::watching(){
	std::cout << "관측중..........\n";
	
	char buffer[1024];
	int received_bytes = recv(_my_sock, buffer, sizeof(buffer), 0); //recv blocking(?)
	if (received_bytes < 0) {
			std::cerr << "Error: Failed to receive data" << std::endl;
			return -1;
	}
	std::string txt = std::string(buffer);
	thinking(txt);
	memset(buffer, 0, 1024);
	return 0;
}

void Bot::thinking(std::string txt){
	std::string target;
	std::string cmd;
	std::string send_msg;
	size_t pos;

	// std::string botbot;
	const char *buf;
	int ret = 0;
	
	std::vector<std::string> v = split_msg(txt, " ");
	if (v.size() != 4)
		return;
	
	target = v[2];
	cmd = v[3];
	// PRIVMSG 호출이 아니라면 무시
	if ((pos = v[1].find("PRIVMSG")) == std::string::npos){
		std::cout << C_RED << "not privmsg\n" << C_NRML;
		return ;
	}

	// PRIVMSG이면서, DM일 때는 메세지로 보내준 채널로 접속
	// msg가 "#42seoul #42gam" 일 경우 띄어쓰기로 구분되는 두번째 요소는 반영되지않음(봇이 굳이 처리할 필요 없다고 생각)
	// ','으로 구분되는 채널은 알아서 잘 들어가짐~!
	if (v[2] == "icbot"){
		if (v[3].find('#') != std::string::npos)
		send_msg = std::string("JOIN " + v[3]);
		// v[3]는 \r\n을 가지도록 스플릿 되어있음 나중에 split함수 수정시 주의
	}
	// v[3]가 특정 명령어일 때 응답하도록 구현
	// TODO : 봇을 호출하는 명령어 세분화
	else if (v[3] == "!bot\r\n"){
		std::cout << C_GREN << "wingchicken\n";
		send_msg = std::string("PRIVMSG " + v[2] + " :hello, bot\r\n");
		std::cout << send_msg << std::endl << C_NRML;
	}
	buf = send_msg.c_str();
	ret = send(_my_sock, buf, strlen(buf), 0);
	if (ret == -1) {
		perror("faild to send \n");
		return ;
	}
}

void Bot::error_handle(int ret, const char *noti){
	if (ret == -1){
		std::cerr << noti << "\n";
		exit(1);
	}
}