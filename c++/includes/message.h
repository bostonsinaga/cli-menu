#ifndef __CLI_MENU__MESSAGE_H__
#define __CLI_MENU__MESSAGE_H__

#include <iostream>
#include "command.h"

namespace cli_menu {
  class Message {
  private:
    static void printDots(int count);
    static std::string setTextCapitalBeginPeriodEnd(std::string text);
    static std::string getObligatoryString(bool isRequired);
    static std::string getColoredTag(int flag);
    static void printCommand(Command *com);

  public:
    enum {HINT, WARNING, ERROR, SUCCEED, CANCELED};
    static void print(int flag, std::string text, std::string comName = "");
    static void printCommandError(Command *com);

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