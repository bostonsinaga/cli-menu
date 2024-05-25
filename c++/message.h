#ifndef __CLI_MENU__MESSAGE_H__
#define __CLI_MENU__MESSAGE_H__

#include <iostream>
#include "command.h"

namespace cli_menu {
  class Message {
  public:
    void printError(std::string text) {
      if (text.length() > 0) {
        std::cerr << "CLI Menu:  Error. " << text;
      }
    }

    void printHelp(Command *comm) {
      std::cout << "Help for '" << comm->getName() << "': \n";
    }
  };
}

#endif // __CLI_MENU__MESSAGE_H__