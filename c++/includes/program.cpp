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
    CR_VERSION version_in,
    CR_SP_CALLBACK callback_in
  ):
  Command(name_in, description_in, callback_in, nullptr, true) {
    author = "Anonymous";
    version = version_in;
  }

  Program::Program(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_STR author_in,
    CR_VERSION version_in,
    CR_SP_PLAIN_CALLBACK callback_in
  ):
  Command(name_in, description_in, callback_in, nullptr, true) {
    author = "Anonymous";
    version = version_in;
  }

  Program::Program(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_STR author_in,
    CR_VERSION version_in
  ):
  Command(name_in, description_in, nullptr, true) {
    author = "Anonymous";
    version = version_in;
  }

  Program::~Program() {
    author = "";
    version.clean();
  }

  void Program::printHelp() {
    std::cout
      << "Welcome to '" << mt_uti::StrTools::getStringToUppercase(getName())
      << "'\n\nversion: " << getVersion().stringify()
      << "\nauthor: " << getAuthor();

    Message::printBoundaryLine();

    std::cout
      << "HINT:\n"
      << "* Use '-' for parameter and '--' for toggle\n"
      << "* The <req> or <opt> are only available in parameter (text/number)\n"
      << "* The toggle (boolean) always optional, except for main command and groups\n"
      << "* Please type '--[command] --help' to view detailed information";

    Message::printBoundaryLine();
    std::cout << getBranchLeafString(1, true);
  }

  void Program::printError() {
    std::cerr << "Program Error..";
  }
}

#endif // __CLI_MENU__PROGRAM_CPP__