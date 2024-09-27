#ifndef __CLI_MENU__MESSAGE_H__
#define __CLI_MENU__MESSAGE_H__

#include "base.h"

namespace cli_menu {

  class Message {
  private:
    static constexpr mt::USI colorCodes[8] = {30, 31, 32, 33, 34, 35, 36, 37};
    static constexpr mt::USI colorsCount = 8;

  public:
    enum STATUS {
      HINT, WARNING, ERROR, SUCCEED, CANCELED
    };

    enum COLOR {
      BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, LIGHT_BLUE, WHITE
    };

    static std::string getColoredTag(mt::CR_INT flag);
    static std::string getObligatoryString(mt::CR_BOL isRequired);

    static void print(
      mt::CR_INT flag,
      std::string text,
      std::string name = "",
      bool toUppercase = true
    );

    static std::string customColored(
      mt::USI code,
      mt::CR_STR text
    );

    static void printBoundaryLine(
      mt::CR_INT count = 45,
      mt::CR_CH character = '-'
    );
  };
}

#endif // __CLI_MENU__MESSAGE_H__