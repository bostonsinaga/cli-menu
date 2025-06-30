#ifndef __CLI_MENU__CLIPBOARD_HPP__
#define __CLI_MENU__CLIPBOARD_HPP__

#include "language.hpp"

namespace cli_menu {

  /** ONLY FOR STRING COPY! */
  class Clipboard {
  public:
    static std::string pasteText();
    static mt::VEC_LD pasteNumbers();
    static mt::VEC_BOL pasteConditions();

    Clipboard() = delete;
  };
}

#endif // __CLI_MENU__CLIPBOARD_HPP__