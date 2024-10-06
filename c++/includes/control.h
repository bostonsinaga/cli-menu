#ifndef __CLI_MENU__CONTROL_H__
#define __CLI_MENU__CONTROL_H__

#include "message.h"

namespace cli_menu {

  class Control {
  private:
    enum {
      CANCEL = 0, SELECT = 1,
      PREVIOUS = 2, NEXT = 3,
      BACK = 4, ENTER = 5
    };

    static const std::string NAMES[6][2];
    static int test(std::string str);

  public:
    static bool cancelTest(std::string &str);
    static bool selectTest(std::string &str);
    static bool previousTest(std::string &str);
    static bool nextTest(std::string &str);
    static bool backTest(std::string &str);
    static bool enterTest(std::string &str);
    static void printHelp(mt::CR_BOL isSupporter);
  };
}

#endif // __CLI_MENU__CONTROL_H__