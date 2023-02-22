#include "../includes/CommandInvoker.hpp"
#include "../includes/Server.hpp"
#include "../includes/Nick.hpp"
#include "../includes/Join.hpp"
#include "../includes/Part.hpp"
#include "../includes/Privmsg.hpp"
#include "../includes/Notice.hpp"

//TODO: Connect Channel Map
CommandInvoker::CommandInvoker()
{
  _commandMap.insert(std::make_pair("NICK", new Nick()));
  // _commandMap.insert(std::make_pair("PASS", new Pass()));
  // _commandMap.insert(std::make_pair("USER", new Client()));
  // _commandMap.insert(std::make_pair("PONG", new Pong()));
   _commandMap.insert(std::make_pair("JOIN", new Join()));
  _commandMap.insert(std::make_pair("PART", new Part()));
  // _commandMap.insert(std::make_pair("QUIT", new Quit()));
   _commandMap.insert(std::make_pair("PRIVMSG", new Privmsg()));
   _commandMap.insert(std::make_pair("NOTICE", new Notice()));
}

void CommandInvoker::executeCommand(std::vector<std::string> &cmdline, int ident, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
  int res;
  
  //TODO: Set Common Errors ex) no such commands
  if (_commandMap.find(cmdline[0]) == _commandMap.end())
  {
    std::cout << "no command" << std::endl;
  }
  else
  {
    Client &client = Server::getInstance().getClients().find(ident)->second;
    Command *command = _commandMap.find(cmdline[0])->second;
    command->execute(cmdline, client, changelist, NULL);
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
    if (len > 0)
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
  Server& server = Server::getInstance();
  std::vector<std::string> commands;
  std::vector<std::string> cmdline;

  //TODO: fix/check newline before colon
  parseString(message, commands);
  std::vector<std::string>::iterator  it;
  for (it = commands.begin(); it < commands.end(); it++)
  {
    parseLine(*it, cmdline);
    executeCommand(cmdline, ident, server.getChangeList(), NULL);
  }
}
