#ifndef __CLI_MENU__MESSAGE_CPP__
#define __CLI_MENU__MESSAGE_CPP__

#include "message.hpp"

namespace cli_menu {

  /** NORMAL */

  void Message::logString(
    mt::CR_STR text,
    CR_CLR foreground
  ) {
    std::cout << Color::getString(
      text, foreground
    );
  }

  void Message::logString(
    mt::CR_STR text,
    CR_CLR foreground,
    CR_CLR background
  ) {
    std::cout << Color::getString(
      text, foreground, background
    );
  }

  /** ITALIC */

  void Message::logItalicString(mt::CR_STR text) {
    std::cout << Color::getItalicString(text);
  }

  void Message::logItalicString(
    mt::CR_STR text,
    CR_CLR foreground
  ) {
    std::cout << Color::getItalicString(
      text, foreground
    );
  }

  void Message::logItalicString(
    mt::CR_STR text,
    CR_CLR foreground,
    CR_CLR background
  ) {
    std::cout << Color::getItalicString(
      text, foreground, background
    );
  }

  /** UNDERLINE */

  void Message::logUnderlineString(mt::CR_STR text) {
    std::cout << Color::getUnderlineString(text);
  }

  void Message::logUnderlineString(
    mt::CR_STR text,
    CR_CLR foreground
  ) {
    std::cout << Color::getUnderlineString(
      text, foreground
    );
  }

  void Message::logUnderlineString(
    mt::CR_STR text,
    CR_CLR foreground,
    CR_CLR background
  ) {
    std::cout << Color::getUnderlineString(
      text, foreground, background
    );
  }

  /** SPECIALS */

  void Message::logBoundaryLine() {
    std::cout << std::endl
      << std::string(boundaryCharactersAmount, boundaryCharacter)
      << std::endl;
  }

  void Message::logResponse(
    CR_MESSAGE_ENUM statusCode,
    mt::CR_STR reason
  ) {
    std::cout << std::endl << Message::logString(
      listPointStyle + " " + reason,
      Message::colors[statusCode]
    ) << std::endl;
  }

  /** SETTERS */

  void Message::setInterruptedCtrlC() {
    INTERRUPTED_CTRL_C.store(true);
  }

  bool Message::isInterruptedCtrlC() {

    // 'CTRL+C' is detected
    if (Message::INTERRUPTED_CTRL_C.load()) {
      std::cout << std::endl;

      Message::logResponse(MESSAGE_ERROR,
        "Interrupt signal received. Exiting program."
      );

      return true;
    }

    return false;
  }

  bool Message::setDialogInput(std::string &buffer) {

    // decoration string
    std::cout << listPointStyle << ' ';

    if (Message::isInterruptedCtrlC()) return false; // stop loop

    // user input
    std::getline(std::cin, buffer);

    if (Message::isInterruptedCtrlC()) return false; // stop loop

    // loop still running
    return true;
  }

  void Message::setColor(
    CR_MESSAGE_ENUM statusCode,
    CR_CLR color_in
  ) {
    Message::colors[statusCode] = color_in;
  }
}

#endif // __CLI_MENU__MESSAGE_CPP__