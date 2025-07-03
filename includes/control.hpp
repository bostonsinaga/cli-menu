#ifndef __CLI_MENU__CONTROL_HPP__
#define __CLI_MENU__CONTROL_HPP__

#include "mini-tools.hpp"

namespace cli_menu {

  class Control {
  public:
    // can be obtained after testing the input string
    enum CODE {
      UNKNOWN,
      BACK, CLIPBOARD, ENTER, HELP,
      LIST, MODIFY, NEXT, PREVIOUS,
      QUIT, RESET, SELECT, VIEW
    };

  private:
    inline static CODE sharedEnum = UNKNOWN;
    static constexpr int totalKeys = 12;

    inline static std::string symbol = ":",
      keyLetters[totalKeys][2] = {
        ["B", "b"], // back
        ["C", "c"], // clipboard
        ["E", "e"], // enter
        ["H", "h"], // help
        ["L", "l"], // list
        ["M", "m"], // modify
        ["N", "n"], // next
        ["P", "p"], // previous
        ["Q", "q"], // quit
        ["R", "r"], // reset
        ["S", "s"], // select
        ["V", "v"]  // view
      };

    // find 'keyLetters' pattern in 'str'
    static CODE whitespacesCheck(mt::CR_STR str);

    // multilingual feature
    static mt::STRUNORMAP<mt::ARR_STR<totalKeys>> terms;

  public:
    static void rename(
      const CODE& index,
      mt::CR_STR name,
      mt::CR_STR abbreviation
    );

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

    static void printParameterInformation(mt::CR_STR existingISOCode);
    static void printToggleAvailableValues(mt::CR_STR existingISOCode);

    static const CODE getSharedEnum() {
      return sharedEnum;
    }

    /** Multilingual Feature */

    static bool termsFound(mt::CR_STR existingISOCode);

    static void addTerms(
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
    );

    static void removeTerms(mt::CR_STR existingISOCode);

    /**
     * Booleanizer Multilingual Feature.
     * Add/remove the terms with its member functions.
     */
    inline static mt_util::Booleanizer booleanizer;
  };
}

#endif // __CLI_MENU__CONTROL_HPP__