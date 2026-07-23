#ifndef __CLI_MENU__COLOR_HPP__
#define __CLI_MENU__COLOR_HPP__

#include "mini-tools.hpp"

namespace cli_menu {

  enum COLOR_CODE {
    AZURE, BLACK, BLUE, BROWN, CANARY,
    CHARTREUSE, CHOCOLATE, CRIMSON, CYAN, FOREST_GREEN,
    GOLD, GRAY, GREEN, LIGHT_BLUE, LIGHT_GREEN,
    LIME, MAGENTA, MAROON, MINT, NAVY_BLUE,
    OLIVE, ORANGE, PINK, PURPLE, RED,
    ROYAL_BLUE, SALMON, SILVER, SKY_BLUE, TEAL,
    VIOLET, WENGE, WHEAT, WHITE, YELLOW
  };

  constexpr int COLOR_TOTAL = 35;

  class Color;
  typedef const Color& CR_CLR;

  class Color {
  private:
    size_t r = 0, g = 0, b = 0;
    bool unset = true;

    static const std::string
      antidote, italic, underline;

    static std::string correctNewlines(
      std::string &text,
      mt::CR_STR escapeCode
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
      CR_CLR foreground
    );

    static std::string getString(
      std::string &text,
      mt::CR_STR styleEscapeCode,
      CR_CLR foreground,
      CR_CLR background
    );

  public:
    static Color set[COLOR_TOTAL];

    Color(
      mt::CR_SZ r_in,
      mt::CR_SZ g_in,
      mt::CR_SZ b_in
    );

    bool operator=(CR_CLR comparison) const;
    const bool isUnset() const { return unset; }
    void setUnset();

    /** GET RGB VALUE */

    inline size_t getR() const { return r; }
    inline size_t getG() const { return g; }
    inline size_t getB() const { return b; }

    /** OPENED STRINGS */

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

    /** CLOSED STRINGS */

    static std::string getString(
      std::string text,
      CR_CLR foreground
    );

    static std::string getString(
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
  };
}

#endif // __CLI_MENU__COLOR_HPP__