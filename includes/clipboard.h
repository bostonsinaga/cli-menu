#ifndef __CLI_MENU__CLIPBOARD_H__
#define __CLI_MENU__CLIPBOARD_H__

#include "message.h"

namespace cli_menu {

  class Clipboard {
  private:
    typedef unsigned char* uchar_ptr;
    typedef std::function<bool(uchar_ptr)> rule_clbk;

    std::string errorMessage;
    rule_clbk rule;

  public:
    Clipboard(
      mt::CR_STR errorMessage_in = ""
    );

    Clipboard(
      rule_clbk rule_in,
      mt::CR_STR errorMessage_in = ""
    );

    void paste();
  };
}

#endif // __CLI_MENU__CLIPBOARD_H__