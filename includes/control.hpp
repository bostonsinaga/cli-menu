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

    /**
     * Uppercase symbol that have a different code from its left lowercase symbol
     * will be 1 level below it when displayed in the 'printAbbreviations' method.
     */
    inline static const mt::PAIR2<std::string, CONTROL_CODE>
    symbols[totalSymbols][2] = {
      {{":h", CONTROL_COMMAND_HELP      }, {":H", CONTROL_CONTROLLER_LIST   }},
      {{":e", CONTROL_CHILDREN_ENTER    }, {":E", CONTROL_CHILDREN_ENTER    }},
      {{":l", CONTROL_CHILDREN_LIST     }, {":L", CONTROL_CHILDREN_LIST     }},
      {{":>", CONTROL_NEIGHBOR_NEXT     }, {":>", CONTROL_NEIGHBOR_NEXT     }},
      {{":<", CONTROL_NEIGHBOR_PREVIOUS }, {":<", CONTROL_NEIGHBOR_PREVIOUS }},
      {{":m", CONTROL_SWITCH_MODIFY     }, {":M", CONTROL_SWITCH_MODIFY     }},
      {{":s", CONTROL_SWITCH_SELECT     }, {":S", CONTROL_SWITCH_SELECT     }},
      {{":v", CONTROL_VIEW_INPUT_THIS   }, {":V", CONTROL_VIEW_OUTPUT_THIS  }},
      {{":w", CONTROL_VIEW_INPUT_ALL    }, {":W", CONTROL_VIEW_OUTPUT_ALL   }},
      {{":r", CONTROL_RESET_INPUT_THIS  }, {":R", CONTROL_RESET_OUTPUT_THIS }},
      {{":x", CONTROL_RESET_INPUT_ALL   }, {":X", CONTROL_RESET_OUTPUT_ALL  }},
      {{":c", CONTROL_COPY_OUTPUT       }, {":C", CONTROL_COPY_OUTPUT       }},
      {{":p", CONTROL_PASTE_INPUT       }, {":P", CONTROL_PASTE_INPUT       }},
      {{":b", CONTROL_PARENT_BACK       }, {":B", CONTROL_PARENT_BACK       }},
      {{":q", CONTROL_PROGRAM_QUIT      }, {":Q", CONTROL_PROGRAM_QUIT      }}
    };

    // find 'symbols' pattern in 'rawstr'
    static CONTROL_CODE whitespacesCheck(mt::CR_STR rawstr);

    // use an atomic boolean to signal an interrupt
    inline static std::atomic<bool> INTERRUPTED_CTRL_C = false;

  public:
    Control() = delete;

    static bool commandHelpTest(mt::CR_STR rawstr);
    static bool controllerListTest(mt::CR_STR rawstr);
    static bool childrenEnterTest(mt::CR_STR rawstr);
    static bool childrenListTest(mt::CR_STR rawstr);
    static bool neighborNextTest(mt::CR_STR rawstr);
    static bool neighborPreviousTest(mt::CR_STR rawstr);
    static bool switchModifyTest(mt::CR_STR rawstr);
    static bool switchSelectTest(mt::CR_STR rawstr);
    static bool viewInputThisTest(mt::CR_STR rawstr);
    static bool viewOutputThisTest(mt::CR_STR rawstr);
    static bool viewInputAllTest(mt::CR_STR rawstr);
    static bool viewOutputAllTest(mt::CR_STR rawstr);
    static bool resetInputThisTest(mt::CR_STR rawstr);
    static bool resetOutputThisTest(mt::CR_STR rawstr);
    static bool resetInputAllTest(mt::CR_STR rawstr);
    static bool resetOutputAllTest(mt::CR_STR rawstr);
    static bool copyOutputTest(mt::CR_STR rawstr);
    static bool pasteInputTest(mt::CR_STR rawstr);
    static bool parentBackTest(mt::CR_STR rawstr);
    static bool programQuitTest(mt::CR_STR rawstr);

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