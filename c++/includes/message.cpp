#ifndef __CLI_MENU__MESSAGE_CPP__
#define __CLI_MENU__MESSAGE_CPP__

#include "message.h"

namespace cli_menu {

  std::string Message::getObligatoryString(mt::CR_BOL isRequired) {
    if (isRequired) return "<req>";
    else return "<opt>";
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
}

#endif // __CLI_MENU__MESSAGE_CPP__