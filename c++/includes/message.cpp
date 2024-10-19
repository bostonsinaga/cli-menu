#ifndef __CLI_MENU__MESSAGE_CPP__
#define __CLI_MENU__MESSAGE_CPP__

#include "message.h"

namespace cli_menu {

  std::string Message::getColoredTag(mt::CR_INT flag) {
    switch (flag) {
      case STATUS::HINT: {
        return Color::getString("HINT. ", Color::SKY_BLUE);
      }
      case STATUS::WARNING: {
        return Color::getString("WARNING. ", Color::YELLOW);
      }
      case STATUS::ERROR: {
        return Color::getString("ERROR. ", Color::RED);
      }
      case STATUS::SUCCEED: {
        return Color::getString("SUCCEED. ", Color::GREEN);
      }
      case STATUS::CANCELED: {
        return Color::getString("CANCELED. ", Color::BLUE);
      }
    }
    return "";
  }

  void Message::printItalicString(mt::CR_STR text) {
    std::cout << Color::getItalicString(text);
  }

  void Message::printUnderlineString(mt::CR_STR text) {
    std::cout << Color::getUnderlineString(text);
  }

  void Message::printString(
    mt::CR_STR text,
    CR_CLR foreground
  ) {
    std::cout << Color::getString(
      text, foreground
    );
  }

  void Message::printString(
    mt::CR_STR text,
    CR_CLR foreground,
    CR_CLR background
  ) {
    std::cout << Color::getString(
      text, foreground, background
    );
  }

  void Message::printItalicString(
    mt::CR_STR text,
    CR_CLR foreground
  ) {
    std::cout << Color::getItalicString(
      text, foreground
    );
  }

  void Message::printItalicString(
    mt::CR_STR text,
    CR_CLR foreground,
    CR_CLR background
  ) {
    std::cout << Color::getItalicString(
      text, foreground, background
    );
  }

  void Message::printUnderlineString(
    mt::CR_STR text,
    CR_CLR foreground
  ) {
    std::cout << Color::getUnderlineString(
      text, foreground
    );
  }

  void Message::printUnderlineString(
    mt::CR_STR text,
    CR_CLR foreground,
    CR_CLR background
  ) {
    std::cout << Color::getUnderlineString(
      text, foreground, background
    );
  }

  void Message::printNamed(
    mt::CR_INT flag,
    mt::CR_STR text,
    std::string name,
    mt::CR_BOL toUppercase
  ) {
    if (text.length() > 0) {
      std::cout << std::endl;

      if (toUppercase) {
        mt_uti::StrTools::changeStringToUppercase(name);
      }

      if (name != "") {
        std::cout << name << ": ";
      }

      std::cout << getColoredTag(flag) << text << std::endl;
    }
  }

  void Message::printBoundaryLine(
    mt::CR_INT count,
    mt::CR_CH character
  ) {
    static int ct = 0;
    static char ch;
    static std::string boundaryLine;

    if (count > 0 && (ct != count || ch != character)) {
      ct = count;
      ch = character;
      boundaryLine = std::string(ct, ch);
    }

    if (ct > 0) std::cout << std::endl << boundaryLine;
  }

  void Message::printDialogError(
    mt::CR_STR reason,
    int endNewlineCount
  ) {
    if (endNewlineCount < 0) endNewlineCount = 0;

    printString(
      "\n> " + reason + std::string(endNewlineCount, '\n'),
      Color::RED
    );
  }

  void Message::setDialogInput(
    std::string &buffer,
    mt::CR_CH ch
  ) {
    std::cout << ch << ' ';
    std::getline(std::cin, buffer);
  }
}

#endif // __CLI_MENU__MESSAGE_CPP__