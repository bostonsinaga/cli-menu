#ifndef __CLI_MENU__COLOR_CPP__
#define __CLI_MENU__COLOR_CPP__

#include "color.hpp"

namespace cli_menu {

  Color Color::set[COLOR_TOTAL] = {
    {0, 127, 255},    // AZURE
    {0, 0, 0},        // BLACK
    {0, 0, 255},      // BLUE
    {165, 42, 42},    // BROWN
    {255, 255, 153},  // CANARY
    {127, 255, 0},    // CHARTREUSE
    {210, 105, 30},   // CHOCOLATE
    {220, 20, 60},    // CRIMSON
    {0, 255, 255},    // CYAN
    {34, 139, 34},    // FOREST_GREEN
    {255, 215, 0},    // GOLD
    {128, 128, 128},  // GRAY
    {0, 255, 0},      // GREEN
    {173, 216, 230},  // LIGHT_BLUE
    {144, 238, 144},  // LIGHT_GREEN
    {191, 255, 0},    // LIME
    {255, 0, 255},    // MAGENTA
    {128, 0, 0},      // MAROON
    {62, 180, 137},   // MINT
    {0, 0, 128},      // NAVY_BLUE
    {128, 128, 0},    // OLIVE
    {255, 165, 0},    // ORANGE
    {255, 192, 203},  // PINK
    {128, 0, 128},    // PURPLE
    {255, 0, 0},      // RED
    {0, 95, 223},     // ROYAL_BLUE
    {255, 140, 105},  // SALMON
    {192, 192, 192},  // SILVER
    {135, 206, 235},  // SKY_BLUE
    {0, 128, 128},    // TEAL
    {238, 130, 238},  // VIOLET
    {100, 84, 82},    // WENGE
    {245, 222, 179},  // WHEAT
    {255, 255, 255},  // WHITE
    {255, 255, 0}     // YELLOW
  };

  const std::string
    Color::antidote = "\x1b[0m",
    Color::italic = "\x1b[3m",
    Color::underline = "\x1b[4m";

  Color::Color(
    mt::CR_SZ r_in,
    mt::CR_SZ g_in,
    mt::CR_SZ b_in
  ) {
    unset = false;
    r = r_in % 256;
    g = g_in % 256;
    b = b_in % 256;
  }

  bool Color::operator=(mt::CR<Color> comparison) const {
    return (this == &comparison || (
      unset == comparison.unset &&
      r == comparison.r &&
      g == comparison.g &&
      b == comparison.b
    ));
  }

  void Color::setUnset() {
    r = 0; g = 0; b = 0;
    unset = true;
  }

  std::string Color::correctNewlines(
    std::string &text,
    mt::CR_STR escapeCode
  ) {
    if (text.empty()) return "";

    std::string newlines[2];
    bool anyNl = false;
    const int lastIndex = text.length() - 1;

    /** Forward Newlines */

    for (int i = 0; i < lastIndex; i++) {
      if (text[i] == '\r') {
        text.erase(text.begin());
        newlines[0] += '\r';
        anyNl = true;
        i--;
      }
      else if (text[i] == '\n') {
        text.erase(text.begin());
        newlines[0] += '\n';
        anyNl = true;
        i--;
      }
      else break;
    }

    if (!anyNl) newlines[0] = "";
    else anyNl = false;

    /** Reverse Newlines */

    for (int i = lastIndex; i > 0; i--) {
      if (text[i] == '\r') {
        text.pop_back();
        newlines[1] += '\r';
        anyNl = true;
      }
      else if (text[i] == '\n') {
        text.pop_back();
        newlines[1] += '\n';
        anyNl = true;
      }
      else break;
    }

    if (!anyNl) newlines[1] = "";
    mt::VEC_STR fractions;

    // inner newline detection
    for (int i = 0; i < text.length(); i++) {
      if (text[i] == '\n' || text[i] == '\r') {
        fractions.push_back(text.substr(0, i));
        text.erase(0, i+1);
        i = 0;
      }
      /**
       * Also an antidote correction.
       * Adding parameter codes around it.
       */
      else if (i+3 < text.length() &&
        text[i] == '\x1B' &&
        text[i+1] == '[' &&
        text[i+2] == '0' &&
        text[i+3] == 'm'
      ) {
        text = text.substr(0, i+4) + escapeCode + text.substr(i+4);
        i += 3 + escapeCode.length();
      }
    }

    /**
     * Reconnect 'fractions' to 'text'
     * with corrected ANSI escape code
     * by adding parameter codes around '\n'.
     */
    if (!fractions.empty()) {
      for (int i = fractions.size() - 1; i >= 0; i--) {
        text = fractions[i] + antidote + "\n"
          + escapeCode + text;
      }
    }

    return newlines[0] + escapeCode
      + text + antidote + newlines[1];
  }

  std::string Color::getEscapeCode(
    mt::CR_STR styleEscapeCode,
    mt::CR<Color> foreground,
    mt::CR_BOL mEnd
  ) {
    std::string code = styleEscapeCode;

    if (!foreground.unset) {
      code += "\x1B[38;2;"
        + std::to_string(foreground.r) + ";"
        + std::to_string(foreground.g) + ";" 
        + std::to_string(foreground.b);

      if (mEnd) code += "m";
    }

    return code;
  }

  std::string Color::getEscapeCode(
    mt::CR_STR styleEscapeCode,
    mt::CR<Color> foreground,
    mt::CR<Color> background
  ) {
    std::string code = getEscapeCode(
      styleEscapeCode, foreground, false
    );

    if (!background.unset) {
      code += ";48;2;"
        + std::to_string(background.r) + ";"
        + std::to_string(background.g) + ";" 
        + std::to_string(background.b) + "m";
    }
    else code += "m";

    return code;
  }

  /** OPENED STRINGS */

  std::string Color::start(
    mt::CR<Color> foreground
  ) {
    return getEscapeCode("", foreground, true);
  }

  std::string Color::start(
    mt::CR<Color> foreground,
    mt::CR<Color> background
  ) {
    return getEscapeCode("", foreground, background);
  }

  std::string Color::startItalic(
    mt::CR<Color> foreground
  ) {
    return getEscapeCode(italic, foreground, true);
  }

  std::string Color::startItalic(
    mt::CR<Color> foreground,
    mt::CR<Color> background
  ) {
    return getEscapeCode(italic, foreground, background);
  }

  std::string Color::startUnderline(
    mt::CR<Color> foreground
  ) {
    return getEscapeCode(underline, foreground, true);
  }

  std::string Color::startUnderline(
    mt::CR<Color> foreground,
    mt::CR<Color> background
  ) {
    return getEscapeCode(underline, foreground, background);
  }

  /** CLOSED STRINGS */

  std::string Color::getString(
    std::string &text,
    mt::CR_STR styleEscapeCode,
    mt::CR<Color> foreground
  ) {
    return correctNewlines(
      text,
      getEscapeCode(styleEscapeCode, foreground, true)
    );
  }

  std::string Color::getString(
    std::string &text,
    mt::CR_STR styleEscapeCode,
    mt::CR<Color> foreground,
    mt::CR<Color> background
  ) {
    return correctNewlines(
      text,
      getEscapeCode(styleEscapeCode, foreground, background)
    );
  }

  std::string Color::getString(
    std::string text,
    mt::CR<Color> foreground
  ) {
    return getString(text, "", foreground);
  }

  std::string Color::getString(
    std::string text,
    mt::CR<Color> foreground,
    mt::CR<Color> background
  ) {
    return getString(text, "", foreground, background);
  }

  std::string Color::getItalicString(
    std::string text
  ) {
    return correctNewlines(text, italic);
  }

  std::string Color::getItalicString(
    std::string text,
    mt::CR<Color> foreground
  ) {
    return getString(text, italic, foreground);
  }

  std::string Color::getItalicString(
    std::string text,
    mt::CR<Color> foreground,
    mt::CR<Color> background
  ) {
    return getString(
      text, italic, foreground, background
    );
  }

  std::string Color::getUnderlineString(
    std::string text
  ) {
    return correctNewlines(text, underline);
  }

  std::string Color::getUnderlineString(
    std::string text,
    mt::CR<Color> foreground
  ) {
    return getString(text, underline, foreground);
  }

  std::string Color::getUnderlineString(
    std::string text,
    mt::CR<Color> foreground,
    mt::CR<Color> background
  ) {
    return getString(
      text, underline, foreground, background
    );
  }
}

#endif // __CLI_MENU__COLOR_CPP__