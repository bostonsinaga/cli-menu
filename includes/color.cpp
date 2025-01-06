#ifndef __CLI_MENU__COLOR_CPP__
#define __CLI_MENU__COLOR_CPP__

#include "color.h"

namespace cli_menu {

  const Color
    Color::AZURE(0, 127, 255),
    Color::BLACK(0, 0, 0),
    Color::BLUE(0, 0, 255),
    Color::BROWN(165, 42, 42),
    Color::CANARY(255, 255, 153),
    Color::CHARTREUSE(127, 255, 0),
    Color::CHOCOLATE(210, 105, 30),
    Color::CRIMSON(220, 20, 60),
    Color::CYAN(0, 255, 255),
    Color::FOREST_GREEN(34, 139, 34),
    Color::GOLD(255, 215, 0),
    Color::GRAY(128, 128, 128),
    Color::GREEN(0, 255, 0),
    Color::LIME(191, 255, 0),
    Color::MAGENTA(255, 0, 255),
    Color::MAROON(128, 0, 0),
    Color::MINT(62, 180, 137),
    Color::NAVY_BLUE(0, 0, 128),
    Color::OLIVE(128, 128, 0),
    Color::ORANGE(255, 165, 0),
    Color::PINK(255, 192, 203),
    Color::PURPLE(128, 0, 128),
    Color::RED(255, 0, 0),
    Color::SALMON(255, 140, 105),
    Color::SILVER(192, 192, 192),
    Color::SKY_BLUE(135, 206, 235),
    Color::TEAL(0, 128, 128),
    Color::VIOLET(238, 130, 238),
    Color::WENGE(100, 84, 82),
    Color::WHEAT(245, 222, 179),
    Color::WHITE(255, 255, 255),
    Color::YELLOW(255, 255, 0);

  const std::string
    Color::antidote = "\x1B[0m",
    Color::italic = "\x1b[3m",
    Color::underline = "\x1b[4m";

  Color::Color(
    mt::CR_INT r_in,
    mt::CR_INT g_in,
    mt::CR_INT b_in
  ) {
    empty = false;

    if (r_in < 0) r = 0;
    else r = r_in;

    if (g_in < 0) g = 0;
    else g = g_in;

    if (b_in < 0) b = 0;
    else b = b_in;
  }

  std::string Color::correctNewlines(
    std::string &text,
    mt::CR_STR escapeCode,
    mt::CR_INT forwardSpaceBoundaryIndex,
    mt::CR_INT reverseSpaceBoundaryIndex
  ) {
    if (text.empty()) return "";

    std::string newlines[2];
    bool anyNl = false;
    const int lastIndex = text.length() - 1;

    // forward newlines
    if (forwardSpaceBoundaryIndex < 0) {

      for (int i = 0; i < lastIndex; i++) {
        if (text[i] == '\n') {
          text.erase(text.begin());
          newlines[0] += "\n";
          anyNl = true;
          i--;
        }
        else break;
      }
    }
    else {
      anyNl = true;
      text.erase(0, forwardSpaceBoundaryIndex + 1);
      newlines[0] += std::string(forwardSpaceBoundaryIndex + 1, '\n');
    }

    if (!anyNl) newlines[0] = "";
    else anyNl = false;

    // reverse newlines
    if (forwardSpaceBoundaryIndex < 0 ||
      reverseSpaceBoundaryIndex > lastIndex
    ) {
      for (int i = lastIndex; i > 0; i--) {
        if (text[i] == '\n') {
          text.pop_back();
          newlines[1] += "\n";
          anyNl = true;
        }
        else break;
      }
    }
    else {
      anyNl = true;
      const int linesCt = text.length() - reverseSpaceBoundaryIndex;
      text.erase(reverseSpaceBoundaryIndex);
      newlines[1] += std::string(linesCt, '\n');
    }

    if (!anyNl) newlines[1] = "";
    mt::VEC_STR fractions;

    // inner newline detection
    for (int i = 0; i < text.length(); i++) {
      if (text[i] == '\n') {
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

  /** COLORS */

  std::string Color::getEscapeCode(
    mt::CR_STR styleEscapeCode,
    CR_CLR foreground,
    mt::CR_BOL mEnd
  ) {
    std::string code = styleEscapeCode;

    if (!foreground.empty) {
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
    CR_CLR foreground,
    CR_CLR background
  ) {
    std::string code = getEscapeCode(
      styleEscapeCode, foreground, false
    );

    if (!background.empty) {
      code += ";48;2;"
        + std::to_string(background.r) + ";"
        + std::to_string(background.g) + ";" 
        + std::to_string(background.b) + "m";
    }
    else code += "m";

    return code;
  }

  std::string Color::start(
    CR_CLR foreground
  ) {
    return getEscapeCode("", foreground, true);
  }

  std::string Color::start(
    CR_CLR foreground,
    CR_CLR background
  ) {
    return getEscapeCode("", foreground, background);
  }

  std::string Color::startItalic(
    CR_CLR foreground
  ) {
    return getEscapeCode(italic, foreground, true);
  }

  std::string Color::startItalic(
    CR_CLR foreground,
    CR_CLR background
  ) {
    return getEscapeCode(italic, foreground, background);
  }

  std::string Color::startUnderline(
    CR_CLR foreground
  ) {
    return getEscapeCode(underline, foreground, true);
  }

  std::string Color::startUnderline(
    CR_CLR foreground,
    CR_CLR background
  ) {
    return getEscapeCode(underline, foreground, background);
  }

  /** STRINGS */

  std::string Color::getString(
    std::string &text,
    mt::CR_STR styleEscapeCode,
    CR_CLR foreground,
    mt::CR_INT forwardSpaceBoundaryIndex,
    mt::CR_INT reverseSpaceBoundaryIndex
  ) {
    return correctNewlines(
      text,
      getEscapeCode(styleEscapeCode, foreground, true),
      forwardSpaceBoundaryIndex,
      reverseSpaceBoundaryIndex
    );
  }

  std::string Color::getString(
    std::string &text,
    mt::CR_STR styleEscapeCode,
    CR_CLR foreground,
    CR_CLR background,
    mt::CR_INT forwardSpaceBoundaryIndex,
    mt::CR_INT reverseSpaceBoundaryIndex
  ) {
    return correctNewlines(
      text,
      getEscapeCode(styleEscapeCode, foreground, background),
      forwardSpaceBoundaryIndex,
      reverseSpaceBoundaryIndex
    );
  }

  std::string Color::getString(
    std::string text,
    CR_CLR foreground,
    mt::CR_INT forwardSpaceBoundaryIndex,
    mt::CR_INT reverseSpaceBoundaryIndex
  ) {
    return getString(
      text, "", foreground,
      forwardSpaceBoundaryIndex,
      reverseSpaceBoundaryIndex
    );
  }

  std::string Color::getString(
    std::string text,
    CR_CLR foreground,
    CR_CLR background,
    mt::CR_INT forwardSpaceBoundaryIndex,
    mt::CR_INT reverseSpaceBoundaryIndex
  ) {
    return getString(
      text, "", foreground, background,
      forwardSpaceBoundaryIndex,
      reverseSpaceBoundaryIndex
    );
  }

  std::string Color::getItalicString(
    std::string text,
    mt::CR_INT forwardSpaceBoundaryIndex,
    mt::CR_INT reverseSpaceBoundaryIndex
  ) {
    return correctNewlines(
      text, italic,
      forwardSpaceBoundaryIndex,
      reverseSpaceBoundaryIndex
    );
  }

  std::string Color::getItalicString(
    std::string text,
    CR_CLR foreground,
    mt::CR_INT forwardSpaceBoundaryIndex,
    mt::CR_INT reverseSpaceBoundaryIndex
  ) {
    return getString(
      text, italic, foreground,
      forwardSpaceBoundaryIndex,
      reverseSpaceBoundaryIndex
    );
  }

  std::string Color::getItalicString(
    std::string text,
    CR_CLR foreground,
    CR_CLR background,
    mt::CR_INT forwardSpaceBoundaryIndex,
    mt::CR_INT reverseSpaceBoundaryIndex
  ) {
    return getString(
      text, italic, foreground, background,
      forwardSpaceBoundaryIndex,
      reverseSpaceBoundaryIndex
    );
  }

  std::string Color::getUnderlineString(
    std::string text,
    mt::CR_INT forwardSpaceBoundaryIndex,
    mt::CR_INT reverseSpaceBoundaryIndex
  ) {
    return correctNewlines(
      text, underline,
      forwardSpaceBoundaryIndex,
      reverseSpaceBoundaryIndex
    );
  }

  std::string Color::getUnderlineString(
    std::string text,
    CR_CLR foreground,
    mt::CR_INT forwardSpaceBoundaryIndex,
    mt::CR_INT reverseSpaceBoundaryIndex
  ) {
    return getString(
      text, underline, foreground,
      forwardSpaceBoundaryIndex,
      reverseSpaceBoundaryIndex
    );
  }

  std::string Color::getUnderlineString(
    std::string text,
    CR_CLR foreground,
    CR_CLR background,
    mt::CR_INT forwardSpaceBoundaryIndex,
    mt::CR_INT reverseSpaceBoundaryIndex
  ) {
    return getString(
      text, underline, foreground, background,
      forwardSpaceBoundaryIndex,
      reverseSpaceBoundaryIndex
    );
  }

  /** UTILS */

  bool Color::areEqual(CR_CLR color_1, CR_CLR color_2) {
    if (&color_1 == &color_2 || (
      color_1.r == color_2.r &&
      color_1.g == color_2.g && 
      color_1.b == color_2.b
    )) { return true; }
    return false;
  }

  void Color::printPresets() {
    std::cout
      << Color::getUnderlineString("PRESET COLORS:\n", Color::CHOCOLATE, Color::SKY_BLUE)
      << Color::getString(" azure        \n", Color::AZURE, Color::WHITE)
      << Color::getString(" black        \n", Color::BLACK, Color::WHITE)
      << Color::getString(" blue         \n", Color::BLUE, Color::WHITE)
      << Color::getString(" brown        \n", Color::BROWN, Color::WHITE)
      << Color::getString(" canary       \n", Color::CANARY, Color::GRAY)
      << Color::getString(" chartreuse   \n", Color::CHARTREUSE, Color::GRAY)
      << Color::getString(" chocolate    \n", Color::CHOCOLATE, Color::WHITE)
      << Color::getString(" crimson      \n", Color::CRIMSON, Color::WHITE)
      << Color::getString(" cyan         \n", Color::CYAN, Color::WHITE)
      << Color::getString(" forest_green \n", Color::FOREST_GREEN, Color::WHITE)
      << Color::getString(" gold         \n", Color::GOLD, Color::WHITE)
      << Color::getString(" gray         \n", Color::GRAY, Color::WHITE)
      << Color::getString(" green        \n", Color::GREEN, Color::WHITE)
      << Color::getString(" lime         \n", Color::LIME, Color::GRAY)
      << Color::getString(" magenta      \n", Color::MAGENTA, Color::WHITE)
      << Color::getString(" maroon       \n", Color::MAROON, Color::WHITE)
      << Color::getString(" mint         \n", Color::MINT, Color::WHITE)
      << Color::getString(" navy_blue    \n", Color::NAVY_BLUE, Color::WHITE)
      << Color::getString(" olive        \n", Color::OLIVE, Color::WHITE)
      << Color::getString(" orange       \n", Color::ORANGE, Color::WHITE)
      << Color::getString(" pink         \n", Color::PINK, Color::GRAY)
      << Color::getString(" purple       \n", Color::PURPLE, Color::WHITE)
      << Color::getString(" red          \n", Color::RED, Color::WHITE)
      << Color::getString(" salmon       \n", Color::SALMON, Color::WHITE)
      << Color::getString(" silver       \n", Color::SILVER, Color::GRAY)
      << Color::getString(" sky_blue     \n", Color::SKY_BLUE, Color::GRAY)
      << Color::getString(" teal         \n", Color::TEAL, Color::WHITE)
      << Color::getString(" violet       \n", Color::VIOLET, Color::GRAY)
      << Color::getString(" wenge        \n", Color::WENGE, Color::WHITE)
      << Color::getString(" wheat        \n", Color::WHEAT, Color::GRAY)
      << Color::getString(" white        \n", Color::WHITE, Color::GRAY)
      << Color::getString(" yellow       \n", Color::YELLOW, Color::WHITE);
  }
}

#endif // __CLI_MENU__COLOR_CPP__