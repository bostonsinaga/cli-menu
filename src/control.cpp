#ifndef __CLI_MENU__CONTROL_CPP__
#define __CLI_MENU__CONTROL_CPP__

#include "control.hpp"

namespace cli_menu {

  /** Unordered Maps with English Preset */

  mt::STRUNORMAP_STR
    Control::abbreviationsTitle = {{"en", "Controller List"}},
    Control::toggleAvailableValuesTitle = {{"en", "Boolean Available Values"}};

  mt::STRUNORMAP<mt::ARR_STR<Control::totalSymbols>> Control::terms = {{"en", {
    "help", "list", "enter", "back", "next", "previous", "modify",
    "select", "reset", "view", "copy", "paste", "quit"
  }}};

  /** Controllers Test */

  CONTROL_CODE Control::whitespacesCheck(mt::CR_STR str) {
    bool prevSpaced = false;
    std::string input;

    // force to return 'UNKNOWN' when pattern 'abc123 \t abc123' is detected
    for (mt::CR_CH ch : str) {
      if (!mt_uti::StrTools::isWhitespace(ch)) {
        if (prevSpaced && !input.empty()) return CONTROL_UNKNOWN;
        input += ch;
      }
      else prevSpaced = true;
    }

    // find a match with pattern ' abc123 \t'
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < totalSymbols; j++) {

        if (input == symbols[j][i]) {
          sharedEnum = static_cast<CONTROL_CODE>(j+1);
          return sharedEnum;
        }
      }
    }

    return CONTROL_UNKNOWN;
  }

  bool Control::helpTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_HELP;
  }

  bool Control::listTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_LIST;
  }

  bool Control::enterTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_ENTER;
  }

  bool Control::backTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_BACK;
  }

  bool Control::nextTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_NEXT;
  }

  bool Control::previousTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_PREVIOUS;
  }

  bool Control::modifyTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_MODIFY;
  }

  bool Control::selectTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_SELECT;
  }

  bool Control::resetTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_RESET;
  }

  bool Control::viewTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_VIEW;
  }

  bool Control::copyTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_COPY;
  }

  bool Control::pasteTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_PASTE;
  }

  bool Control::quitTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_QUIT;
  }

  void Control::printAbbreviations() {

    Console::logItalicString(
      abbreviationsTitle[Language::currentISOCode] + ":\n",
      Console::messageColors[CONSOLE_HINT_1]
    );

    size_t leastMaxTermLength = 0;

    // find the maximum term length at even indices
    for (int i = 0; i < totalSymbols; i++) {

      size_t curlen = terms[Language::currentISOCode][i].length();

      if (i % 2 == 0 && curlen > leastMaxTermLength) {
        leastMaxTermLength = curlen;
      }
    }

    // display terms with 2 columns
    for (int i = 0; i < totalSymbols; i++) {
      std::string curterm = terms[Language::currentISOCode][i];

      // even is followed by spaces, uneven is followed by newline
      Console::logString(
        "  " + symbols[i][0] + " = " + curterm
        + (i % 2 ? "\n" : std::string(leastMaxTermLength - curterm.length(), ' ')),
        Console::messageColors[CONSOLE_HINT_2]
      );
    }

    std::cout << "\n\n";
  }

  void Control::printToggleAvailableValues() {

    Console::logItalicString(
      toggleAvailableValuesTitle[Language::currentISOCode] + ":\n  ",
      Console::messageColors[CONSOLE_HINT_1]
    );

    mt::VEC_STR trueTerms = booleanizer.getTrueTerms(Language::currentISOCode);
    mt::VEC_STR falseTerms = booleanizer.getFalseTerms(Language::currentISOCode);

    // true terms
    for (int i = 0; i < trueTerms.size(); i++) {
      Console::logString(
        trueTerms[i] + ", ",
        Console::messageColors[CONSOLE_HINT_2]
      );
    }

    // number is not zero
    Console::logString(
      "n != 0\n  ",
      Console::messageColors[CONSOLE_HINT_2]
    );

    // false terms
    for (int i = 0; i < falseTerms.size(); i++) {
      Console::logString(
        falseTerms[i] + ", ",
        Console::messageColors[CONSOLE_HINT_2]
      );
    }

    // number is zero
    Console::logString(
      "n == 0\n\n",
      Console::messageColors[CONSOLE_HINT_2]
    );
  }

  /** Multilingual Features */

  void Control::addISOCode(mt::CR_STR newISOCode) {
    Language::messages[newISOCode] = {};
    terms[newISOCode] = {};
    booleanizer.addTerms(newISOCode, {}, {});
  }

  void Control::removeISOCode(mt::CR_STR existingISOCode) {
    Language::messages.erase(existingISOCode);
    terms.erase(existingISOCode);
    booleanizer.removeTerms(existingISOCode);
  }

  void Control::setAbbreviationsTitle(mt::CR_STR title) {
    abbreviationsTitle[Language::currentISOCode] = title;
  }

  void Control::setToggleAvailableValuesTitle(mt::CR_STR title) {
    toggleAvailableValuesTitle[Language::currentISOCode] = title;
  }

  void Control::limitTerm(
    const CONTROL_CODE &code,
    std::string &newTerm
  ) {
    if (newTerm.length() > Control::maxTermLength) {
      newTerm = newTerm.substr(0, Control::maxTermLength - 2);
      newTerm += "..";
    }

    terms[Language::currentISOCode][code] = newTerm;
  }

  void Control::setTerms(
    std::string helpTerm,
    std::string listTerm,
    std::string enterTerm,
    std::string backTerm,
    std::string nextTerm,
    std::string previousTerm,
    std::string modifyTerm,
    std::string selectTerm,
    std::string resetTerm,
    std::string viewTerm,
    std::string copyTerm,
    std::string pasteTerm,
    std::string quitTerm
  ) {
    Control::limitTerm(CONTROL_HELP, helpTerm);
    Control::limitTerm(CONTROL_LIST, listTerm);
    Control::limitTerm(CONTROL_ENTER, enterTerm);
    Control::limitTerm(CONTROL_BACK, backTerm);
    Control::limitTerm(CONTROL_NEXT, nextTerm);
    Control::limitTerm(CONTROL_PREVIOUS, previousTerm);
    Control::limitTerm(CONTROL_MODIFY, modifyTerm);
    Control::limitTerm(CONTROL_SELECT, selectTerm);
    Control::limitTerm(CONTROL_RESET, resetTerm);
    Control::limitTerm(CONTROL_VIEW, viewTerm);
    Control::limitTerm(CONTROL_COPY, copyTerm);
    Control::limitTerm(CONTROL_PASTE, pasteTerm);
    Control::limitTerm(CONTROL_QUIT, quitTerm);
  }

  void Control::setBooleanizerTerms(
    mt::CR_VEC_STR existingTrueTerms,
    mt::CR_VEC_STR existingFalseTerms
  ) {
    booleanizer.changeTerms(
      Language::currentISOCode,
      existingTrueTerms,
      existingFalseTerms
    );
  }

  bool Control::booleanizerTest(mt::CR_STR raw) {
    return booleanizer.test(
      Language::currentISOCode, raw
    );
  }

  /** Interrupted 'Ctrl+C' Interactions */

  bool Control::cinDialogInput(
    std::string &buffer,
    mt::CR_BOL editing
  ) {
    // decoration string
    Console::logString(
      Console::listPointStyle + ' ',
      Console::chooseBoundaryColor(editing)
    );

    if (Control::isInterruptedCtrlC()) return false; // stop loop

    // user input
    std::getline(std::cin, buffer);

    if (Control::isInterruptedCtrlC()) return false; // stop loop

    // loop still running
    return true;
  }

  void Control::setInterruptedCtrlC(int) {
    Control::INTERRUPTED_CTRL_C.store(true);
  }

  bool Control::isInterruptedCtrlC() {

    // 'Ctrl+C' is detected
    if (Control::INTERRUPTED_CTRL_C.load()) {
      return true;
    }

    return false;
  }
}

#endif // __CLI_MENU__CONTROL_CPP__