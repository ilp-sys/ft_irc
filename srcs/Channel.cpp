#include "../includes/Channel.hpp"

Channel::Channel(std::string name, int opfd) : _name(name), _opfd(opfd) {}

std::vector<Client*> Channel::getClients() { return (_clients); }
