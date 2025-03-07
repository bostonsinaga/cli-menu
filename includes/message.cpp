#ifndef __CLI_MENU__MESSAGE_CPP__
#define __CLI_MENU__MESSAGE_CPP__

#include "message.h"

namespace cli_menu {

  char Message::boundaryCharacter = '-';
  int Message::boundaryCharactersAmount = 45;
  std::string Message::listPointStyle = ">";
  std::atomic<bool> Message::INTERRUPTED_CTRL_C(false);

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
    mt_uti::StrTools::findSpaceBoundaryIndex(text, i, j);
    mt_uti::StrTools::tidyUp(text, true, i, j);
    std::cout << Color::getItalicString(text, i, j);
  }

  void Message::printUnderlineString(mt::CR_STR text) {
    std::cout << Color::getUnderlineString(text);
  }

  void Message::printNeatUnderlineString(std::string text) {
    int i, j;
    mt_uti::StrTools::findSpaceBoundaryIndex(text, i, j);
    mt_uti::StrTools::tidyUp(text, true, i, j);
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
    mt_uti::StrTools::findSpaceBoundaryIndex(text, i, j);
    mt_uti::StrTools::tidyUp(text, true, i, j);

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
    mt_uti::StrTools::findSpaceBoundaryIndex(text, i, j);
    mt_uti::StrTools::tidyUp(text, true, i, j);

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
    mt_uti::StrTools::findSpaceBoundaryIndex(text, i, j);
    mt_uti::StrTools::tidyUp(text, true, i, j);

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
    mt_uti::StrTools::findSpaceBoundaryIndex(text, i, j);
    mt_uti::StrTools::tidyUp(text, true, i, j);

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
    mt_uti::StrTools::findSpaceBoundaryIndex(text, i, j);
    mt_uti::StrTools::tidyUp(text, true, i, j);

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
    mt_uti::StrTools::findSpaceBoundaryIndex(text, i, j);
    mt_uti::StrTools::tidyUp(text, true, i, j);

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
    mt_uti::StrTools::findSpaceBoundaryIndex(text, i, j);
    mt_uti::StrTools::tidyUp(text, true, i, j);
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
    mt::CR_STR reason
  ) {
    static bool initialNewline = true;

    if (initialNewline) {
      initialNewline = false;
      std::cout << std::endl;
    }

    std::cout << getColoredTag(
      enumeration, listPointStyle + " " + reason
    ) << std::endl;
  }

  void Message::printNeatDialog(
    mt::CR_USI enumeration,
    std::string reason
  ) {
    int i, j;
    mt_uti::StrTools::findSpaceBoundaryIndex(text, i, j);
    mt_uti::StrTools::tidyUp(text, true, i, j);
    printDialog(enumeration, reason);
  }

  // check if interrupted before waiting for input
  bool Message::interruptedCtrlC() {

    if (Message::INTERRUPTED_CTRL_C.load()) {
      std::cout << std::endl;

      printDialog(MESSAGE_ERROR,
        "Interrupt signal received. Exiting program."
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