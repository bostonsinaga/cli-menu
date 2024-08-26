#ifndef __CLI_MENU__PROGRAM_CPP__
#define __CLI_MENU__PROGRAM_CPP__

#include "program.h"

namespace cli_menu {

  Version::Version(
    mt::CR_UI major_in,
    mt::CR_UI minor_in,
    mt::CR_UI patch_in
  ) {
    major = major_in;
    minor = minor_in;
    patch = patch_in;
  }

  std::string Version::stringify() {
    return std::to_string(major) + "."
      + std::to_string(minor) + "."
      + std::to_string(patch);
  }

  void Version::clean() {
    major = 0;
    minor = 0;
    patch = 0;
  }

  Program::Program(): Command() {
    author = "Anonymous";
  }

  Program::Program(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_STR author_in,
    const Version &version_in,
    CR_SP_CALLBACK callback_in
  ):
  Command(name_in, description_in, callback_in, true) {
    if (author.empty()) author = "Anonymous";
    version = version_in;
  }

  Program::~Program() {
    author = "";
    version.clean();
  }
}

#endif // __CLI_MENU__PROGRAM_CPP__