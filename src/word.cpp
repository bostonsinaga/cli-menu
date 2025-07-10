#ifndef __CLI_MENU__WORD_CPP__
#define __CLI_MENU__WORD_CPP__

#include "word.hpp"

namespace cli_menu {

  Word::Word(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in = false
  ) : Command::Command(
    keyword_in,
    description_in,
    required_in
  ) {}

  void Word::copyPaste() {
    words[keyword].push_back(Clipboard::pasteText());
  }

  void Word::pushUnormap(CR_STR raw) {
    words[keyword].push_back(raw);
  }
}

#endif // __CLI_MENU__WORD_CPP__