#ifndef __CLI_MENU__CONTROL_CPP__
#define __CLI_MENU__CONTROL_CPP__

#include "control.hpp"

namespace cli_menu {

  /** English Presets */
  mt::STRUNORMAP<mt::ARR_STR<Control::totalSymbols>> Control::terms = {{"en", {
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
      for (int j = 0; j < totalSymbols; j++) {

        if (input == symbols[j][i]) {
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

  void Control::printAbbreviations() {
    static bool printed = false;

    if (!printed) {
      printed = true;

      for (int i = 0; i < totalSymbols; i++) {
        std::cout << "  '" << symbols[i][1] << "' = "
          << terms[Language::getCurrentISOCode()][i] << std::endl;
      }
    }
  }

  void Control::printToggleAvailableValues() {
    static bool printed = false;

    if (!printed) {
      printed = true;
      mt::VEC_STR trueTerms = booleanizer.getTrueTerms(Language::getCurrentISOCode());
      mt::VEC_STR falseTerms = booleanizer.getFalseTerms(Language::getCurrentISOCode());

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

  void Control::addISOCode(mt::CR_STR newISOCode) {
    Language::messages[newISOCode] = {};
    terms[newISOCode] = {};
    booleanizer.addTerms(newISOCode, {}, {});
  }

  void Control::removeISOCode(mt::CR_STR existingISOCode) {
    Language::messages[existingISOCode].erase();
    terms[existingISOCode].erase();
    booleanizer.removeTerms(existingISOCode);
  }

  void Control::setTerms(
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
    terms[Language::getCurrentISOCode()][BACK] = backTerm;
    terms[Language::getCurrentISOCode()][CLIPBOARD] = clipboardTerm;
    terms[Language::getCurrentISOCode()][ENTER] = enterTerm;
    terms[Language::getCurrentISOCode()][HELP] = helpTerm;
    terms[Language::getCurrentISOCode()][LIST] = listTerm;
    terms[Language::getCurrentISOCode()][MODIFY] = modifyTerm;
    terms[Language::getCurrentISOCode()][NEXT] = nextTerm;
    terms[Language::getCurrentISOCode()][PREVIOUS] = previousTerm;
    terms[Language::getCurrentISOCode()][QUIT] = quitTerm;
    terms[Language::getCurrentISOCode()][RESET] = resetTerm;
    terms[Language::getCurrentISOCode()][SELECT] = selectTerm;
    terms[Language::getCurrentISOCode()][VIEW] = viewTerm;
  }

  void Control::setBooleanizerTerms(
    mt::CR_VEC_STR existingTrueTerms,
    mt::CR_VEC_STR existingFalseTerms
  ) {
    booleanizer.changeTerms(
      Language::getCurrentISOCode(),
      existingTrueTerms,
      existingFalseTerms
    );
  }

  bool Control::booleanizerTest(mt::CR_STR raw) {
    return booleanizer.test(
      Language::getCurrentISOCode(), raw
    );
  }
}

#endif // __CLI_MENU__CONTROL_CPP__