#ifndef __CLI_MENU__TOGGLE_CPP__
#define __CLI_MENU__TOGGLE_CPP__

#include "toggle.hpp"

namespace cli_menu {

  Toggle::Toggle(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in
  ) : Command::Command(
    keyword_in,
    description_in,
    required_in
  ) {}

  void Toggle::copyPaste() {
    mt_uti::VecTools<mt::LD>::concatCopy(
      Result::toggles[keyword], Clipboard::pasteConditions()
    );
  }

  void Toggle::pushUnormap(CR_STR raw) {
    Result::toggles[keyword].push_back(
      Control::booleanizerTest(raw)
    );
  }
}

#endif // __CLI_MENU__TOGGLE_CPP__