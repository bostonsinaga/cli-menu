#ifndef __CLI_MENU__CONSOLE_CPP__
#define __CLI_MENU__CONSOLE_CPP__

#include "console.hpp"

namespace cli_menu {

  Color Console::messageColors[Console::totalStatus] = {
    Color::RED,
    Color::YELLOW,
    Color::BLUE,
    Color::SKY_BLUE,
    Color::AZURE,
    Color::GREEN,
    Color::ORANGE
  };

  Color Console::boundaryModifyColor = Color::WHITE,
    Console::boundarySelectionColor = Color::SILVER,
    Console::boxModifyColors[2] = {Color::WHITE, Color::MINT},
    Console::boxSelectionColors[2] = {Color::WHITE, Color::TEAL};

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

  CR_CLR Console::chooseBoundaryColor(mt::CR_BOL editing) {
    return editing ?
    Console::boundaryModifyColor : Console::boundarySelectionColor;
  }

  void Console::logBoundaryLine(mt::CR_BOL editing) {
    Console::logString(
      "\n" + std::string(boundaryCharactersAmount, boundaryCharacter) + '\n',
      chooseBoundaryColor(editing)
    );
  }

  void Console::logStylishHeader(
    mt::CR_STR title,
    mt::CR_BOL editing
  ) {
    if (Console::outlineStyle) {
      Console::logBoundaryLine(editing);

      Console::logString(
        title, chooseBoundaryColor(editing)
      );

      Console::logBoundaryLine(editing);
    }
    // filled style
    else {
      std::string fillerSpaces;

      if (title.length() < boundaryCharactersAmount) {
        fillerSpaces = std::string(boundaryCharactersAmount - title.length() - 1, ' ');
      }

      Console::logString(
        ' ' + title + fillerSpaces + '\n',
        editing ? Console::boxModifyColors[0] : Console::boxSelectionColors[0],
        editing ? Console::boxModifyColors[1] : Console::boxSelectionColors[1]
      );
    }
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