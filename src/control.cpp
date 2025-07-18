#ifndef __CLI_MENU__CONTROL_CPP__
#define __CLI_MENU__CONTROL_CPP__

#include "control.hpp"

namespace cli_menu {

  /** Unordered Maps with English Preset */

  mt::STRUNORMAP_STR
    Control::abbreviationsTitle = {{"en", "Controllers"}},
    Control::toggleAvailableValuesTitle = {{"en", "Toggle Available Values"}};

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
      Console::messageColors[CONSOLE_HIGHLIGHT]
    );

    for (int i = 0; i < totalSymbols; i++) {
      Console::logString(
        "  " + symbols[i][0] + " = "
        + terms[Language::currentISOCode][i] + '\n',
        Console::messageColors[CONSOLE_HINT]
      );
    }

    // additional newline
    std::cout << std::endl;
  }

  void Control::printToggleAvailableValues() {

    Console::logItalicString(
      toggleAvailableValuesTitle[Language::currentISOCode] + ":\n  ",
      Console::messageColors[CONSOLE_HIGHLIGHT]
    );

    mt::VEC_STR trueTerms = booleanizer.getTrueTerms(Language::currentISOCode);
    mt::VEC_STR falseTerms = booleanizer.getFalseTerms(Language::currentISOCode);

    // true terms
    for (int i = 0; i < trueTerms.size(); i++) {
      Console::logString(
        trueTerms[i] + ", ",
        Console::messageColors[CONSOLE_HINT]
      );
    }

    // number is not zero
    Console::logString(
      "n != 0\n  ",
      Console::messageColors[CONSOLE_HINT]
    );

    // false terms
    for (int i = 0; i < falseTerms.size(); i++) {
      Console::logString(
        falseTerms[i] + ", ",
        Console::messageColors[CONSOLE_HINT]
      );
    }

    // number is zero
    Console::logString(
      "n == 0\n\n",
      Console::messageColors[CONSOLE_HINT]
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

  void Control::setTerms(
    mt::CR_STR helpTerm,
    mt::CR_STR listTerm,
    mt::CR_STR enterTerm,
    mt::CR_STR backTerm,
    mt::CR_STR nextTerm,
    mt::CR_STR previousTerm,
    mt::CR_STR modifyTerm,
    mt::CR_STR selectTerm,
    mt::CR_STR resetTerm,
    mt::CR_STR viewTerm,
    mt::CR_STR copyTerm,
    mt::CR_STR pasteTerm,
    mt::CR_STR quitTerm
  ) {
    terms[Language::currentISOCode][CONTROL_HELP] = helpTerm;
    terms[Language::currentISOCode][CONTROL_LIST] = listTerm;
    terms[Language::currentISOCode][CONTROL_ENTER] = enterTerm;
    terms[Language::currentISOCode][CONTROL_BACK] = backTerm;
    terms[Language::currentISOCode][CONTROL_NEXT] = nextTerm;
    terms[Language::currentISOCode][CONTROL_PREVIOUS] = previousTerm;
    terms[Language::currentISOCode][CONTROL_MODIFY] = modifyTerm;
    terms[Language::currentISOCode][CONTROL_SELECT] = selectTerm;
    terms[Language::currentISOCode][CONTROL_RESET] = resetTerm;
    terms[Language::currentISOCode][CONTROL_VIEW] = viewTerm;
    terms[Language::currentISOCode][CONTROL_COPY] = copyTerm;
    terms[Language::currentISOCode][CONTROL_PASTE] = pasteTerm;
    terms[Language::currentISOCode][CONTROL_QUIT] = quitTerm;
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