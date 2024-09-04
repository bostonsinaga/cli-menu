#ifndef __CLI_MENU__MESSAGE_CPP__
#define __CLI_MENU__MESSAGE_CPP__

#include "message.h"

namespace cli_menu {

  // set text to start with a capital letter and end with a period
  std::string Message::tidyUpText(mt::CR_STR text) {
    std::string retStr = text;

    if (!text.empty()) {

      const char last = retStr[text.length() - 1];
      retStr[0] = std::toupper(retStr[0]);

      if (last != '.' && last != ';' &&
        last != '!' && last != '?' &&
        last != ':' && last != ','
      ) {
        retStr.push_back('.');
      }
      else if (last == ',') {
        retStr.pop_back();
        retStr.push_back('.');
      }
    }

    return retStr;
  }

  std::string Message::getObligatoryString(mt::CR_BOL isRequired) {
    if (isRequired) return "<req>";
    else return "<opt>";
  }

  std::string Message::customColored(
    mt::USI code,
    mt::CR_STR text
  ) {
    if (code < 0 || code > colorsCount) code = COLOR.WHITE;
    return "\033[" + std::to_string(colorCodes[code]) + "m" + text + "\033[0m";
  }

  std::string Message::getColoredTag(mt::CR_INT flag) {
    switch (flag) {
      case STATUS.HINT: {
        return customColored(COLOR.LIGHT_BLUE, "HINT. ");
      }
      case STATUS.WARNING: {
        return customColored(COLOR.YELLOW, "WARNING. ");
      }
      case STATUS.ERROR: {
        return customColored(COLOR.RED, "ERROR. ");
      }
      case STATUS.SUCCEED: {
        return customColored(COLOR.GREEN, "SUCCEED. ");
      }
      case STATUS.CANCELED: {
        return customColored(COLOR.BLUE, "CANCELED. ");
      }
    }
    return "";
  }

  void Message::print(
    mt::CR_INT flag,
    std::string text,
    std::string name,
    bool toUppercase
  ) {
    if (text.length() > 0) {
      std::cout << std::endl;

      if (toUppercase) {
        mt_uti::StrTools::changeStringToUppercase(name);
      }

      if (name != "") {
        std::cout << name << ": ";
      }

      std::cout << getColoredTag(flag) << tidyUpText(text);
    }
  }

  void Message::printDecoration(mt::CR_INT count, mt::CR_CHR ch) {
    std::cout << std::endl;
    for (int i = 0; i < count; i++) { std::cout << ch; }
    std::cout << std::endl;
  }

  void Message::printDevError(
    mt::CR_STR funName,
    mt::CR_STR paramTypeName,
    mt::CR_STR customMessage
  ) {
    print(STATUS.ERROR,
      customMessage + " '" + paramTypeName + "'",
      "cli_menu::" + funName,
      false
    );
  }

  void Message::printCommandsSequence(
    Command *command,
    std::string continueString
  ) {
    if (command) {
      continueString += command->getFullName() +
        "<" + getObligatoryString(command->isRequired()) + "> ";

      for (Command *com : command->getItems()) {
        printCommandsSequence(com, continueString);
      }

      std::cout << continueString << std::endl;
    }
    else printDevError("Message::printCommandsSequence", "Command");
  }

  void Message::printCommandError(Command *command) {
    if (command) {
      std::cerr
        << mt_uti::StrTools::getStringToUppercase(command->getName())
        << ": " << getColoredTag(STATUS.ERROR) << "Please follow the format.\n\n";
      printCommandsSequence(command);
    }
    else printDevError("Message::printCommandError", "Command");
  }

  void Message::printProgramError(
    Program *program,
    mt::CR_BOL isEmpty
  ) {
    if (!program) {
      printDevError("Message::printProgramError", "Program");
      return;
    }

    std::string about;
    if (isEmpty) about = "no input provided";
    else about = "invalid inputs";

    print(STATUS.ERROR,
      about + ". Please type '--help' or '--menu' for available commands",
      program->getName()
    );
  }

  void Message::printMainHelp(Program *program) {
    if (!program) {
      printDevError("Message::printMainHelp", "Program");
      return;
    }

    std::string programName = program->getName();
    int decorDotsCount = programName.length() * 2;
    mt_uti::StrTools::changeStringToUppercase(programName);

    std::cout << "Welcome to '" << programName << "'\n\n";

    std::cout << "version: " << program->getVersion().stringify() << std::endl;
    std::cout << "author: " << program->getAuthor();

    printDecoration(decorDotsCount);

    std::cout
      << "HINT:\n"
      << "* Use '-' for parameter and '--' for toggle\n"
      << "* The <req> or <opt> are only available in parameter (text/number)\n"
      << "* The toggle (boolean) always optional, except for main command and groups\n"
      << "* Please type '--[command] --help' to view detailed information";

    printDecoration(decorDotsCount);

    for (Command *com : program->getItems()) {
      printCommandsSequence(com);
    }
  }

  void Message::printSubHelp(Command *command) {
    if (!command) {
      printDevError("Message::printSubHelp", "Command");
      return;
    }

    if (command->getInheritanceFlag() == PROGRAM) {
      printDevError("Message::printSubHelp", "Program", "cannot use");
    }
    else {
      std::string comName = command->getName();
      int decorDotsCount = comName.length() * 2;
      mt_uti::StrTools::changeStringToUppercase(comName);

      std::cout << comName << ':';
      printDecoration(decorDotsCount);

      std::cout << tidyUpText(command->getDescription());
      printDecoration(decorDotsCount);

      printCommandsSequence(command);
    }
  }
}

#endif // __CLI_MENU__MESSAGE_CPP__