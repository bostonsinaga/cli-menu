#ifndef __CLI_MENU__CONTROL_HPP__
#define __CLI_MENU__CONTROL_HPP__

#include <atomic>
#include "language.hpp"

namespace cli_menu {

  // can be obtained after testing the input string
  enum CONTROL_CODE {
    CONTROL_UNKNOWN,
    CONTROL_HELP,
    CONTROL_LIST,
    CONTROL_ENTER,
    CONTROL_BACK,
    CONTROL_NEXT,
    CONTROL_PREVIOUS,
    CONTROL_MODIFY,
    CONTROL_SELECT,
    CONTROL_RESET,
    CONTROL_VIEW,
    CONTROL_COPY,
    CONTROL_PASTE,
    CONTROL_QUIT
  };

  class Control {
  private:
    inline static CONTROL_CODE sharedEnum = CONTROL_UNKNOWN;
    static constexpr int totalSymbols = 13;

    inline static const std::string symbols[totalSymbols][2] = {
      {":h", ":H"}, // help
      {":l", ":L"}, // list
      {":e", ":E"}, // enter
      {":b", ":B"}, // back
      {":>", ":>"}, // next
      {":<", ":<"}, // previous
      {":m", ":M"}, // modify
      {":s", ":S"}, // select
      {":r", ":R"}, // reset
      {":v", ":V"}, // view
      {":c", ":C"}, // copy
      {":p", ":P"}, // paste
      {":q", ":Q"}  // quit
    };

    // find 'symbols' pattern in 'str'
    static CONTROL_CODE whitespacesCheck(mt::CR_STR str);

    /** Multilingual Features */

    static mt::STRUNORMAP_STR
      abbreviationsTitle,
      toggleAvailableValuesTitle;

    static mt::STRUNORMAP<mt::ARR_STR<totalSymbols>> terms;
    inline static mt_uti::Booleanizer booleanizer;

    static void limitTerm(
      const CONTROL_CODE &code,
      std::string &newTerm
    );

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
    static bool resetTest(mt::CR_STR str);
    static bool viewTest(mt::CR_STR str);
    static bool copyTest(mt::CR_STR str);
    static bool pasteTest(mt::CR_STR str);
    static bool quitTest(mt::CR_STR str);

    static void printAbbreviations();
    static void printToggleAvailableValues();

    static CONTROL_CODE getSharedEnum() {
      return sharedEnum;
    }

    /** Multilingual Features */

    inline static size_t maxTermLength = 16;

    // sync 'Language::currentISOCode' with 'Language::selectISOCode' manually
    static void addISOCode(mt::CR_STR newISOCode);
    static void removeISOCode(mt::CR_STR existingISOCode);

    // titles
    static void setAbbreviationsTitle(mt::CR_STR title);
    static void setToggleAvailableValuesTitle(mt::CR_STR title);

    // string length is limited to 'maxTermLength'
    static void setTerms(
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
    );

    /** Booleanizer method call wrappers */

    static void setBooleanizerTerms(
      mt::CR_VEC_STR existingTrueTerms,
      mt::CR_VEC_STR existingFalseTerms
    );

    static bool booleanizerTest(mt::CR_STR raw);

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

    // check if interrupted before waiting for input
    static bool isInterruptedCtrlC();
  };
}

#endif // __CLI_MENU__CONTROL_HPP__