#ifndef __CLI_MENU__MESSAGE_H__
#define __CLI_MENU__MESSAGE_H__

#include <iostream>
// #include <vector>
#include "command.h"

namespace cli_menu {
  class Message {
  private:
    static void printDots(int count);
    static std::string fixTextBeginEnd(std::string text);
    static std::string getObligatoryString(bool isRequired);
    static void printCommand(Command *com);

  public:
    static void printError(std::string text, std::string comName = "CLI Menu");

    static void printMainHelp(
      VEC_COM &comms,
      std::string programName,
      std::string author = "",
      std::string version = "1.0"
    );
    
    static void printSubHelp(Command *com);
  };
}

#endif // __CLI_MENU__MESSAGE_H__