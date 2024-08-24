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

  Version::~Version() {
    major = 0;
    minor = 0;
    patch = 0;
  }

  std::string Version::stringify() {
    return std::to_string(version.major) + "."
      + std::to_string(version.minor) + "."
      + std::to_string(version.patch);
  }

  Program::Program(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_STR author_in,
    const Version &version_in,
    const std::shared_ptr<CALLBACK> &callback_in
  ):
  Command(name_in, description_in, callback_in, true) {
    version = version_in;
  }

  Program::~Program() {
    author = "";
    version->clean();
  }
}

#endif // __CLI_MENU__PROGRAM_CPP__