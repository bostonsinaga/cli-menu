#ifndef __CLI_MENU__TYPE_CPP__
#define __CLI_MENU__TYPE_CPP__

#include "type.hpp"

namespace cli_menu {

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
  ) {
    hyphens = "-";
    stringifiedTypeIndex = LANGUAGE_WORD_STRINGIFIED_TYPE;
  }

  void Word::copyPaste() {
    Result::words[keyword].push_back(Clipboard::pasteText());
  }

  void Word::pushUnormap(mt::CR_STR input) {
    Result::words[keyword].push_back(input);
  }

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
  ) {
    hyphens = "-";
    stringifiedTypeIndex = LANGUAGE_NUMBER_STRINGIFIED_TYPE;
  }

  void Number::copyPaste() {
    mt_uti::VecTools<mt::LD>::concatCopy(
      Result::numbers[keyword], Clipboard::pasteNumbers()
    );
  }

  void Number::pushUnormap(mt::CR_STR input) {
    mt_uti::VecTools<mt::LD>::concatCopy(
      Result::numbers[keyword], mt_uti::Scanner::parseNumbers<mt::LD>(input)
    );
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
  ) {
    hyphens = "--";
    stringifiedTypeIndex = LANGUAGE_TOGGLE_STRINGIFIED_TYPE;
  }

  void Toggle::copyPaste() {
    mt_uti::VecTools<bool>::concatCopy(
      Result::toggles[keyword], Clipboard::pasteConditions()
    );
  }

  void Toggle::pushUnormap(mt::CR_STR input) {
    Result::toggles[keyword].push_back(
      Control::booleanizerTest(input)
    );
  }
}

#endif // __CLI_MENU__TYPE_CPP__