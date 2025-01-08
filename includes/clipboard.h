#ifndef __CLI_MENU__CLIPBOARD_H__
#define __CLI_MENU__CLIPBOARD_H__

#include "message.h"

namespace cli_menu {

  class Clipboard {
  private:
    typedef std::function<bool(mt::CR_STR)> rule_clbk;

    std::string errorMessage;
    rule_clbk rule;

    static bool defaultRule(mt::CR_STR text);

  public:
    Clipboard();
    Clipboard(rule_clbk rule_in);
    Clipboard(mt::CR_STR errorMessage_in);

    Clipboard(
      rule_clbk rule_in,
      mt::CR_STR errorMessage_in
    );

    // only for string copy
    std::string paste();
  };
}

#endif // __CLI_MENU__CLIPBOARD_H__