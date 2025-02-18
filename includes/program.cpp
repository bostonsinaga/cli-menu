#ifndef __CLI_MENU__PROGRAM_CPP__
#define __CLI_MENU__PROGRAM_CPP__

#include "program.h"
#include <csignal>

namespace cli_menu {

  void ctrlCSignalHandler(int) {
    Message::INTERRUPTED_CTRL_C.store(true);
  }

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
    mt::CR_BOL accumulating_in,
    CM_CALLBACK callback_in,
    mt::CR_BOL propagatingCallback_in
  ) : Toggle(
    name_in, description_in, true, nullptr,
    accumulating_in, callback_in, propagatingCallback_in
  ) {
    author = author_in;
    version = version_in;
  }

  Program::Program(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_STR author_in,
    CR_VERSION version_in,
    mt::CR_BOL accumulating_in
  ) : Toggle(
    name_in, description_in, true, nullptr, accumulating_in
  ) {
    author = author_in;
    version = version_in;
  }

  std::string Program::getDashedName() {
    if (disguised) return Toggle::getDashedName();
    return Command::getDashedName();
  }

  std::string Program::getFullName(
    mt::CR_BOL useDash,
    mt::CR_BOL useInputType,
    mt::CR_BOL useLevelName,
    CR_CLR nameColor
  ) {
    if (disguised) return Toggle::getFullName(
      useDash,
      useInputType,
      useLevelName,
      nameColor
    );

    return Command::getFullName(
      useDash,
      useInputType,
      useLevelName,
      nameColor
    );
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
          Color::SKY_BLUE
        ) + "'\n\n" + description
        + "\n\nversion: " + version.stringify()
        + "\nauthor: " + author
      );

      static const std::string text_2 = (
        Color::getString("\nHINTS:\n\n", Color::SKY_BLUE)
        + "* Use '-' for parameter and '--' for toggle\n"
        + "* Please type '--[command] --help' to view detailed information"
      );

      Message::printBoundaryLine(1, 1);
      std::cout << text_1;
      Message::printBoundaryLine(1, 0);
      std::cout << text_2;

      printChildrenNamesDescriptions(false);
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

    /**
     * Assigned to true will open
     * the dialog after the match.
     */
    Command::dialogued = completingDialog;

    /**
     * Skip the first one because its format is unpredictable
     * to compare with program name e.g. "./bin/program.exe".
     */
    for (int i = 1; i < argc; i++) {
      directInputs.push_back(argv[i]);
    }

    // will get 'pop_back' in 'Command::match'
    std::reverse(directInputs.begin(), directInputs.end());

    // add the program name instead as it can be compared
    directInputs.push_back("--" + name);

    // transform letters case
    if (Command::usingLowercaseName) {
      changeTreeNamesToLowercase();
    }
    else if (Command::usingUppercaseName) {
      changeTreeNamesToUppercase();
    }

    // register signal handler for Ctrl+C (SIGINT)
    std::signal(SIGINT, ctrlCSignalHandler);

    //_____________________|
    // Chain to Completion |
    //_____________________|

    if (children.size() > 0) {
      COMMAND_ENUM enumeration = matchTo(this);

      switch (enumeration) {
        case COMMAND_COMPLETED: {
          // check if has any callback
          if (!executeTo(Command::lastCom)) {
            Message::printNeatNamed(
              MESSAGE_ERROR,
              "no callback",
              Command::lastCom->getName()
            );
          }
          // succeeded after main callback
          else Message::printNeatNamed(
            MESSAGE_SUCCEED,
            "output file written to 'foo.kml'",
            name
          );
        break;}
        case COMMAND_CANCELED: {
          Message::printNeatNamed(
            MESSAGE_CANCELED,
            "program terminated",
            name
          );
        break;}
        case COMMAND_FAILED: {
          // program
          if (Command::lastCom == this) {
            if (!executeTo(Command::lastCom)) printError();
          }
          // dependence
          else if (Command::lastCom->isSupporter()) {
            Command::lastCom->getUltimate()->printError();
          }
          // group
          else if (
            !(Command::lastCom->isUltimate() || executeTo(Command::lastCom)) ||
            Command::lastCom->isUltimate()
          ) {
            Command::lastCom->printError();
          }
        break;}
        // error
        default: {
          Message::printNeatNamed(
            MESSAGE_ERROR,
            "there should be an error handler",
            name
          );
        }
      }
    }
    // no children
    else Message::printNeatNamed(
      MESSAGE_ERROR,
      "no groups/commands available",
      name
    );

    // end of runtime
    remove();
  }
}

#endif // __CLI_MENU__PROGRAM_CPP__