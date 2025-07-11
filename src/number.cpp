#ifndef __CLI_MENU__NUMBER_CPP__
#define __CLI_MENU__NUMBER_CPP__

#include "number.hpp"

namespace cli_menu {

  Number::Number(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in
  ) : Command::Command(
    keyword_in,
    description_in,
    required_in
  ) {}

  void Number::copyPaste() {
    mt_uti::VecTools<mt::LD>::concatCopy(
      Result::numbers[keyword], Clipboard::pasteNumbers()
    );
  }

  void Number::pushUnormap(mt::CR_STR raw) {
    mt_uti::VecTools<mt::LD>::concatCopy(
      Result::numbers[keyword], mt_uti::Scanner::parseNumbers<mt::LD>(raw)
    );
  }
}

#endif // __CLI_MENU__NUMBER_CPP__