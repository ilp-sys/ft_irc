#include "../includes/CommandInvoker.hpp"
#include "../includes/Server.hpp"

//TODO: Connect Channel Map
CommandInvoker::CommandInvoker()
{
  _commandMap.insert(std::make_pair("NICK", new Nick()));
//   _commandMap.insert(std::make_pair("USER", new User()));
  // _commandMap.insert(std::make_pair("PASS", new Pass()));
  // _commandMap.insert(std::make_pair("PONG", new Pong()));
  // _commandMap.insert(std::make_pair("JOIN", new Join()));
  // _commandMap.insert(std::make_pair("PART", new Part()));
  // _commandMap.insert(std::make_pair("QUIT", new Quit()));
  // _commandMap.insert(std::make_pair("PRIVMSG", new Privmsg()));
  // _commandMap.insert(std::make_pair("NOTICE", new Notice()));
}

void	CommandInvoker::executeCommand(std::vector<std::string> &cmdline, int ident, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
	//TODO: Set Common Errors ex) no such commands
	Server& server = Server::getInstance();
	Client &client = server.getClients().find(ident)->second;
	if (_commandMap.find(cmdline[0]) == _commandMap.end())
	{
		_commandMap[0]->makeWriteEvent(ident, server.getChangeList(), ERR_UNKNOWNCOMMAND(client.getNickname(), cmdline[0]));
		return ;
	}
	//pass하지 않은 상태라면, PASS 밖에 실행 못함!
	if (client.getIsPassed() == false)
	{
		if (cmdline[0] != "PASS")
			return ;
	}
	else if (client.getIsRegistered() == false)
	{
		if (cmdline[0] != "NICK" && cmdline[0] != "USER")
			return ;
	}
	else
	{
		Command *command = _commandMap.find(cmdline[0])->second;
		command->execute(cmdline, client, changelist, channels);	//
	}
}


void  CommandInvoker::parseString(const std::string& raw, std::vector<std::string>& cmds)
{
  size_t  idx = 0;
  size_t  len = 0;
  size_t  end = raw.length();

  cmds.clear();
  while (idx < end)
  {
    while (raw[idx + len] != '\n' && raw[idx + len] != '\r' && raw[idx + len] != '\0')
      len++;
    if (len > 0)
      cmds.push_back(raw.substr(idx, len));
    idx += (len + 1);
    len = 0;
  }
}

void  CommandInvoker::parseLine(const std::string& msg, std::vector<std::string>& cmdline)
{
  size_t idx = 0;
  size_t len = 0;
  size_t end = msg.length();

  cmdline.clear();
  while (idx < end)
  {
    while (msg[idx + len] != ' ' && msg[idx + len] != ':' && msg[idx + len] != '\0')
      len++;
    cmdline.push_back(msg.substr(idx, len));
    if (msg[idx + len] == ':')
    {
      cmdline.push_back(msg.substr(idx + len));
      break ;
    }
    idx += (len + 1);
    len = 0;
  }
}

void CommandInvoker::commandConnector(int ident, const std::string& message)
{
	// (void)ident;
  Server& server = Server::getInstance();
  std::vector<std::string> commands;
  std::vector<std::string> cmdline;

  parseString(message, commands);
  std::vector<std::string>::iterator  it;
  for (it = commands.begin(); it < commands.end(); it++)
  {
    parseLine(*it, cmdline);
    executeCommand(cmdline, ident, server.getChangeList(), &server.getChannels());
  }
}
