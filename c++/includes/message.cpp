#ifndef __CLI_MENU__MESSAGE_CPP__
#define __CLI_MENU__MESSAGE_CPP__

#include "message.h"

namespace cli_menu {

  // set text to start with a capital letter and end with a period
  std::string Message::tidyUpText(mt::CR_STR text) {
    std::string retStr = text;

    if (!text.empty()) {

      const char last = retStr[text.length() - 1];
      retStr[0] = std::toupper(retStr[0]);

      if (last != '.' && last != ';' &&
        last != '!' && last != '?' &&
        last != ':' && last != ','
      ) {
        retStr.push_back('.');
      }
      else if (last == ',') {
        retStr.pop_back();
        retStr.push_back('.');
      }
    }

    return retStr;
  }

  std::string Message::getObligatoryString(mt::CR_BOL isRequired) {
    if (isRequired) return "<req>";
    else return "<opt>";
  }

  std::string Message::customColored(
    mt::USI code,
    mt::CR_STR text
  ) {
    if (code < 0 || code > colorsCount) code = COLOR.WHITE;
    return "\033[" + std::to_string(colorCodes[code]) + "m" + text + "\033[0m";
  }

  std::string Message::getColoredTag(mt::CR_INT flag) {
    switch (flag) {
      case STATUS.HINT: {
        return customColored(COLOR.LIGHT_BLUE, "HINT. ");
      }
      case STATUS.WARNING: {
        return customColored(COLOR.YELLOW, "WARNING. ");
      }
      case STATUS.ERROR: {
        return customColored(COLOR.RED, "ERROR. ");
      }
      case STATUS.SUCCEED: {
        return customColored(COLOR.GREEN, "SUCCEED. ");
      }
      case STATUS.CANCELED: {
        return customColored(COLOR.BLUE, "CANCELED. ");
      }
    }
    return "";
  }

  void Message::print(
    mt::CR_INT flag,
    std::string text,
    std::string name,
    bool toUppercase
  ) {
    if (text.length() > 0) {
      std::cout << std::endl;

      if (toUppercase) {
        mt_uti::StrTools::changeStringToUppercase(name);
      }

      if (name != "") {
        std::cout << name << ": ";
      }

      std::cout << getColoredTag(flag) << tidyUpText(text);
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

    if (ct > 0) std::cout << std::endl << boundaryLine << std::endl;
  }
}

#endif // __CLI_MENU__MESSAGE_CPP__