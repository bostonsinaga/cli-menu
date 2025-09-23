#ifndef __CLI_MENU__CONTROL_HPP__
#define __CLI_MENU__CONTROL_HPP__

#include <atomic>
#include "language.hpp"

namespace cli_menu {

  // the 'CONTROL_CODE' enum defined in 'language.hpp'
  class Control {
  private:
    inline static CONTROL_CODE sharedEnum = CONTROL_UNKNOWN;
    static constexpr int totalSymbols = 19;

    /**
     * Uppercase symbol that have a different code from its left lowercase symbol
     * will be 1 level below it when displayed in the 'printAbbreviations' method.
     */
    inline static const mt::PAIR2<std::string, CONTROL_CODE>
    symbols[totalSymbols][2] = {
      {{":h", CONTROL_HELP             }, {":H", CONTROL_HELP              }},
      {{":l", CONTROL_LIST             }, {":L", CONTROL_LIST              }},
      {{":e", CONTROL_ENTER            }, {":E", CONTROL_ENTER             }},
      {{":b", CONTROL_BACK             }, {":B", CONTROL_BACK              }},
      {{":>", CONTROL_NEXT             }, {":>", CONTROL_NEXT              }},
      {{":<", CONTROL_PREVIOUS         }, {":<", CONTROL_PREVIOUS          }},
      {{":m", CONTROL_MODIFY           }, {":M", CONTROL_MODIFY            }},
      {{":s", CONTROL_SELECT           }, {":S", CONTROL_SELECT            }},
      {{":v", CONTROL_VIEW_INPUT_THIS  }, {":V", CONTROL_VIEW_OUTPUT_THIS  }},
      {{":w", CONTROL_VIEW_INPUT_ALL   }, {":W", CONTROL_VIEW_OUTPUT_ALL   }},
      {{":r", CONTROL_RESET_INPUT_THIS }, {":R", CONTROL_RESET_OUTPUT_THIS }},
      {{":x", CONTROL_RESET_INPUT_ALL  }, {":X", CONTROL_RESET_OUTPUT_ALL  }},
      {{":c", CONTROL_COPY             }, {":C", CONTROL_COPY              }},
      {{":p", CONTROL_PASTE            }, {":P", CONTROL_PASTE             }},
      {{":q", CONTROL_QUIT             }, {":Q", CONTROL_QUIT              }}
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
    static bool viewInputThisTest(mt::CR_STR str);
    static bool viewInputAllTest(mt::CR_STR str);
    static bool viewOutputThisTest(mt::CR_STR str);
    static bool viewOutputAllTest(mt::CR_STR str);
    static bool resetInputThisTest(mt::CR_STR str);
    static bool resetInputAllTest(mt::CR_STR str);
    static bool resetOutputThisTest(mt::CR_STR str);
    static bool resetOutputAllTest(mt::CR_STR str);
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