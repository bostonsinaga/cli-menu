#ifndef __CLI_MENU__TYPE_CPP__
#define __CLI_MENU__TYPE_CPP__

#include "type.hpp"

namespace cli_menu {

  /** NUMBER */

  Number::Number(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    const COMMAND_CALLBACK &callback_in
  ) : Command::Command(
    keyword_in,
    description_in,
    required_in,
    callback_in
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

  /** WORD */

  Word::Word(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    const COMMAND_CALLBACK &callback_in
  ) : Command::Command(
    keyword_in,
    description_in,
    required_in,
    callback_in
  ) {}

  void Word::copyPaste() {
    Result::words[keyword].push_back(Clipboard::pasteText());
  }

  void Word::pushUnormap(mt::CR_STR raw) {
    Result::words[keyword].push_back(raw);
  }

  /** TOGGLE */

  Toggle::Toggle(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    const COMMAND_CALLBACK &callback_in
  ) : Command::Command(
    keyword_in,
    description_in,
    required_in,
    callback_in
  ) {}

  void Toggle::copyPaste() {
    mt_uti::VecTools<bool>::concatCopy(
      Result::toggles[keyword], Clipboard::pasteConditions()
    );
  }

  void Toggle::pushUnormap(mt::CR_STR raw) {
    Result::toggles[keyword].push_back(
      Control::booleanizerTest(raw)
    );
  }
}

#endif // __CLI_MENU__TYPE_CPP__