#ifndef __CLI_MENU__MESSAGE_CPP__
#define __CLI_MENU__MESSAGE_CPP__

#include "message.h"

namespace cli_menu {

  void Message::editToCapitalFirstPeriodEnd(
    std::string &text,
    int &forwardSpaceBoundaryIndex,
    int &reverseSpaceBoundaryIndex
  ) {
    if (!text.empty()) {
      // forward
      for (forwardSpaceBoundaryIndex = 0;
        forwardSpaceBoundaryIndex < text.length();
        forwardSpaceBoundaryIndex++
      ) {
        if (!mt_uti::StrTools::isSpaceLine(text[forwardSpaceBoundaryIndex])) {
          mt_uti::StrTools::changeToUppercase(text[forwardSpaceBoundaryIndex]);
          forwardSpaceBoundaryIndex--;
          break;
        }
      }

      // reverse
      for (reverseSpaceBoundaryIndex = text.length() - 1;
        reverseSpaceBoundaryIndex >= 0;
        reverseSpaceBoundaryIndex--
      ) {
        if (!mt_uti::StrTools::isSpaceLine(text[reverseSpaceBoundaryIndex])) {
          text = text.substr(
            0, reverseSpaceBoundaryIndex + 1) + '.'
            + text.substr(reverseSpaceBoundaryIndex + 1
          );

          reverseSpaceBoundaryIndex += 2;
          break;
        }
      }
    }
  }

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

  void Message::printNeatItalicString(std::string text) {
    int i, j;
    editToCapitalFirstPeriodEnd(text, i, j);
    std::cout << Color::getItalicString(text, i, j);
  }

  void Message::printUnderlineString(mt::CR_STR text) {
    std::cout << Color::getUnderlineString(text);
  }

  void Message::printNeatUnderlineString(std::string text) {
    int i, j;
    editToCapitalFirstPeriodEnd(text, i, j);
    std::cout << Color::getUnderlineString(text, i, j);
  }

  void Message::printString(
    mt::CR_STR text,
    CR_CLR foreground
  ) {
    std::cout << Color::getString(
      text, foreground
    );
  }

  void Message::printNeatString(
    std::string text,
    CR_CLR foreground
  ) {
    int i, j;
    editToCapitalFirstPeriodEnd(text, i, j);

    std::cout << Color::getString(
      text, foreground, i, j
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

  void Message::printNeatString(
    std::string text,
    CR_CLR foreground,
    CR_CLR background
  ) {
    int i, j;
    editToCapitalFirstPeriodEnd(text, i, j);

    std::cout << Color::getString(
      text, foreground, background, i, j
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

  void Message::printNeatItalicString(
    std::string text,
    CR_CLR foreground
  ) {
    int i, j;
    editToCapitalFirstPeriodEnd(text, i, j);

    std::cout << Color::getItalicString(
      text, foreground, i, j
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

  void Message::printNeatItalicString(
    std::string text,
    CR_CLR foreground,
    CR_CLR background
  ) {
    int i, j;
    editToCapitalFirstPeriodEnd(text, i, j);

    std::cout << Color::getItalicString(
      text, foreground, background, i, j
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

  void Message::printNeatUnderlineString(
    std::string text,
    CR_CLR foreground
  ) {
    int i, j;
    editToCapitalFirstPeriodEnd(text, i, j);

    std::cout << Color::getUnderlineString(
      text, foreground, i, j
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

  void Message::printNeatUnderlineString(
    std::string text,
    CR_CLR foreground,
    CR_CLR background
  ) {
    int i, j;
    editToCapitalFirstPeriodEnd(text, i, j);

    std::cout << Color::getUnderlineString(
      text, foreground, background, i, j
    );
  }

  void Message::printNamed(
    mt::CR_INT flag,
    mt::CR_STR text,
    mt::CR_STR name,
    mt::CR_BOL toUppercase
  ) {
    if (text.length() > 0) {
      std::cout << std::endl;

      if (name != "") {
        std::cout << (toUppercase ?
          mt_uti::StrTools::getStringToUppercase(name) : name
        ) << ": ";
      }

      std::cout << getColoredTag(flag) << text << std::endl;
    }
  }

  void Message::printNeatNamed(
    mt::CR_INT flag,
    std::string text,
    mt::CR_STR name,
    mt::CR_BOL toUppercase
  ) {
    int i, j;
    editToCapitalFirstPeriodEnd(text, i, j);
    printNamed(flag, text, name, toUppercase);
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
    if (endNewlineCount < 0) {
      endNewlineCount = 0;
    }

    printString(
      "\n> " + reason + std::string(endNewlineCount, '\n'),
      Color::RED
    );
  }

  void Message::printNeatDialogError(
    mt::CR_STR reason,
    int endNewlineCount
  ) {
    if (endNewlineCount < 0) {
      endNewlineCount = 0;
    }

    int i, j;
    std::string text = reason + std::string(endNewlineCount, '\n');

    editToCapitalFirstPeriodEnd(text, i, j);

    std::cout << Color::getString(
      "\n> " + text, Color::RED, -1, j
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