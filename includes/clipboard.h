#ifndef __CLI_MENU__CLIPBOARD_H__
#define __CLI_MENU__CLIPBOARD_H__

#include "message.h"

namespace cli_menu {

  /** Only for string copy */
  class Clipboard {
  private:
    static void printSucceed();

  public:
    static std::string pasteText();
    static std::string pasteNumbers();
    static std::string pasteConditions();
  };
}

#endif // __CLI_MENU__CLIPBOARD_H__