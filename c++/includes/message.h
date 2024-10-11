#ifndef __CLI_MENU__MESSAGE_H__
#define __CLI_MENU__MESSAGE_H__

#include "color.h"

namespace cli_menu {

  class Message {
  public:
    enum STATUS {
      HINT, WARNING, ERROR, SUCCEED, CANCELED
    };

    static std::string getColoredTag(mt::CR_INT flag);
    static void printItalicString(mt::CR_STR text);
    static void printUnderlineString(mt::CR_STR text);

    static void printString(
      mt::CR_STR text,
      CR_CLR foreground
    );

    static void printString(
      mt::CR_STR text,
      CR_CLR foreground,
      CR_CLR background
    );

    static void printItalicString(
      mt::CR_STR text,
      CR_CLR foreground
    );

    static void printItalicString(
      mt::CR_STR text,
      CR_CLR foreground,
      CR_CLR background
    );

    static void printUnderlineString(
      mt::CR_STR text,
      CR_CLR foreground
    );

    static void printUnderlineString(
      mt::CR_STR text,
      CR_CLR foreground,
      CR_CLR background
    );

    static void printNamed(
      mt::CR_INT flag,
      mt::CR_STR text,
      std::string name = "",
      mt::CR_BOL toUppercase = true
    );

    static void printBoundaryLine(
      mt::CR_INT count = 45,
      mt::CR_CH character = '-'
    );

    // decorated input interface
    static void setDialogInput(
      std::string &buffer,
      mt::CR_CH ch = '>'
    );
  };
}

#endif // __CLI_MENU__MESSAGE_H__