#ifndef __CLI_MENU__CONSOLE_CPP__
#define __CLI_MENU__CONSOLE_CPP__

#include "console.hpp"

namespace cli_menu {

  Color Console::messageColors[Console::totalStatus] = {
    Color::RED,
    Color::YELLOW,
    Color::WHITE,
    Color::AZURE,
    Color::GREEN,
    Color::ORANGE
  };

  Color Console::boundaryModifyColor = Color::WHITE,
    Console::boundarySelectionColor = Color::SILVER,
    Console::boxModifyColors[2] = {Color::WHITE, Color::CYAN},
    Console::boxSelectionColors[2] = {Color::WHITE, Color::MINT};

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

  CR_CLR Console::chooseBoundaryColor(mt::CR_BOL selecting) {
    return selecting ?
    Console::boundarySelectionColor : Console::boundaryModifyColor;
  }

  void Console::logBoundaryLine(mt::CR_BOL selecting) {
    Console::logString(
      "\n" + std::string(boundaryCharactersAmount, boundaryCharacter) + '\n',
      chooseBoundaryColor(selecting)
    );
  }

  void Console::logStylishHeader(
    mt::CR_STR title,
    mt::CR_BOL selecting
  ) {
    if (Console::outlineStyle) {
      Console::logBoundaryLine(selecting);

      Console::logString(
        title, chooseBoundaryColor(selecting)
      );

      Console::logBoundaryLine(selecting);
    }
    else Console::logString(
      title,
      selecting ? Console::boxSelectionColors[0] : Console::boxModifyColors[0],
      selecting ? Console::boxSelectionColors[1] : Console::boxModifyColors[1]
    );
  }

  void Console::logResponse(
    const CONSOLE_CODE &code,
    mt::CR_STR reason
  ) {
    Console::logString(
      listPointStyle + ' ' + reason + '\n',
      Console::messageColors[code]
    );
  }
}

#endif // __CLI_MENU__CONSOLE_CPP__