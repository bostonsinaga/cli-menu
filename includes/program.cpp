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

  Program::Program(): Toggle() {
    author = "anonymous";
  }

  Program::Program(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_STR author_in,
    CR_VERSION version_in,
    CR_SP_CALLBACK callback_in,
    mt::CR_BOL propagatingCallback_in
  ) : Toggle(
    name_in, description_in, true, nullptr,
    callback_in, propagatingCallback_in
  ) {
    author = author_in;
    version = version_in;
  }

  Program::Program(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_STR author_in,
    CR_VERSION version_in,
    CR_SP_PLAIN_CALLBACK callback_in,
    mt::CR_BOL propagatingCallback_in
  ) : Toggle(
    name_in, description_in, true, nullptr,
    callback_in, propagatingCallback_in
  ) {
    author = author_in;
    version = version_in;
  }

  Program::Program(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_STR author_in,
    CR_VERSION version_in
  ) : Toggle(
    name_in, description_in, true, nullptr
  ) {
    author = author_in;
    version = version_in;
  }

  std::string Program::getDashedName() {
    if (disguised) return Toggle::getDashedName();
    return Command::getDashedName();
  }

  std::string Program::getFullName(
    CR_FullNameProfile fullNameProfile
  ) {
    if (disguised) return Toggle::getFullName(fullNameProfile);
    return Command::getFullName(fullNameProfile);
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
      static const std::string text_1 = (
        Color::getString(
          "Welcome to '" + mt_uti::StrTools::getStringToUppercase(name),
          Color::WHITE
        ) + "'\n\n" + description
        + "\n\nversion: " + version.stringify()
        + "\nauthor: " + author
      );

      static const std::string text_2 = (
        Color::getString("\nHINTS:\n\n", Color::WHITE)
        + "* Use '-' for parameter and '--' for toggle\n"
        + "* The <req> or <opt> are only available in parameter (text/number)\n"
        + "* The toggle (boolean) always optional, except for main command and groups\n"
        + "* Please type '--[command] --help' to view detailed information"
      );

      Message::printBoundaryLine(1, 1);
      std::cout << text_1;
      Message::printBoundaryLine(1, 0);
      std::cout << text_2;

      printChildrenNamesDescriptions();
    }
  }

  void Program::printError() {
    std::cout << "Program Error..";
  }

  void Program::run(
    mt::CR_INT argc,
    char *argv[],
    mt::CR_BOL completingDialog
  ) {
    // in case if this added to children of 'Command'
    if (parent) {
      remove();
      return;
    }

    //______|
    // Init |
    //______|

    mt::VEC_STR inputs;
    ParamData paramData;
    Command::dialogued = completingDialog;

    /**
     * Skip the first one because its format is unpredictable
     * to compare with program name e.g. "./bin/program.exe".
     */
    for (int i = 1; i < argc; i++) {
      inputs.push_back(argv[i]);
    }

    // will get 'pop_back' in 'Command::match'
    std::reverse(inputs.begin(), inputs.end());

    if (Command::usingLowercaseName) {
      changeTreeNamesToLowercase();
    }
    else if (Command::usingUppercaseName) {
      changeTreeNamesToUppercase();
    }

    //_________|
    // Process |
    //_________|

    Command *lastCom = this;
    mt::USI flag;

    if (children.size() > 0) {

      flag = matchTo(
        static_cast<Cm*>(children.front()),
        inputs, paramData, &lastCom
      );

      switch (flag) {
        case FLAG::COMPLETED: {
          // check if has any callback
          if (!runTo(lastCom, paramData)) {
            Message::printNamed(
              Message::STATUS::ERROR,
              "No callback.",
              lastCom->getName()
            );
          }
          // succeeded after main callback
          else Message::printNamed(
            Message::STATUS::SUCCEED,
            "Output file written to 'foo.kml'.",
            name
          );
        break;}
        case FLAG::CANCELED: {
          Message::printNamed(
            Message::STATUS::CANCELED,
            "Input discarded.",
            name
          );
        break;}
        case FLAG::FAILED: {
          // program
          if (lastCom == this) {
            if (!runTo(lastCom, paramData)) {
              printError();
            }
          }
          // dependence
          else if (lastCom->isDependence()) {
            lastCom->getUltimate()->printError();
          }
          // group
          else if (!(lastCom->isUltimate() ||
            runTo(lastCom, paramData)
          ) || lastCom->isUltimate()) {
            lastCom->printError();
          }
        break;}
        // error
        default: {
          Message::printNamed(
            Message::STATUS::ERROR,
            "There should be an error handler.",
            name
          );
        }
      }
    }
    // no children
    else Message::printNamed(
      Message::STATUS::ERROR,
      "No groups/commands available.",
      name
    );

    remove(); // runtime end
  }
}

#endif // __CLI_MENU__PROGRAM_CPP__