#include "Bot.hpp"

int main(int ac, char*av[]){
  if (ac != 4){
    std::cerr << "check argument\n";
    return 1;
  }
  
  Bot *bot1 = new Bot(av[1], atoi(av[2]), av[3]);

  bot1->wingChicken();

  delete bot1;
  return 0;
}




    // ----------------- receive loop----------------------
    // while (true) {
    //   char buffer[1024];
    //   int received_bytes = recv(sock, buffer, sizeof(buffer), 0);
    //   if (received_bytes < 0) {
    //       std::cerr << "Error: Failed to receive data" << std::endl;
    //       return -1;
    //   }
    //   std::cout << buffer;
    //   memset(buffer, 0, 1024);
    //   break;
    // }