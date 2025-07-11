#ifndef __CLI_MENU__CLIPBOARD_HPP__
#define __CLI_MENU__CLIPBOARD_HPP__

#include "control.hpp"

namespace cli_menu {

  /**
   * COPY & PASTE (only available for string).
   * The main purpose for this is to enable pasting large
   * amounts of hidden text in the CLI via the interface,
   * using the ':c' symbol from 'Control' within
   * the 'Command::dialog' method by using
   * 'Command::copyPaste' method.
   */
  class Clipboard {
  public:
    Clipboard() = delete;

    static std::string pasteText();
    static mt::VEC_LD pasteNumbers();
    static mt::VEC_BOL pasteConditions();
  };
}

#endif // __CLI_MENU__CLIPBOARD_HPP__