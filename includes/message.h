#ifndef __CLI_MENU__MESSAGE_H__
#define __CLI_MENU__MESSAGE_H__

#include "color.h"

namespace cli_menu {

  class Message {
  private:
    static void editToCapitalFirstPeriodEnd(
      std::string &text,
      int &forwardSpaceBoundaryIndex,
      int &reverseSpaceBoundaryIndex
    );

  public:
    enum {
      MSGFG_HINT, MSGFG_WARNING, MSGFG_ERROR,
      MSGFG_SUCCEED, MSGFG_CANCELED
    };

    static std::string getColoredTag(mt::CR_INT flag);

    static void printItalicString(mt::CR_STR text);
    static void printNeatItalicString(std::string text);

    static void printUnderlineString(mt::CR_STR text);
    static void printNeatUnderlineString(std::string text);

    static void printString(
      mt::CR_STR text,
      CR_CLR foreground
    );

    static void printNeatString(
      std::string text,
      CR_CLR foreground
    );

    static void printString(
      mt::CR_STR text,
      CR_CLR foreground,
      CR_CLR background
    );

    static void printNeatString(
      std::string text,
      CR_CLR foreground,
      CR_CLR background
    );

    static void printItalicString(
      mt::CR_STR text,
      CR_CLR foreground
    );

    static void printNeatItalicString(
      std::string text,
      CR_CLR foreground
    );

    static void printItalicString(
      mt::CR_STR text,
      CR_CLR foreground,
      CR_CLR background
    );

    static void printNeatItalicString(
      std::string text,
      CR_CLR foreground,
      CR_CLR background
    );

    static void printUnderlineString(
      mt::CR_STR text,
      CR_CLR foreground
    );

    static void printNeatUnderlineString(
      std::string text,
      CR_CLR foreground
    );

    static void printUnderlineString(
      mt::CR_STR text,
      CR_CLR foreground,
      CR_CLR background
    );

    static void printNeatUnderlineString(
      std::string text,
      CR_CLR foreground,
      CR_CLR background
    );

    static void printNamed(
      mt::CR_INT flag,
      mt::CR_STR text,
      mt::CR_STR name = "",
      mt::CR_BOL toUppercase = true
    );

    static void printNeatNamed(
      mt::CR_INT flag,
      std::string text,
      mt::CR_STR name = "",
      mt::CR_BOL toUppercase = true
    );

    static void printBoundaryLine(
      int startNewlinesCount,
      int endNewlinesCount,
      mt::CR_INT count = 45,
      mt::CR_CH character = '-'
    );

    static void printDialogError(
      mt::CR_STR reason,
      int endNewlinesCount = 2
    );

    static void printNeatDialogError(
      mt::CR_STR reason,
      int endNewlinesCount = 2
    );

    // decorated input interface
    static void setDialogInput(
      std::string &buffer,
      mt::CR_STR listPointStyle = ">"
    );
  };
}

#endif // __CLI_MENU__MESSAGE_H__