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

  std::string Color::newlines[2] = {"", ""};

  void Color::updateNewlines(
    std::string &text
  ) {
    if (text.empty()) return;
    bool anyNl = false;

    for (int i = 0; i < text.size() - 1; i++) {
      if (text[i] == '\n') {
        text.erase(text.begin());
        newlines[0] += "\n";
        anyNl = true;
        i--;
      }
      else break;
    }

    if (!anyNl) newlines[0] = "";
    else anyNl = false;

    for (int i = text.size() - 1; i > 0; i--) {
      if (text[i] == '\n') {
        text.erase(text.begin() + text.size() - 1);
        newlines[1] += "\n";
        anyNl = true;
      }
      else break;
    }

    if (!anyNl) newlines[1] = "";
  }

  std::string Color::getString(
    std::string &text,
    mt::CR_STR additionalEscapeCode
  ) {
    updateNewlines(text);
    return newlines[0] + additionalEscapeCode
      + text + (additionalEscapeCode.empty() ? "" : "\x1B[0m")
      + newlines[1];
  }

  std::string Color::getString(
    std::string &text,
    CR_CLR foreground,
    mt::CR_STR additionalEscapeCode
  ) {
    updateNewlines(text);
    return newlines[0]
      + additionalEscapeCode + "\x1B[38;2;"
      + std::to_string(foreground.r) + ";"
      + std::to_string(foreground.g) + ";" 
      + std::to_string(foreground.b) + "m"
      + text + "\x1B[0m" + (additionalEscapeCode.empty() ? "" : "\x1B[0m")
      + newlines[0];
  }

  std::string Color::getString(
    std::string &text,
    CR_CLR foreground,
    CR_CLR background,
    mt::CR_STR additionalEscapeCode
  ) {
    updateNewlines(text);
    return newlines[0]
      + additionalEscapeCode + "\x1B[38;2;"
      + std::to_string(foreground.r) + ";"
      + std::to_string(foreground.g) + ";" 
      + std::to_string(foreground.b) + ";48;2;"
      + std::to_string(background.r) + ";"
      + std::to_string(background.g) + ";" 
      + std::to_string(background.b) + "m"
      + text + "\x1B[0m" + (additionalEscapeCode.empty() ? "" : "\x1B[0m")
      + newlines[1];
  }

  std::string Color::getString(
    std::string text,
    CR_CLR foreground
  ) {
    return getString(text, foreground, "");
  }

  std::string Color::getString(
    std::string text,
    CR_CLR foreground,
    CR_CLR background
  ) {
    return getString(text, foreground, background, "");
  }

  std::string Color::getUnderlineString(
    std::string text
  ) {
    return getString(text, "\x1b[4m");
  }

  std::string Color::getUnderlineString(
    std::string text,
    CR_CLR foreground
  ) {
    return getString(text, foreground, "\x1b[4m");
  }

  std::string Color::getUnderlineString(
    std::string text,
    CR_CLR foreground,
    CR_CLR background
  ) {
    return getString(text, foreground, background, "\x1b[4m");
  }

  std::string Color::getItalicString(
    std::string text
  ) {
    return getString(text, "\x1b[0m");
  }

  std::string Color::getItalicString(
    std::string text,
    CR_CLR foreground
  ) {
    return getString(text, foreground, "\x1b[3m");
  }

  std::string Color::getItalicString(
    std::string text,
    CR_CLR foreground,
    CR_CLR background
  ) {
    return getString(text, foreground, background, "\x1b[3m");
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