#ifndef __CLI_MENU__CONSOLE_CPP__
#define __CLI_MENU__CONSOLE_CPP__

#include "console.hpp"

namespace cli_menu {

  Color Console::messageColors[Console::totalStatus] = {
    Color::set[RED],
    Color::set[YELLOW],
    Color::set[CYAN],
    Color::set[SKY_BLUE],
    Color::set[AZURE],
    Color::set[GREEN],
    Color::set[ORANGE]
  };

  Color Console::boundaryModifyColor = Color::set[WHITE],
    Console::boundarySelectionColor = Color::set[SILVER],
    Console::boxModifyColors[2] = {Color::set[WHITE], Color::set[MINT]},
    Console::boxSelectionColors[2] = {Color::set[WHITE], Color::set[TEAL]};

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
    return editing ? boundaryModifyColor : boundarySelectionColor;
  }

  void Console::logBoundaryLine(mt::CR_BOL editing) {
    logString(
      std::string(boundaryLineLength, boundaryCharacter) + '\n',
      chooseBoundaryColor(editing)
    );
  }

  void Console::logStylishHeader(
    mt::CR_STR title,
    mt::CR_BOL editing
  ) {
    if (outlineStyle) {
      logBoundaryLine(editing);
      logString(title + '\n', chooseBoundaryColor(editing));
      logBoundaryLine(editing);
    }
    // filled style
    else {
      std::string fillerSpaces;

      if (title.length() < boundaryLineLength) {
        fillerSpaces = std::string(boundaryLineLength - title.length() - 1, ' ');
      }

      logString(
        ' ' + title + fillerSpaces + '\n',
        editing ? boxModifyColors[0] : boxSelectionColors[0],
        editing ? boxModifyColors[1] : boxSelectionColors[1]
      );
    }
  }

  void Console::logResponse(
    const CONSOLE_CODE &code,
    mt::CR_STR reason
  ) {
    logString(
      listPointStyle + ' ' + reason + '\n',
      messageColors[code]
    );
  }

  std::string Console::LimitedText::trim(
    mt::CR_STR text,
    mt::CR_BOL withTailDots,
    mt::CR_BOL withRemainingBrackets
  ) {
    int remainingLength = text.length() - maxCharacters;

    if (remainingLength > 0) {
      return text.substr(0, maxCharacters)
        + (withTailDots ? tailDots : "")
        + (withRemainingBrackets ? (remainingBrackets[0]
        + std::to_string(remainingLength)
        + remainingBrackets[1]) : "");
    }

    return text;
  }
}

#endif // __CLI_MENU__CONSOLE_CPP__