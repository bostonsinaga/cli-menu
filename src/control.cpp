#ifndef __CLI_MENU__CONTROL_CPP__
#define __CLI_MENU__CONTROL_CPP__

#include <csignal>
#include "control.hpp"

namespace cli_menu {

  /** Controllers Test */

  CONTROL_CODE Control::whitespacesCheck(mt::CR_STR rawstr) {
    bool prevSpaced = false;
    std::string str;

    // force to return 'UNKNOWN' when pattern 'abc123 \t abc123' is detected
    for (mt::CR_CH ch : rawstr) {
      if (!mt_uti::StrTool::isWhitespace(ch)) {

        if (prevSpaced && !str.empty()) {
          return CONTROL_UNKNOWN;
        }

        str += ch;
        prevSpaced = false;
      }
      else prevSpaced = true;
    }

    // find a match with pattern ' abc123 \t'
    for (int i = 0; i < CONTROL_TOTAL; i++) {
      for (int j = 0; j < 2; j++) {
        if (!str.empty() && str == symbols[i][j]) {
          sharedEnum = static_cast<CONTROL_CODE>(i);
          return sharedEnum;
        }
      }
    }

    return CONTROL_UNKNOWN;
  }

  bool Control::commandHelpTest(mt::CR_STR rawstr) {
    bool isEqual = whitespacesCheck(rawstr) == CONTROL_COMMAND_HELP;
    return isEqual;
  }

  bool Control::controllerListTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_CONTROLLER_LIST;
  }

  bool Control::childrenListTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_CHILDREN_LIST;
  }

  bool Control::clearScreenTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_CLEAR_SCREEN;
  }

  bool Control::neighborNextTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_NEIGHBOR_NEXT;
  }

  bool Control::neighborPreviousTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_NEIGHBOR_PREVIOUS;
  }

  bool Control::childrenEnterTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_CHILDREN_ENTER;
  }

  bool Control::childrenExecuteTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_CHILDREN_EXECUTE;
  }

  bool Control::switchModifyTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_SWITCH_MODIFICATION;
  }

  bool Control::switchSelectTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_SWITCH_SELECTION;
  }

  bool Control::selectInputDownTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_SELECT_INPUT_DOWN;
  }

  bool Control::selectInputUpTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_SELECT_INPUT_UP;
  }

  bool Control::selectOutputDownTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_SELECT_OUTPUT_DOWN;
  }

  bool Control::selectOutputUpTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_SELECT_OUTPUT_UP;
  }

  bool Control::viewInputThisTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_VIEW_INPUT_THIS;
  }

  bool Control::viewInputChildrenTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_VIEW_INPUT_CHILDREN;
  }

  bool Control::viewOutputThisTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_VIEW_OUTPUT_THIS;
  }

  bool Control::viewOutputChildrenTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_VIEW_OUTPUT_CHILDREN;
  }

  bool Control::resetInputThisTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_RESET_INPUT_THIS;
  }

  bool Control::resetInputDescendantsTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_RESET_INPUT_DESCENDANTS;
  }

  bool Control::resetOutputThisTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_RESET_OUTPUT_THIS;
  }

  bool Control::resetOutputDescendantsTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_RESET_OUTPUT_DESCENDANTS;
  }

  bool Control::resetDataThisTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_RESET_DATA_THIS;
  }

  bool Control::resetDataDescendantsTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_RESET_DATA_DESCENDANTS;
  }

  bool Control::copyOutputTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_COPY_OUTPUT;
  }

  bool Control::pasteInputTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_PASTE_INPUT;
  }

  bool Control::nodeLevelUndoTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_NODE_LEVEL_UNDO;
  }

  bool Control::nodeLevelRedoTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_NODE_LEVEL_REDO;
  }

  bool Control::parentBackTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_PARENT_BACK;
  }

  bool Control::rootBackTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_ROOT_BACK;
  }

  bool Control::programQuitTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_PROGRAM_QUIT;
  }

  bool Control::stringIsController(mt::CR_STR rawstr) {
    return commandHelpTest(rawstr) || controllerListTest(rawstr) ||
      childrenEnterTest(rawstr) || childrenExecuteTest(rawstr) ||
      childrenListTest(rawstr) || clearScreenTest(rawstr) ||
      neighborNextTest(rawstr) || neighborPreviousTest(rawstr) ||
      switchModifyTest(rawstr) || switchSelectTest(rawstr) ||
      viewInputThisTest(rawstr) || viewInputChildrenTest(rawstr) ||
      viewOutputThisTest(rawstr) || viewOutputChildrenTest(rawstr) ||
      resetInputThisTest(rawstr) || resetInputDescendantsTest(rawstr) ||
      resetOutputThisTest(rawstr) || resetOutputDescendantsTest(rawstr) ||
      resetDataThisTest(rawstr) || resetDataDescendantsTest(rawstr) ||
      copyOutputTest(rawstr) || pasteInputTest(rawstr) ||
      nodeLevelUndoTest(rawstr) || nodeLevelRedoTest(rawstr) ||
      parentBackTest(rawstr) || rootBackTest(rawstr) ||
      programQuitTest(rawstr);
  }

  void Control::printAbbreviations(
    mt::CR_BOL withHeader,
    mt::CR<Console::Indent> indent
  ) {
    if (withHeader) {
      Console::logString(
        Langu::ageControl::getAbbreviationsTitle() + ":\n",
        Console::messageColors[CONLOR_TITLE]
      );

      Console::logItalicString(
        Console::LimitedText::wrap(Langu::ageControl::getAbbreviationsDescription()),
        Console::messageColors[CONLOR_DESCRIPTION]
      );
    }

    // display terms in rows
    for (int i = 0; i < CONTROL_TOTAL; i++) {
      std::cout << indent.get();
      std::string curterm = Langu::ageControl::getTerm(static_cast<CONTROL_CODE>(i));

      Console::logString(
        Control::symbols[i][0],
        Console::messageColors[CONLOR_LIST]
      );

      Console::logString(
        " = " + curterm + '\n',
        Console::messageColors[CONLOR_DESCRIPTION]
      );
    }
  }

  void Control::printBooleanAvailableValues(
    mt::CR_BOL withHeader,
    mt::CR<Console::Indent> indent
  ) {
    if (withHeader) {
      Console::logString(
        Langu::ageControl::getBooleanAvailableValuesTitle() + ":\n",
        Console::messageColors[CONLOR_TITLE]
      );

      Console::logItalicString(
        Console::LimitedText::wrap(Langu::ageControl::getBooleanAvailableValuesDescription()),
        Console::messageColors[CONLOR_DESCRIPTION]
      );
    }

    // boolean terms
    mt::CR_PAIR<mt::VEC_STR> boolTerms = Langu::ageBooleanizer::getTerms();

    // print sign
    std::function<void(mt::CR_CH)> printSign = [&](mt::CR_CH sign) {
      Console::logString(
        indent.get() + sign,
        Console::messageColors[CONLOR_LIST]
      );

      Console::logString(" = ", Console::messageColors[CONLOR_DESCRIPTION]);
    };

    // print code
    std::function<void(mt::CR<CONTROL_CODE>)> printCode = [&](mt::CR<CONTROL_CODE> code) {
      Console::logString(
        symbols[code][0] + ' ',
        Console::messageColors[CONLOR_DESCRIPTION]
      );
    };

    /** False Terms */

    printSign('0');

    for (int i = 0; i < boolTerms.second.size(); i++) {
      Console::logString(
        boolTerms.second[i] + ' ',
        Console::messageColors[CONLOR_DESCRIPTION]
      );
    }

    printCode(CONTROL_NEIGHBOR_PREVIOUS);

    Console::logString( // number is zero
      "n==0\n", Console::messageColors[CONLOR_DESCRIPTION]
    );

    /** True Terms */

    printSign('1');

    for (int i = 0; i < boolTerms.first.size(); i++) {
      Console::logString(
        boolTerms.first[i] + ' ',
        Console::messageColors[CONLOR_DESCRIPTION]
      );
    }

    printCode(CONTROL_CHILDREN_ENTER);
    printCode(CONTROL_NEIGHBOR_NEXT);

    Console::logString( // number is not zero
      "n!=0\n", Console::messageColors[CONLOR_DESCRIPTION]
    );

    // cancel terms
    printSign('X');
    printCode(CONTROL_PARENT_BACK);
    printCode(CONTROL_PROGRAM_QUIT);
    std::cout << std::endl;
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

    if (Control::INTERRUPTED_CTRL_C.load()) return false; // stop loop

    // user input
    std::getline(std::cin, buffer);

    if (Control::INTERRUPTED_CTRL_C.load()) return false; // stop loop

    // loop still running
    return true;
  }

  void Control::setInterruptedCtrlC(int) {
    Control::INTERRUPTED_CTRL_C.store(true);
  }

  void Control::registerInterruptedCtrlC() {
    std::signal(SIGINT, Control::setInterruptedCtrlC);
  }
}

#endif // __CLI_MENU__CONTROL_CPP__