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
	std::string watching_ret;
	std::string thinking_ret;

	error_handle(socket_init(), "faild to create socket");
	error_handle(connect(_my_sock, reinterpret_cast<sockaddr*>(&_server_addr), sizeof(_server_addr)), "faid to connect socket");
	error_handle(server_welcome(), "faild to enter to server");
	while (true){
		watching_ret = watching();
		thinking_ret = thinking(watching_ret);
		answering(thinking_ret); // must be null guard
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
	std::cout << C_BLUE << "connect to server\n" << C_NRML;
	return 0;
}

// watching{ thinking { botcmd } }
std::string Bot::watching(){
	std::cout << "관측중..........\n";
	std::string watched;
	char recv_buf[1024];
	memset(recv_buf, 0, 1024);
	
	int received_bytes = recv(_my_sock, recv_buf, sizeof(recv_buf), 0); //recv blocking(?)
	if (received_bytes < 0) {
			// maybe server off
			std::cerr << "Error: Failed to receive data" << std::endl;
			exit(1);
	}
	std::cout << C_BLUE << "[recv] -> " << recv_buf << C_NRML << std::endl;
	watched = std::string(recv_buf);
	return watched;
}

std::string Bot::thinking(std::string watched_msg){
	size_t pos;
	int ret = 0;
	std::string think = "";

	std::cout << "생각중.........\n";

	std::vector<std::string> v = split_msg(watched_msg, " ");
	std::cout << "----파싱완료----\n";
	// v = {발신인, irc명령어, 타겟, 메세지}
	if (v.size() != 4)
		return think;

	// PRIVMSG 호출이 아니라면 무시
	if ((pos = v[1].find("PRIVMSG")) == std::string::npos){
		return think;
	}
	
	_target = v[2];
	// DM일 때는 메세지로 보내준 채널로 접속
	if (_target == "icbot"){
		if (v[3].find('#') != std::string::npos){
			// v[3]는 \r\n을 가지도록 스플릿 되어있음 나중에 split함수 수정시 주의
			think = std::string("JOIN " + v[3]).c_str();
		}
	}
	else{
		std::cout << "명령어 처리하러 가기\n";
		// v[3]가 특정 명령어일 때 응답하도록 구현
		think = make_sense(v[3]);
	}
	return think;
}

// PRIVMSG target :answer
// 무슨 기능을 추가하지?
std::string Bot::make_sense(std::string bot_cmd){
	std::cout << "작성중 ......\n";
	std::string msg = "";
	if (bot_cmd == "!bot\r\n")
		msg = std::string("PRIVMSG " + _target + " :Did you miss me?\r\n");
	else if (bot_cmd == "!swang\r\n")
		msg = std::string("PRIVMSG " + _target + " :taylor swangft\r\n");
	else if (bot_cmd == "!jiwahn\r\n")
		msg = std::string("PRIVMSG " + _target + " :vim master\r\n");
	else if (bot_cmd == "!namkim\r\n")
		msg = std::string("PRIVMSG " + _target + " :south.k\r\n"); 
	std::cout << C_YLLW << ":" << msg << C_NRML << std::endl;
	return msg;
}


void Bot::answering(std::string answer){
	std::cout << "응답중......\n";
	const char *msg;
	if (answer == "")
		return ;
	std::cout << C_YLLW << ":" << answer << C_NRML << std::endl;
	msg = answer.c_str();
	int ret = send(_my_sock, msg, strlen(msg), 0);
	if (ret == -1) {
		std::cerr << "faild to send \n";
		exit(1);
	}
}

void Bot::error_handle(int ret, const char *noti){
	if (ret == -1){
		std::cerr << noti << "\n";
		exit(1);
	}
}
