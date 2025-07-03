#ifndef __CLI_MENU__CONTROL_CPP__
#define __CLI_MENU__CONTROL_CPP__

#include "control.hpp"

namespace cli_menu {

  /** English Presets */
  mt::STRUNORMAP<mt::ARR_STR<Control::totalKeys>> Control::terms = {{"en", {
    "back", "clipboard", "enter", "help", "list", "modify",
    "next", "previous", "quit", "reset", "select", "view"
  }}};

  Control::CODE Control::whitespacesCheck(mt::CR_STR str) {
    bool prevSpaced = false;
    std::string input;

    // force to return 'UNKNOWN' when pattern 'abc123 \t abc123' is detected
    for (mt::CR_CH ch : str) {
      if (!mt_uti::StrTools::isWhitespace(ch)) {
        if (prevSpaced && !input.empty()) return UNKNOWN;
        input += ch;
      }
      else prevSpaced = true;
    }

    // find a match with pattern ' abc123 \t'
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < totalKeys; j++) {

        if (input == modeSymbol + keyLetters[j][i]) {
          sharedEnum = static_cast<CODE>(j);
          return j;
        }
      }
    }

    return UNKNOWN;
  }

  bool Control::backTest(mt::CR_STR str) {
    return whitespacesCheck(str) == BACK;
  }

  bool Control::clipboardTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CLIPBOARD;
  }

  bool Control::enterTest(mt::CR_STR str) {
    return whitespacesCheck(str) == ENTER;
  }

  bool Control::helpTest(mt::CR_STR str) {
    return whitespacesCheck(str) == HELP;
  }

  bool Control::listTest(mt::CR_STR str) {
    return whitespacesCheck(str) == LIST;
  }

  bool Control::modifyTest(mt::CR_STR str) {
    return whitespacesCheck(str) == MODIFY;
  }

  bool Control::nextTest(mt::CR_STR str) {
    return whitespacesCheck(str) == NEXT;
  }

  bool Control::previousTest(mt::CR_STR str) {
    return whitespacesCheck(str) == PREVIOUS;
  }

  bool Control::quitTest(mt::CR_STR str) {
    return whitespacesCheck(str) == QUIT;
  }

  bool Control::resetTest(mt::CR_STR str) {
    return whitespacesCheck(str) == RESET;
  }

  bool Control::selectTest(mt::CR_STR str) {
    return whitespacesCheck(str) == SELECT;
  }

  bool Control::viewTest(mt::CR_STR str) {
    return whitespacesCheck(str) == VIEW;
  }

  void Control::printAbbreviations(mt::CR_STR existingISOCode) {
    static bool printed = false;

    if (!printed && hasISOCode(existingISOCode)) {
      printed = true;

      for (int i = 0; i < totalKeys; i++) {
        std::cout << "  '" << keyLetters[i] << "' = "
          << terms[existingISOCode][i] << std::endl;
      }
    }
  }

  void Control::printToggleAvailableValues(mt::CR_STR existingISOCode) {
    static bool printed = false;

    if (!printed && booleanizer.hasISOCode(existingISOCode)) {
      printed = true;
      VEC_STR trueTerms = booleanizer.getTrueTerms();
      VEC_STR falseTerms = booleanizer.getFalseTerms();

      for (int i = 0; i < trueTerms.size() - 1; i++) {
        std::cout << '\'' << trueTerms[i] << "', ";
      }

      if (!trueTerms.empty()) {
        std::cout << '\'' << trueTerms[trueTerms.size() - 1] << "', n != 0\n";
      }

      std::cout << std::endl;

      for (int i = 0; i < falseTerms.size() - 1; i++) {
        std::cout << '\'' << falseTerms[i] << "', ";
      }

      if (!falseTerms.empty()) {
        std::cout << '\'' << falseTerms[falseTerms.size() - 1] << "', n == 0\n";
      }
    }
  }

  bool Control::hasISOCode(mt::CR_STR existingISOCode) {
    return mt::STRUNORMAP_FOUND<mt::ARR<std::string, totalKeys>(
      terms, existingISOCode
    );
  }

  void Control::addTerms(
    mt::CR_STR newISOCode,
    mt::CR_STR backTerm,
    mt::CR_STR clipboardTerm,
    mt::CR_STR enterTerm,
    mt::CR_STR helpTerm,
    mt::CR_STR listTerm,
    mt::CR_STR modifyTerm,
    mt::CR_STR nextTerm,
    mt::CR_STR previousTerm,
    mt::CR_STR quitTerm,
    mt::CR_STR resetTerm,
    mt::CR_STR selectTerm,
    mt::CR_STR viewTerm
  ) {
    if (hasISOCode(newISOCode)) {
      terms[newISOCode][BACK] = backTerm;
      terms[newISOCode][CLIPBOARD] = clipboardTerm;
      terms[newISOCode][ENTER] = enterTerm;
      terms[newISOCode][HELP] = helpTerm;
      terms[newISOCode][LIST] = listTerm;
      terms[newISOCode][MODIFY] = modifyTerm;
      terms[newISOCode][NEXT] = nextTerm;
      terms[newISOCode][PREVIOUS] = previousTerm;
      terms[newISOCode][QUIT] = quitTerm;
      terms[newISOCode][RESET] = resetTerm;
      terms[newISOCode][SELECT] = selectTerm;
      terms[newISOCode][VIEW] = viewTerm;
    }
  }

  void Control::removeTerms(mt::CR_STR existingISOCode) {
    terms[existingISOCode].erase();
  }
}

#endif // __CLI_MENU__CONTROL_CPP__