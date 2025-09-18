#ifndef __CLI_MENU__CONTROL_HPP__
#define __CLI_MENU__CONTROL_HPP__

#include <atomic>
#include "language.hpp"

namespace cli_menu {

  // the 'CONTROL_CODE' enum defined in 'language.hpp'
  class Control {
  private:
    inline static CONTROL_CODE sharedEnum = CONTROL_UNKNOWN;
    static constexpr int totalSymbols = 15;

    inline static const std::string symbols[totalSymbols][2] = {
      {":h", ":H"}, // help
      {":l", ":L"}, // list
      {":e", ":E"}, // enter
      {":b", ":B"}, // back
      {":>", ":>"}, // next
      {":<", ":<"}, // previous
      {":m", ":M"}, // modify
      {":s", ":S"}, // select
      {":r", ":R"}, // reset this
      {":v", ":V"}, // view this
      {":x", ":X"}, // reset all
      {":w", ":W"}, // view all
      {":c", ":C"}, // copy
      {":p", ":P"}, // paste
      {":q", ":Q"}  // quit
    };

    // find 'symbols' pattern in 'str'
    static CONTROL_CODE whitespacesCheck(mt::CR_STR str);

    // use an atomic boolean to signal an interrupt
    inline static std::atomic<bool> INTERRUPTED_CTRL_C = false;

  public:
    Control() = delete;

    static bool helpTest(mt::CR_STR str);
    static bool listTest(mt::CR_STR str);
    static bool enterTest(mt::CR_STR str);
    static bool backTest(mt::CR_STR str);
    static bool nextTest(mt::CR_STR str);
    static bool previousTest(mt::CR_STR str);
    static bool modifyTest(mt::CR_STR str);
    static bool selectTest(mt::CR_STR str);
    static bool resetThisTest(mt::CR_STR str);
    static bool viewThisTest(mt::CR_STR str);
    static bool resetAllTest(mt::CR_STR str);
    static bool viewAllTest(mt::CR_STR str);
    static bool copyTest(mt::CR_STR str);
    static bool pasteTest(mt::CR_STR str);
    static bool quitTest(mt::CR_STR str);

    static void printAbbreviations(
      mt::CR_BOL titleDisplayed,
      mt::CR_SZ numberOfIndents
    );

    static void printBooleanAvailableValues(
      mt::CR_BOL titleDisplayed,
      mt::CR_SZ numberOfIndents
    );

    static const CONTROL_CODE& getSharedEnum() {
      return sharedEnum;
    }

    /**
     * INTERRUPTED 'CTRL+C' INTERACTIONS
     * The 'editing' parameter, defined in 'Command::editing',
     * is used to switch between editing and selecting modes.
     * In this class, it serves to distinguish the theme.
     */

    // decorated input interface
    static bool cinDialogInput(
      std::string &buffer,
      mt::CR_BOL editing
    );

    // to prevent infinite loop after pressing 'Ctrl+C'
    static void setInterruptedCtrlC(int);
  };
}

#endif // __CLI_MENU__CONTROL_HPP__