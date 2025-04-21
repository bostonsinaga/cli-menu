#ifndef __CLI_MENU__COLOR_H__
#define __CLI_MENU__COLOR_H__

#include "base.h"

namespace cli_menu {

  class Color;
  typedef const Color& CR_CLR;

  class Color {
  private:
    int r = 0, g = 0, b = 0;
    bool empty = true;

    static const std::string
      antidote, italic, underline;

    static std::string correctNewlines(
      std::string &text,
      mt::CR_STR escapeCode,
      mt::CR_INT forwardSpaceBoundaryIndex = -1,
      mt::CR_INT reverseSpaceBoundaryIndex = -1
    );

    static std::string getEscapeCode(
      mt::CR_STR styleEscapeCode,
      CR_CLR foreground,
      mt::CR_BOL mEnd
    );

    static std::string getEscapeCode(
      mt::CR_STR styleEscapeCode,
      CR_CLR foreground,
      CR_CLR background
    );

    static std::string getString(
      std::string &text,
      mt::CR_STR styleEscapeCode,
      CR_CLR foreground,
      mt::CR_INT forwardSpaceBoundaryIndex = -1,
      mt::CR_INT reverseSpaceBoundaryIndex = -1
    );

    static std::string getString(
      std::string &text,
      mt::CR_STR styleEscapeCode,
      CR_CLR foreground,
      CR_CLR background,
      mt::CR_INT forwardSpaceBoundaryIndex = -1,
      mt::CR_INT reverseSpaceBoundaryIndex = -1
    );

  public:
    static const Color
      AZURE, BLACK, BLUE, BROWN,
      CANARY, CHARTREUSE, CHOCOLATE, CRIMSON,
      CYAN, FOREST_GREEN, GOLD, GRAY,
      GREEN, LIGHT_BLUE, LIGHT_GREEN, LIME,
      MAGENTA, MAROON, MINT, NAVY_BLUE,
      OLIVE, ORANGE, PINK, PURPLE,
      RED, ROYAL_BLUE, SALMON, SILVER,
      SKY_BLUE, TEAL, VIOLET, WENGE,
      WHEAT, WHITE, YELLOW;

    Color(
      mt::CR_INT r_in = 0,
      mt::CR_INT g_in = 0,
      mt::CR_INT b_in = 0
    );

    const bool isEmpty() const { return empty; }

    static std::string start(
      CR_CLR foreground
    );

    static std::string start(
      CR_CLR foreground,
      CR_CLR background
    );

    static std::string startItalic(
      CR_CLR foreground
    );

    static std::string startItalic(
      CR_CLR foreground,
      CR_CLR background
    );

    static std::string startUnderline(
      CR_CLR foreground
    );

    static std::string startUnderline(
      CR_CLR foreground,
      CR_CLR background
    );

    static std::string end() { return antidote; }

    static std::string getString(
      std::string text,
      CR_CLR foreground,
      mt::CR_INT forwardSpaceBoundaryIndex = -1,
      mt::CR_INT reverseSpaceBoundaryIndex = -1
    );

    static std::string getString(
      std::string text,
      CR_CLR foreground,
      CR_CLR background,
      mt::CR_INT forwardSpaceBoundaryIndex = -1,
      mt::CR_INT reverseSpaceBoundaryIndex = -1
    );

    static std::string getItalicString(
      std::string text,
      mt::CR_INT forwardSpaceBoundaryIndex = -1,
      mt::CR_INT reverseSpaceBoundaryIndex = -1
    );

    static std::string getItalicString(
      std::string text,
      CR_CLR foreground,
      mt::CR_INT forwardSpaceBoundaryIndex = -1,
      mt::CR_INT reverseSpaceBoundaryIndex = -1
    );

    static std::string getItalicString(
      std::string text,
      CR_CLR foreground,
      CR_CLR background,
      mt::CR_INT forwardSpaceBoundaryIndex = -1,
      mt::CR_INT reverseSpaceBoundaryIndex = -1
    );

    static std::string getUnderlineString(
      std::string text,
      mt::CR_INT forwardSpaceBoundaryIndex = -1,
      mt::CR_INT reverseSpaceBoundaryIndex = -1
    );

    static std::string getUnderlineString(
      std::string text,
      CR_CLR foreground,
      mt::CR_INT forwardSpaceBoundaryIndex = -1,
      mt::CR_INT reverseSpaceBoundaryIndex = -1
    );

    static std::string getUnderlineString(
      std::string text,
      CR_CLR foreground,
      CR_CLR background,
      mt::CR_INT forwardSpaceBoundaryIndex = -1,
      mt::CR_INT reverseSpaceBoundaryIndex = -1
    );

    static bool areEqual(CR_CLR color_1, CR_CLR color_2);
    static void printPresets();
  };
}

#endif // __CLI_MENU__COLOR_H__