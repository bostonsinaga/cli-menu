#ifndef __CLI_MENU__CONTROL_HPP__
#define __CLI_MENU__CONTROL_HPP__

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

  typedef const CONTROL_CODE& CR_CONTROL_CODE;

  class Control {
  private:
    inline static CONTROL_CODE sharedEnum = CONTROL_UNKNOWN;
    static constexpr int totalSymbols = 12;
    inline static std::string symbol = ":";

    inline static const std::string symbols[totalSymbols][2] = {
      {":B", ":b"}, // back
      {":C", ":c"}, // clipboard
      {":E", ":e"}, // enter
      {":H", ":h"}, // help
      {":L", ":l"}, // list
      {":M", ":m"}, // modify
      {":N", ":n"}, // next
      {":P", ":p"}, // previous
      {":Q", ":q"}, // quit
      {":R", ":r"}, // reset
      {":S", ":s"}, // select
      {":V", ":v"}  // view
    };

    // find 'symbols' pattern in 'str'
    static CONTROL_CODE whitespacesCheck(mt::CR_STR str);

    // multilingual features
    static mt::STRUNORMAP<mt::ARR_STR<totalSymbols>> terms;
    inline static mt_uti::Booleanizer booleanizer;

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
  };
}

#endif // __CLI_MENU__CONTROL_HPP__