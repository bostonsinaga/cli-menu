#ifndef __CLI_MENU__CLIPBOARD_H__
#define __CLI_MENU__CLIPBOARD_H__

#include "message.h"

namespace cli_menu {

  /** Only for string copy */
  class Clipboard {
  private:
    static bool internalCalling;
    static void printSucceed(mt::CR_INT endNewlinesCount);

  public:
    static void pasteText(std::string &dataRef);
    static void pasteNumbers(mt::VEC_LD &numbersRef);
    static void pasteConditions(mt::VEC_BOL &conditionsRef);
  };
}

#endif // __CLI_MENU__CLIPBOARD_H__