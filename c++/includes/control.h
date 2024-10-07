#ifndef __CLI_MENU__CONTROL_H__
#define __CLI_MENU__CONTROL_H__

#include "message.h"

namespace cli_menu {

  class Control {
  private:
    enum {
      BACK = 0, CANCEL = 1,
      ENTER = 2, HELP  = 3, 
      LIST = 4, NEXT = 5,
      PREVIOUS = 6, SELECT = 7
    };

    static const int count = 8;
    static const std::string NAMES[count][2];
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