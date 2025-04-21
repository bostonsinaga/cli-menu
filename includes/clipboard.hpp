#ifndef __CLI_MENU__CLIPBOARD_HPP__
#define __CLI_MENU__CLIPBOARD_HPP__

#include "message.hpp"

namespace cli_menu {

  /** Only for string copy */
  class Clipboard {
  public:
    static void pasteText(std::string &dataRef);
    static void pasteNumbers(mt::VEC_LD &numbersRef);
    static void pasteConditions(mt::VEC_BOL &conditionsRef);
  };
}

#endif // __CLI_MENU__CLIPBOARD_HPP__