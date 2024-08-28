#ifndef __CLI_MENU__MESSAGE_H__
#define __CLI_MENU__MESSAGE_H__

#include <iostream>
#include "program.h"

namespace cli_menu {
  class Message {
  private:
    enum {HINT, WARNING, ERROR, SUCCEED, CANCELED};

    static std::string tidyUpText(mt::CR_STR text);
    static std::string getObligatoryString(mt::CR_BOL isRequired);
    static std::string getColoredTag(mt::CR_INT flag);

  public:
    static void print(
      mt::CR_INT flag,
      std::string text,
      std::string name = "",
      bool toUppercase = true
    );

    static void printDecoration(
      mt::CR_INT count,
      mt::CR_CHR ch = '.'
    );

    static void printDevError(
      mt::CR_STR funName,
      mt::CR_STR paramTypeName,
      mt::CR_STR customMessage = "unassigned"
    );

    static void printCommandList(
      Command *command,
      std::string continueString = "\n"
    );

    static void printCommandError(Command *command);

    static void printProgramError(
      Program *program,
      mt::CR_BOL isEmpty
    );

    static void printMainHelp(Program *program);
    static void printSubHelp(Command *command);
  };
}

#endif // __CLI_MENU__MESSAGE_H__