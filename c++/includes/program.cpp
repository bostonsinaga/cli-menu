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

  Program::Program(): Toggle() {
    author = "Anonymous";
  }

  Program::Program(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_STR author_in,
    CR_VERSION version_in,
    CR_SP_CALLBACK callback_in
  ):
  Toggle(name_in, description_in, callback_in, nullptr, true) {
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
  Toggle(name_in, description_in, callback_in, nullptr, true) {
    author = "Anonymous";
    version = version_in;
  }

  Program::Program(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_STR author_in,
    CR_VERSION version_in
  ):
  Toggle(name_in, description_in, nullptr, true) {
    author = "Anonymous";
    version = version_in;
  }

  Program::~Program() {
    author = "";
    version.clean();
  }

  std::string Program::getDashedName() {
    if (disguised) return Toggle::getDashedName();
    return Command::getDashedName();
  }

  std::string Program::getFullName() {
    if (disguised) return Toggle::getFullName();
    return Command::getFullName();
  }

  void Program::useCaseSensitiveName(mt::CR_BOL isIt) {
    Command::usingCaseSensitiveName = isIt;
  }

  void Program::useLowercaseName(mt::CR_BOL isIt) {
    Command::usingLowercaseName = isIt;
    if (isIt) Command::usingUppercaseName = false;
  }

  void Program::useUppercaseName(mt::CR_BOL isIt) {
    Command::usingUppercaseName = isIt;
    if (isIt) Command::usingLowercaseName = false;
  }

  void Program::useDashesBoundaryLine(mt::CR_BOL isIt) {
    Command::usingDashesBoundaryLine = isIt;
  }

  void Program::printHelp() {
    if (disguised) {
      Toggle::printHelp();
    }
    else {
      std::cout
        << "Welcome to '" << mt_uti::StrTools::getStringToUppercase(name)
        << "'\n\nversion: " << version.stringify()
        << "\nauthor: " << author;

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
  }

  void Program::printError() {
    std::cerr << "Program Error..";
  }
}

#endif // __CLI_MENU__PROGRAM_CPP__