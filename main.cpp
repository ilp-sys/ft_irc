#include "Bot.hpp"

// test server : irc.atw-inter.net
// 94.125.182.253 6667
// no password argument

int main(int ac, char*av[]){
  // if (ac != 4){
  //   std::cerr << "check argument\n";
  //   return 1;
  // }
  // Bot *bot1 = new Bot(av[1], atoi(av[2]), av[3]);
  
	if (ac != 3){
    std::cerr << "check argument\n";
    return 1;
  }
  Bot *bot1 = new Bot(av[1], atoi(av[2]));

  bot1->wingChicken();

  delete bot1;
	
  return 0;
}