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

  void Program::printError() {
    std::cerr << "Program Error..";
  }

  void Program::disguise(mt::CR_BOL replaced) {

    if (incarnation) {
      if (replaced) incarnation->remove();
      else return;
    }

    if (callback) {
      incarnation = new Toggle(
        name,
        description,
        callback,
        holder,
        required
      );
    }
    else if (plainCallback) {
      incarnation = new Toggle(
        name,
        description,
        plainCallback,
        holder,
        required
      );
    }
    else incarnation = new Toggle(
      name,
      description,
      holder,
      required
    );

    incarnation->setItemsRelease(items);
  }

  void Program::undisguise() {

    if (!incarnation) return;
    VEC_COM released = incarnation->releaseItems();

    /**
     * The remaining 'released' contains items
     * that already existed in 'items' before 'disguise' invocation.
     */
    addItems(
      mt_uti::VecTools<Command*>::cutInterval(
        released, items.size(), incarnation->getNumberOfItems() - 1
      ),
      false
    );

    incarnation->remove();
    incarnation = nullptr;
  }
}

#endif // __CLI_MENU__PROGRAM_CPP__