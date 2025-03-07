#ifndef __CLI_MENU__MESSAGE_H__
#define __CLI_MENU__MESSAGE_H__

#include <atomic>
#include "color.h"
#include "language.h"

namespace cli_menu {

  // to select the color of a string or tag word
  enum MESSAGE_ENUM {
    MESSAGE_HINT, MESSAGE_WARNING, MESSAGE_ERROR,
    MESSAGE_SUCCEED, MESSAGE_CANCELED
  };

  class Message {
  private:
    // the 'text' can replace the tag
    static std::string getColoredTag(
      mt::CR_USI enumeration,
      mt::CR_STR text = ""
    );

  public:
    // use an atomic boolean to signal an interrupt
    static std::atomic<bool> INTERRUPTED_CTRL_C;

    // 2 after beginning + 4 around colon + 3 before end
    static const int frameSpacesCount = 9;

    static char boundaryCharacter;
    static int boundaryCharactersAmount;
    static std::string listPointStyle;

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
      mt::CR_USI enumeration,
      mt::CR_STR text,
      mt::CR_STR name = "",
      mt::CR_BOL toUppercase = true
    );

    static void printNeatNamed(
      mt::CR_USI enumeration,
      std::string text,
      mt::CR_STR name = "",
      mt::CR_BOL toUppercase = true
    );

    static void printBoundaryLine(
      int startNewlinesCount,
      int endNewlinesCount
    );

    static void printDialog(
      mt::CR_USI enumeration,
      mt::CR_STR reason
    );

    static void printNeatDialog(
      mt::CR_USI enumeration,
      std::string reason
    );

    // to prevent infinite loop after pressing 'ctrl+c'
    static bool interruptedCtrlC();

    // decorated input interface
    static bool setDialogInput(std::string &buffer);
  };
}

#endif // __CLI_MENU__MESSAGE_H__