#ifndef __CLI_MENU__CONSOLE_CPP__
#define __CLI_MENU__CONSOLE_CPP__

#include "console.hpp"

namespace cli_menu {

  Color Console::messageColors[Console::totalStatus] = {
    Color(),              // CONLOR_PLAIN
    Color::set[RED],      // CONLOR_ERROR
    Color::set[YELLOW],   // CONLOR_WARNING
    Color::set[CYAN],     // CONLOR_HINT
    Color::set[SKY_BLUE], // CONLOR_SHALLOW
    Color::set[AZURE],    // CONLOR_DEEP
    Color::set[PINK],     // CONLOR_HIGHLIGHT
    Color::set[GREEN],    // CONLOR_CORRECT
    Color::set[ORANGE]    // CONLOR_CANCEL
  };

  Color Console::boundaryModifyColor = Color::set[WHITE],
    Console::boundarySelectionColor = Color::set[SILVER],
    Console::boxModifyColors[2] = {Color::set[WHITE], Color::set[MINT]},
    Console::boxSelectionColors[2] = {Color::set[WHITE], Color::set[TEAL]};

  /** NORMAL */

  void Console::logString(
    mt::CR_STR text,
    mt::CR<Color> foreground
  ) {
    std::cout << Color::getString(
      text, foreground
    );
  }

  void Console::logString(
    mt::CR_STR text,
    mt::CR<Color> foreground,
    mt::CR<Color> background
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
    mt::CR<Color> foreground
  ) {
    std::cout << Color::getItalicString(
      text, foreground
    );
  }

  void Console::logItalicString(
    mt::CR_STR text,
    mt::CR<Color> foreground,
    mt::CR<Color> background
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
    mt::CR<Color> foreground
  ) {
    std::cout << Color::getUnderlineString(
      text, foreground
    );
  }

  void Console::logUnderlineString(
    mt::CR_STR text,
    mt::CR<Color> foreground,
    mt::CR<Color> background
  ) {
    std::cout << Color::getUnderlineString(
      text, foreground, background
    );
  }

  /** SPECIALS */

  mt::CR<Color> Console::chooseBoundaryColor(mt::CR_BOL editing) {
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
    const CONLOR_CODE &code,
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

  std::string Console::LimitedText::wrap(mt::CR_STR text) {

    if (wrapping) {
      mt::VEC_STR wrapped {""},
        sliced = mt_uti::StrTool::whitespaceSlice(text);

      for (mt::CR_STR str : sliced) {

        if (wrapped.back().length() + str.length() > maxCharacters) {
          wrapped.push_back("");
        }

        wrapped.back() += str + ' ';
      }

      return mt_uti::StrTool::joinVector(wrapped, "\n", false);
    }

    return text;
  }
}

#endif // __CLI_MENU__CONSOLE_CPP__