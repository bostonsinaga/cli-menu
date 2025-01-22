#ifndef __CLI_MENU__CONTROL_CPP__
#define __CLI_MENU__CONTROL_CPP__

#include <csignal>
#include <windows.h>
#include "control.h"

namespace cli_menu {
  /**
   * The test argument is expected to be lowercase.
   */
  std::string Control::NAMES[TOTAL][2] = {
    { "$",         "$" },
    { "back",      "b" },
    { "clipboard", "c" },
    { "enter",     "e" },
    { "help",      "h" }, 
    { "list",      "l" },
    { "modify",    "m" },
    { "next",      "n" },
    { "previous",  "p" },
    { "quit",      "q" },
    { "select",    "s" }
  };

  mt::USI Control::sharedFlag = -1;
  bool Control::modeOn = false;

  void Control::rename(
    _CONTROL_FLAG flag,
    mt::CR_STR name,
    mt::CR_STR abbreviation
  ) {
    NAMES[flag][0] = name;
    NAMES[flag][1] = abbreviation;
  }

  mt::SI Control::whitespacesCheck(mt::CR_STR str) {
    bool prevSpaced = false;
    std::string input;

    // force return when 'abc123 space abc123' is detected
    for (mt::CR_CH ch : str) {
      if (!mt_uti::StrTools::isWhitespace(ch)) {
        if (prevSpaced && !input.empty()) return -1;
        input += ch;
      }
      else prevSpaced = true;
    }

    // find a match with pattern ' abc123 \t'
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < TOTAL; j++) {

        if (input == NAMES[j][i]) {
          sharedFlag = j;
          return j;
        }
      }
    }

    return -1;
  }

  bool Control::intoMode(mt::CR_STR str) {

    // the 'modeOn' is set in 'printError' method
    if (whitespacesCheck(str) == _CONTROL_MODE) {

      std::cout << Color::start(Color::MAGENTA);
      return true;
    }

    return false;
  }

  bool Control::checkOut(mt::CR_STR str, mt::CR_SI flag) {

    if (whitespacesCheck(str) == flag) {
      std::cout << Color::end();
      modeOn = false;
      return true;
    }

    return false;
  }

  bool Control::backTest(mt::CR_STR str) {
    return checkOut(str, BACK);
  }

  bool Control::clipboardTest(mt::CR_STR str) {
    return checkOut(str, CLIPBOARD);
  }

  bool Control::enterTest(mt::CR_STR str) {
    return checkOut(str, ENTER);
  }

  bool Control::helpTest(mt::CR_STR str) {
    return checkOut(str, HELP);
  }

  bool Control::listTest(mt::CR_STR str) {
    return checkOut(str, LIST);
  }

  bool Control::modifyTest(mt::CR_STR str) {
    return checkOut(str, MODIFY);
  }

  bool Control::nextTest(mt::CR_STR str) {
    return checkOut(str, NEXT);
  }

  bool Control::previousTest(mt::CR_STR str) {
    return checkOut(str, PREVIOUS);
  }

  bool Control::quitTest(mt::CR_STR str) {
    return checkOut(str, QUIT);
  }

  bool Control::selectTest(mt::CR_STR str) {
    return checkOut(str, SELECT);
  }

  int Control::booleanTest(mt::CR_STR str) {
    if (str == "y" || str == "yes" ||
      str == "1" || str == "true"
    ) {
      return 2;
    }
    else if (str == "n" || str == "no" ||
      str == "0" || str == "false"
    ) {
      return 1;
    }
    return 0;
  }

  bool Control::revealBoolean(mt::CR_INT testedFlag) {
    if (testedFlag > 1) return true;
    return false;
  }

  void Control::handleCtrlC(int signal) {
    if (signal == SIGINT) {
      RUNNING = false;
      CON_VAR.notify_all();
    }
  }

  void Control::handleKeypress() {
    std::unique_lock<std::mutex> lock(CON_VAR_MUTEX);

    while (RUNNING) {
      CON_VAR.wait_for(
        lock,
        std::chrono::milliseconds(100),
        [] { return !RUNNING; }
      );

      if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) &&
        (GetAsyncKeyState(VK_MENU) & 0x8000)
      ) {
        for (char ch = 'A'; ch <= 'Z'; ++ch) {

          if (GetAsyncKeyState(ch) & 0x8000) {
            std::cout << "Ctrl + Alt + " << ch << " pressed!" << std::endl;
            break;
          }
        }
      }
    }
  }

  void Control::printParameterHelp() {

    static int maxLengths[] = {0, 0};
    static std::string text = "";

    if (!(maxLengths[0] || maxLengths[1])) {
      bool even;
      int j;

      for (int i = 0; i < TOTAL - 1; i++) {
        j = i+1;
        even = !(i % 2);

        if (NAMES[j][0].length() > maxLengths[even]) {
          maxLengths[even] = NAMES[j][0].length();
        }
      }

      for (int i = 0; i < TOTAL - 1; i++) {

        j = i+1;
        even = !(i % 2);
        text += even ? "  " : " ";

        text += NAMES[j][0] + std::string(
          maxLengths[even] - NAMES[j][0].length(), ' '
        );

        text += " = " + NAMES[j][1];

        if (j < TOTAL - 1) {
          text += even ? "," : ",\n";
        }
        else text += "\n";
      }
    }

    Message::printItalicString(text);
  }

  void Control::printToggleHelp() {
    Message::printItalicString(
      "  yes = y, no = n, or boolean\n"
    );
  }

  void Control::printError(mt::CR_STR listPointStyle) {
    if (modeOn) {
      Message::printItalicString(
        listPointStyle + " unknown controller\n",
        Color::GRAY
      );

      std::cout << Color::start(Color::CYAN);
    }
    /**
     * Set here not in 'intoMode' method to avoid
     * error message at the beginning. Because this method
     * always the first invoked after 'intoMode' condition.
     */
    else modeOn = true;
  }
}

#endif // __CLI_MENU__CONTROL_CPP__