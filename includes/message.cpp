#ifndef __CLI_MENU__MESSAGE_CPP__
#define __CLI_MENU__MESSAGE_CPP__

#include "message.h"

namespace cli_menu {

  char Message::boundaryCharacter = '-';
  int Message::boundaryCharactersAmount = 45;
  std::string Message::listPointStyle = ">";
  std::atomic<bool> Message::INTERRUPTED_CTRL_C(false);

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
    mt::CR_USI enumeration,
    mt::CR_STR text
  ) {
    switch (enumeration) {
      case MESSAGE_HINT: {
        return Color::getString(
          text.empty() ? "HINT. " : text,
          Color::SKY_BLUE
        );
      }
      case MESSAGE_WARNING: {
        return Color::getString(
          text.empty() ? "WARNING. " : text,
          Color::YELLOW
        );
      }
      case MESSAGE_ERROR: {
        return Color::getString(
          text.empty() ? "ERROR. " : text,
          Color::RED
        );
      }
      case MESSAGE_SUCCEED: {
        return Color::getString(
          text.empty() ? "SUCCEED. " : text,
          Color::GREEN
        );
      }
      case MESSAGE_CANCELED: {
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
    mt::CR_USI enumeration,
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

      std::cout << getColoredTag(enumeration) << text << std::endl;
    }
  }

  void Message::printNeatNamed(
    mt::CR_USI enumeration,
    std::string text,
    mt::CR_STR name,
    mt::CR_BOL toUppercase
  ) {
    int i, j;
    editToCapitalFirstPeriodEnd(text, i, j);
    printNamed(enumeration, text, name, toUppercase);
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
    mt::CR_USI enumeration,
    mt::CR_STR reason,
    int endNewlinesCount
  ) {
    endNewlinesCount *= endNewlinesCount >= 0;

    std::cout
      << getColoredTag(enumeration, "\n" + listPointStyle + " " + reason)
      << std::string(endNewlinesCount, '\n');
  }

  void Message::printNeatDialog(
    mt::CR_USI enumeration,
    std::string reason,
    mt::CR_INT endNewlinesCount
  ) {
    int i, j;
    editToCapitalFirstPeriodEnd(reason, i, j);
    printDialog(enumeration, reason, endNewlinesCount);
  }

  // check if interrupted before waiting for input
  bool Message::interruptedCtrlC() {

    if (Message::INTERRUPTED_CTRL_C.load()) {
      std::cout << std::endl;

      printDialog(
        MESSAGE_ERROR, "Interrupt signal received. Exiting program.", 1
      );

      return true;
    }

    return false;
  }

  bool Message::setDialogInput(std::string &buffer) {

    // decoration string
    std::cout << listPointStyle << ' ';

    if (interruptedCtrlC()) return false;

    // user input
    std::getline(std::cin, buffer);

    if (interruptedCtrlC()) return false;

    // loop still running
    return true;
  }
}

#endif // __CLI_MENU__MESSAGE_CPP__