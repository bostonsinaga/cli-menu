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
    static void printCommand(Command *com);

    static getColoredTag(int flag) {
      switch (flag) {
        case WARNING: {
          return "\033[34mHINT. \033[0m";
        }
        case WARNING: {
          return "\033[33mWARNING. \033[0m";
        }
        case ERROR: {
          return "\033[31mERROR. \033[0m";
        }
        case SUCCEED: {
          return "\033[32mSUCCEED. \033[0m";
        }
        default: return "\033[35mCANCELED. \033[0m";
      }
    }

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