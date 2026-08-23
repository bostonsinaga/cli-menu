#ifndef __CLI_MENU__CONTROL_HPP__
#define __CLI_MENU__CONTROL_HPP__

#include <atomic>
#include "language.hpp"

namespace cli_menu {

  // the 'CONTROL_CODE' enum defined in 'language.hpp'
  class Control {
  private:
    inline static CONTROL_CODE sharedEnum = CONTROL_UNKNOWN;

    /**
     * Uppercase symbol that have a different code from its left lowercase symbol
     * will be 1 level below it when displayed in the 'printAbbreviations' method.
     */
    inline static const std::string symbols[CONTROL_TOTAL][2] = {
      {":h",   ""}, // CONTROL_COMMAND_HELP
      {":H",   ""}, // CONTROL_CONTROLLER_LIST
      {":l", ":L"}, // CONTROL_CHILDREN_LIST
      {":_",   ""}, // CONTROL_CLEAR_SCREEN
      {":>",   ""}, // CONTROL_NEIGHBOR_NEXT
      {":<",   ""}, // CONTROL_NEIGHBOR_PREVIOUS
      {":e",   ""}, // CONTROL_CHILDREN_ENTER
      {":E",   ""}, // CONTROL_CHILDREN_EXECUTE
      {":m", ":M"}, // CONTROL_SWITCH_MODIFICATION
      {":s", ":S"}, // CONTROL_SWITCH_SELECTION
      {":i",   ""}, // CONTROL_SELECT_INPUT_DOWN
      {":I",   ""}, // CONTROL_SELECT_INPUT_UP
      {":o",   ""}, // CONTROL_SELECT_OUTPUT_DOWN
      {":O",   ""}, // CONTROL_SELECT_OUTPUT_UP
      {":v",   ""}, // CONTROL_VIEW_INPUT_THIS
      {":V",   ""}, // CONTROL_VIEW_INPUT_CHILDREN
      {":d",   ""}, // CONTROL_VIEW_OUTPUT_THIS
      {":D",   ""}, // CONTROL_VIEW_OUTPUT_CHILDREN
      {":r",   ""}, // CONTROL_RESET_INPUT_THIS
      {":R",   ""}, // CONTROL_RESET_INPUT_DESCENDANTS
      {":w",   ""}, // CONTROL_RESET_OUTPUT_THIS
      {":W",   ""}, // CONTROL_RESET_OUTPUT_DESCENDANTS
      {":x",   ""}, // CONTROL_RESET_DATA_THIS
      {":X",   ""}, // CONTROL_RESET_DATA_DESCENDANTS
      {":c", ":C"}, // CONTROL_COPY_OUTPUT
      {":p", ":P"}, // CONTROL_PASTE_INPUT
      {":z", ":Z"}, // CONTROL_NODE_LEVEL_UNDO
      {":y", ":Y"}, // CONTROL_NODE_LEVEL_REDO
      {":b",   ""}, // CONTROL_PARENT_BACK
      {":B",   ""}, // CONTROL_ROOT_BACK
      {":q", ":Q"}  // CONTROL_PROGRAM_QUIT
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
    static bool childrenExecuteTest(mt::CR_STR rawstr);
    static bool childrenListTest(mt::CR_STR rawstr);
    static bool clearScreenTest(mt::CR_STR rawstr);
    static bool neighborNextTest(mt::CR_STR rawstr);
    static bool neighborPreviousTest(mt::CR_STR rawstr);
    static bool switchModifyTest(mt::CR_STR rawstr);
    static bool switchSelectTest(mt::CR_STR rawstr);
    static bool selectInputDownTest(mt::CR_STR rawstr);
    static bool selectInputUpTest(mt::CR_STR rawstr);
    static bool selectOutputDownTest(mt::CR_STR rawstr);
    static bool selectOutputUpTest(mt::CR_STR rawstr);
    static bool viewInputThisTest(mt::CR_STR rawstr);
    static bool viewInputChildrenTest(mt::CR_STR rawstr);
    static bool viewOutputThisTest(mt::CR_STR rawstr);
    static bool viewOutputChildrenTest(mt::CR_STR rawstr);
    static bool resetInputThisTest(mt::CR_STR rawstr);
    static bool resetInputDescendantsTest(mt::CR_STR rawstr);
    static bool resetOutputThisTest(mt::CR_STR rawstr);
    static bool resetOutputDescendantsTest(mt::CR_STR rawstr);
    static bool resetDataThisTest(mt::CR_STR rawstr);
    static bool resetDataDescendantsTest(mt::CR_STR rawstr);
    static bool copyOutputTest(mt::CR_STR rawstr);
    static bool pasteInputTest(mt::CR_STR rawstr);
    static bool nodeLevelUndoTest(mt::CR_STR rawstr);
    static bool nodeLevelRedoTest(mt::CR_STR rawstr);
    static bool parentBackTest(mt::CR_STR rawstr);
    static bool rootBackTest(mt::CR_STR rawstr);
    static bool programQuitTest(mt::CR_STR rawstr);
    static bool stringIsController(mt::CR_STR rawstr);

    static void printAbbreviations(
      mt::CR_BOL withHeader,
      mt::CR<Console::Indent> indent
    );

    static void printBooleanAvailableValues(
      mt::CR_BOL withHeader,
      mt::CR<Console::Indent> indent
    );

    static const CONTROL_CODE& getSharedEnum() {
      return sharedEnum;
    }

    /**
     * INTERRUPTED 'CTRL+C' INTERACTIONS
     * The 'editing' parameter, defined in 'Command::editing',
     * is used to switch between edit and select modes.
     * In this class, it serves to distinguish the theme.
     */

    // decorated input interface
    static bool cinDialogInput(
      std::string &buffer,
      mt::CR_BOL editing
    );

    // to prevent infinite loop after pressing 'Ctrl+C'
    static void setInterruptedCtrlC(int);
    static void registerInterruptedCtrlC();
  };
}

#endif // __CLI_MENU__CONTROL_HPP__