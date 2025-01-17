#ifndef __CLI_MENU__MESSAGE_CPP__
#define __CLI_MENU__MESSAGE_CPP__

#include "message.h"

namespace cli_menu {

  char Message::boundaryCharacter = '-';
  int Message::boundaryCharactersAmount = 45;
  std::string Message::listPointStyle = ">";

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

  std::string Message::getColoredTag(
    mt::CR_USI flag,
    mt::CR_STR text
  ) {
    switch (flag) {
      case HINT_FLAG: {
        return Color::getString(
          text.empty() ? "HINT. " : text,
          Color::SKY_BLUE
        );
      }
      case WARNING_FLAG: {
        return Color::getString(
          text.empty() ? "WARNING. " : text,
          Color::YELLOW
        );
      }
      case ERROR_FLAG: {
        return Color::getString(
          text.empty() ? "ERROR. " : text,
          Color::RED
        );
      }
      case SUCCEED_FLAG: {
        return Color::getString(
          text.empty() ? "SUCCEED. " : text,
          Color::GREEN
        );
      }
      case CANCELED_FLAG: {
        return Color::getString(
          text.empty() ? "CANCELED. " : text,
          Color::ORANGE
        );
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
    mt::CR_USI flag,
    mt::CR_STR text,
    mt::CR_STR name,
    mt::CR_BOL toUppercase
  ) {
    if (text.length() > 0) {
      std::cout << std::endl;

      if (!name.empty()) {
        std::cout << (toUppercase ?
          mt_uti::StrTools::getStringToUppercase(name) : name
        ) << ": ";
      }

      std::cout << getColoredTag(flag) << text << std::endl;
    }
  }

  void Message::printNeatNamed(
    mt::CR_USI flag,
    std::string text,
    mt::CR_STR name,
    mt::CR_BOL toUppercase
  ) {
    int i, j;
    editToCapitalFirstPeriodEnd(text, i, j);
    printNamed(flag, text, name, toUppercase);
  }

  void Message::printBoundaryLine(
    int startNewlinesCount,
    int endNewlinesCount
  ) {
    static int ct = 0;
    static char ch;
    static std::string boundaryLine;

    startNewlinesCount *= startNewlinesCount >= 0;
    endNewlinesCount *= endNewlinesCount >= 0;

    if (boundaryCharactersAmount > 0 &&
      (ct != boundaryCharactersAmount || ch != boundaryCharacter)
    ) {
      ct = boundaryCharactersAmount;
      ch = boundaryCharacter;
      boundaryLine = std::string(ct, ch);
    }

    if (ct > 0) std::cout
      << std::string(startNewlinesCount, '\n')
      << boundaryLine
      << std::string(endNewlinesCount, '\n');
  }

  void Message::printDialog(
    mt::CR_USI flag,
    mt::CR_STR reason,
    int endNewlinesCount
  ) {
    endNewlinesCount *= endNewlinesCount >= 0;

    std::cout
      << getColoredTag(flag, getListPointStyle() + reason)
      << std::string(endNewlinesCount, '\n');
  }

  void Message::printNeatDialog(
    mt::CR_USI flag,
    std::string reason,
    mt::CR_INT endNewlinesCount
  ) {
    int i, j;
    editToCapitalFirstPeriodEnd(reason, i, j);
    printDialog(flag, reason, endNewlinesCount);
  }

  void Message::setDialogInput(std::string &buffer) {
    std::cout << listPointStyle << ' ';
    std::getline(std::cin, buffer);
  }
}

#endif // __CLI_MENU__MESSAGE_CPP__