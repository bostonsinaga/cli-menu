#ifndef __CLI_MENU__MESSAGE_H__
#define __CLI_MENU__MESSAGE_H__

#include <iostream>
#include "program.h"

namespace cli_menu {
  class Message {
  private:

    static std::string tidyUpText(mt::CR_STR text);
    static std::string getObligatoryString(mt::CR_BOL isRequired);
    static std::string getColoredTag(mt::CR_INT flag);

    static const mt::USI colorCodes[] = {30, 31, 32, 33, 34, 35, 36, 37};
    static const colorsCount = 8;

  public:
    enum STATUS {HINT, WARNING, ERROR, SUCCEED, CANCELED};
    enum COLOR {BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, LIGHT_BLUE, WHITE};

    static void print(
      mt::CR_INT flag,
      std::string text,
      std::string name = "",
      bool toUppercase = true
    );

    static std::string customColored(
      mt::USI code,
      mt::CR_STR text
    );

    static void printDecoration(
      mt::CR_INT count,
      mt::CR_CH ch = '.'
    );

    static void printDevError(
      mt::CR_STR funName,
      mt::CR_STR paramTypeName,
      mt::CR_STR customMessage = "unassigned"
    );

    static void printCommandsSequence(
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