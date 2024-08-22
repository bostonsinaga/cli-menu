#ifndef __CLI_MENU__MESSAGE_H__
#define __CLI_MENU__MESSAGE_H__

#include <iostream>
#include "command.h"

namespace cli_menu {
  class Message {
  private:
    static void printDecoration(CR_INT count, CR_CHR ch = '.');
    static void setTextCapitalBeginPeriodEnd(std::string &text);
    static std::string getObligatoryString(CR_BOL isRequired);
    static std::string getColoredTag(CR_INT flag);
    static void printCommandList(Command *com, std::string contiStr = "");

  public:
    enum {HINT, WARNING, ERROR, SUCCEED, CANCELED};

    static void print(CR_INT flag, std::string text, CR_STR comName = "");
    static void printCommandError(Command *com);

    static void printMainHelp(
      Command *comProgram,
      CR_STR author = "",
      CR_STR version = "1.0"
    );
    
    static void printSubHelp(Command *com);
  };
}

#endif // __CLI_MENU__MESSAGE_H__