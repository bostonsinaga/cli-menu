#ifndef __CLI_MENU__CONSOLE_CPP__
#define __CLI_MENU__CONSOLE_CPP__

#include "console.hpp"

namespace cli_menu {

  /** NORMAL */

  void Console::logString(
    mt::CR_STR text,
    CR_CLR foreground
  ) {
    std::cout << Color::getString(
      text, foreground
    );
  }

  void Console::logString(
    mt::CR_STR text,
    CR_CLR foreground,
    CR_CLR background
  ) {
    std::cout << Color::getString(
      text, foreground, background
    );
  }

  /** ITALIC */

  void Console::logItalicString(mt::CR_STR text) {
    std::cout << Color::getItalicString(text);
  }

  void Console::logItalicString(
    mt::CR_STR text,
    CR_CLR foreground
  ) {
    std::cout << Color::getItalicString(
      text, foreground
    );
  }

  void Console::logItalicString(
    mt::CR_STR text,
    CR_CLR foreground,
    CR_CLR background
  ) {
    std::cout << Color::getItalicString(
      text, foreground, background
    );
  }

  /** UNDERLINE */

  void Console::logUnderlineString(mt::CR_STR text) {
    std::cout << Color::getUnderlineString(text);
  }

  void Console::logUnderlineString(
    mt::CR_STR text,
    CR_CLR foreground
  ) {
    std::cout << Color::getUnderlineString(
      text, foreground
    );
  }

  void Console::logUnderlineString(
    mt::CR_STR text,
    CR_CLR foreground,
    CR_CLR background
  ) {
    std::cout << Color::getUnderlineString(
      text, foreground, background
    );
  }

  /** SPECIALS */

  void Console::logBoundaryLine() {
    std::cout << std::endl
      << std::string(boundaryCharactersAmount, boundaryCharacter)
      << std::endl;
  }

  void Console::logResponse(
    CR_CONSOLE_ENUM statusCode,
    mt::CR_STR reason
  ) {
    std::cout << std::endl << Console::logString(
      listPointStyle + " " + reason,
      Console::colors[statusCode]
    ) << std::endl;
  }

  /** SETTERS */

  void Console::setInterruptedCtrlC() {
    INTERRUPTED_CTRL_C.store(true);
  }

  bool Console::isInterruptedCtrlC() {

    // 'CTRL+C' is detected
    if (Console::INTERRUPTED_CTRL_C.load()) {
      std::cout << std::endl;

      Console::logResponse(MESSAGE_ERROR,
        "Interrupt signal received. Exiting program."
      );

      return true;
    }

    return false;
  }

  bool Console::setDialogInput(std::string &buffer) {

    // decoration string
    std::cout << listPointStyle << ' ';

    if (Console::isInterruptedCtrlC()) return false; // stop loop

    // user input
    std::getline(std::cin, buffer);

    if (Console::isInterruptedCtrlC()) return false; // stop loop

    // loop still running
    return true;
  }

  void Console::setColor(
    CR_CONSOLE_ENUM statusCode,
    CR_CLR color_in
  ) {
    Console::colors[statusCode] = color_in;
  }
}

#endif // __CLI_MENU__CONSOLE_CPP__