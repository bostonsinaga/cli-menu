#ifndef __CLI_MENU__CONTROL_CPP__
#define __CLI_MENU__CONTROL_CPP__

#include "control.hpp"

namespace cli_menu {

  /** English Presets */
  mt::STRUNORMAP<mt::ARR_STR<Control::totalSymbols>> Control::terms = {{"en", {
    "back", "clipboard", "enter", "help", "list", "modify",
    "next", "previous", "quit", "reset", "select", "view"
  }}};

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

  bool Control::backTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_BACK;
  }

  bool Control::clipboardTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_CLIPBOARD;
  }

  bool Control::enterTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_ENTER;
  }

  bool Control::helpTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_HELP;
  }

  bool Control::listTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_LIST;
  }

  bool Control::modifyTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_MODIFY;
  }

  bool Control::nextTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_NEXT;
  }

  bool Control::previousTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_PREVIOUS;
  }

  bool Control::quitTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_QUIT;
  }

  bool Control::resetTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_RESET;
  }

  bool Control::selectTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_SELECT;
  }

  bool Control::viewTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_VIEW;
  }

  void Control::printAbbreviations() {
    static bool printed = false;

    if (!printed) {
      printed = true;

      for (int i = 0; i < totalSymbols; i++) {
        std::cout << "  '" << symbols[i][0] << "' = "
          << terms[Language::currentISOCode][i] << std::endl;
      }
    }
  }

  void Control::printToggleAvailableValues() {
    static bool printed = false;

    if (!printed) {
      printed = true;
      mt::VEC_STR trueTerms = booleanizer.getTrueTerms(Language::currentISOCode);
      mt::VEC_STR falseTerms = booleanizer.getFalseTerms(Language::currentISOCode);

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
    Language::messages.erase(existingISOCode);
    terms.erase(existingISOCode);
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
    terms[Language::currentISOCode][CONTROL_BACK] = backTerm;
    terms[Language::currentISOCode][CONTROL_CLIPBOARD] = clipboardTerm;
    terms[Language::currentISOCode][CONTROL_ENTER] = enterTerm;
    terms[Language::currentISOCode][CONTROL_HELP] = helpTerm;
    terms[Language::currentISOCode][CONTROL_LIST] = listTerm;
    terms[Language::currentISOCode][CONTROL_MODIFY] = modifyTerm;
    terms[Language::currentISOCode][CONTROL_NEXT] = nextTerm;
    terms[Language::currentISOCode][CONTROL_PREVIOUS] = previousTerm;
    terms[Language::currentISOCode][CONTROL_QUIT] = quitTerm;
    terms[Language::currentISOCode][CONTROL_RESET] = resetTerm;
    terms[Language::currentISOCode][CONTROL_SELECT] = selectTerm;
    terms[Language::currentISOCode][CONTROL_VIEW] = viewTerm;
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
}

#endif // __CLI_MENU__CONTROL_CPP__