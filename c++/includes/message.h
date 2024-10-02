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
    static std::string getObligatoryString(mt::CR_BOL isRequired);

    static void print(
      mt::CR_INT flag,
      std::string text,
      std::string name = "",
      bool toUppercase = true
    );

    static void printBoundaryLine(
      mt::CR_INT count = 45,
      mt::CR_CH character = '-'
    );
  };
}

#endif // __CLI_MENU__MESSAGE_H__