#ifndef __CLI_MENU__COLOR_H__
#define __CLI_MENU__COLOR_H__

#include "base.h"

namespace cli_menu {

  class Color;
  typedef const Color& CR_CLR;

  class Color {
  private:
    int r = 0, g = 0, b = 0;

    static const std::string
      antidote, italic, underline;

    static std::string correctNewlines(
      std::string &text,
      mt::CR_STR styleEscapeCode,
      mt::CR_STR colorEscapeCode
    );

    static std::string getString(
      std::string &text,
      mt::CR_STR styleEscapeCode,
      CR_CLR foreground,
      CR_CLR background
    );

    static std::string getString(
      std::string &text,
      mt::CR_STR styleEscapeCode,
      CR_CLR foreground
    );

  public:
    static const Color
      AZURE, BLACK, BLUE, BROWN,
      CANARY, CHARTREUSE, CHOCOLATE, CRIMSON,
      CYAN, FOREST_GREEN, GOLD, GRAY,
      GREEN, LIME, MAGENTA, MAROON,
      MINT, NAVY_BLUE, OLIVE, ORANGE,
      PINK, PURPLE, RED, SALMON,
      SILVER, SKY_BLUE, TEAL, VIOLET,
      WENGE, WHEAT, WHITE, YELLOW;

    Color(
      mt::CR_INT r_in = 0,
      mt::CR_INT g_in = 0,
      mt::CR_INT b_in = 0
    ) {
      if (r_in < 0) r = 0;
      else r = r_in;

      if (g_in < 0) g = 0;
      else g = g_in;

      if (b_in < 0) b = 0;
      else b = b_in;
    }

    static std::string getString(
      std::string text,
      CR_CLR foreground
    );

    static std::string getString(
      std::string text,
      CR_CLR foreground,
      CR_CLR background
    );

    static std::string getUnderlineString(
      std::string text
    );

    static std::string getUnderlineString(
      std::string text,
      CR_CLR foreground
    );

    static std::string getUnderlineString(
      std::string text,
      CR_CLR foreground,
      CR_CLR background
    );

    static std::string getItalicString(
      std::string text
    );

    static std::string getItalicString(
      std::string text,
      CR_CLR foreground
    );

    static std::string getItalicString(
      std::string text,
      CR_CLR foreground,
      CR_CLR background
    );

    static void printPresets();
  };
}

#endif // __CLI_MENU__COLOR_H__