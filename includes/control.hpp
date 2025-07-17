#ifndef __CLI_MENU__CONTROL_HPP__
#define __CLI_MENU__CONTROL_HPP__

#include <atomic>
#include "language.hpp"

namespace cli_menu {

  // can be obtained after testing the input string
  enum CONTROL_CODE {
    CONTROL_UNKNOWN,
    CONTROL_BACK, CONTROL_CLIPBOARD,
    CONTROL_ENTER, CONTROL_HELP,
    CONTROL_LIST, CONTROL_MODIFY,
    CONTROL_NEXT, CONTROL_PREVIOUS,
    CONTROL_QUIT, CONTROL_RESET,
    CONTROL_SELECT, CONTROL_VIEW
  };

  class Control {
  private:
    inline static CONTROL_CODE sharedEnum = CONTROL_UNKNOWN;
    static constexpr int totalSymbols = 12;

    inline static const std::string symbols[totalSymbols][2] = {
      {":b", ":B"}, // back
      {":c", ":C"}, // clipboard
      {":e", ":E"}, // enter
      {":h", ":H"}, // help
      {":l", ":L"}, // list
      {":m", ":M"}, // modify
      {":n", ":N"}, // next
      {":p", ":P"}, // previous
      {":q", ":Q"}, // quit
      {":r", ":R"}, // reset
      {":s", ":S"}, // select
      {":v", ":V"}  // view
    };

    // find 'symbols' pattern in 'str'
    static CONTROL_CODE whitespacesCheck(mt::CR_STR str);

    // multilingual features
    static mt::STRUNORMAP<mt::ARR_STR<totalSymbols>> terms;
    inline static mt_uti::Booleanizer booleanizer;

    // use an atomic boolean to signal an interrupt
    inline static std::atomic<bool> INTERRUPTED_CTRL_C = false;

  public:
    Control() = delete;

    static bool backTest(mt::CR_STR str);
    static bool quitTest(mt::CR_STR str);
    static bool clipboardTest(mt::CR_STR str);
    static bool enterTest(mt::CR_STR str);
    static bool helpTest(mt::CR_STR str);
    static bool listTest(mt::CR_STR str);
    static bool modifyTest(mt::CR_STR str);
    static bool nextTest(mt::CR_STR str);
    static bool previousTest(mt::CR_STR str);
    static bool resetTest(mt::CR_STR str);
    static bool selectTest(mt::CR_STR str);
    static bool viewTest(mt::CR_STR str);

    static void printAbbreviations();
    static void printToggleAvailableValues();

    static CONTROL_CODE getSharedEnum() {
      return sharedEnum;
    }

    /** Multilingual Features */

    // sync 'Language::currentISOCode' with 'Language::selectISOCode' manually
    static void addISOCode(mt::CR_STR newISOCode);
    static void removeISOCode(mt::CR_STR existingISOCode);

    static void setTerms(
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
    );

    static void setBooleanizerTerms(
      mt::CR_VEC_STR existingTrueTerms,
      mt::CR_VEC_STR existingFalseTerms
    );

    static bool booleanizerTest(mt::CR_STR raw);

    /**
     * INTERRUPTED 'CTRL+C' INTERACTIONS
     * The 'selecting' parameter, defined in 'Command::selecting',
     * is used to switch between editing and selecting modes.
     * In this class, it serves to distinguish the theme.
     */

    // decorated input interface
    static bool cinDialogInput(
      std::string &buffer,
      mt::CR_BOL selecting
    );

    // to prevent infinite loop after pressing 'Ctrl+C'
    static void setInterruptedCtrlC(int);

    // check if interrupted before waiting for input
    static bool isInterruptedCtrlC();
  };
}

#endif // __CLI_MENU__CONTROL_HPP__