#ifndef __CLI_MENU__MESSAGE_CPP__
#define __CLI_MENU__MESSAGE_CPP__

#include "message.h"

namespace cli_menu {

  void Message::printDecoration(CR_INT count, CR_CHR ch) {
    std::cout << std::endl;
    for (int i = 0; i < count; i++) { std::cout << ch; }
    std::cout << std::endl;
  }

  void Message::setTextCapitalBeginPeriodEnd(std::string &text) {
    if (text.length() > 0) {

      text[0] = std::toupper(text[0]);
      char last = text[text.length() - 1];

      if (last != '.' && last != ';' &&
        last != '!' && last != '?' &&
        last != ':' && last != ','
      ) {
        text.push_back('.');
      }
      else if (last == ',') {
        text.pop_back();
        text.push_back('.');
      }
    }
    
    return text;
  }

  std::string Message::getObligatoryString(CR_BOL isRequired) {
    if (isRequired) return "<req>";
    else return "<opt>";
  }

  std::string Message::getColoredTag(CR_INT flag) {
    switch (flag) {
      case HINT: {
        return "\033[34mHINT. \033[0m";
      }
      case WARNING: {
        return "\033[33mWARNING. \033[0m";
      }
      case ERROR: {
        return "\033[31mERROR. \033[0m";
      }
      case SUCCEED: {
        return "\033[32mSUCCEED. \033[0m";
      }
      case CANCELED: {
        return "\033[35mCANCELED. \033[0m";
      }
    }
    return "";
  }

  void Message::printCommandList(
    Command *com,
    std::string contiStr
  ) {
    if (com) {
      contiStr += DashTest::getString(com) + com->getName(true)
        + '<' getObligatoryString(com->isRequired()) + "> ";

      for (Command *com : startCom->getItems()) {
        printCommandList(com, contiStr);
      }

      std::cout << contiStr << std::endl;
    }
  }

  void Message::printCommandError(Command *com) {
    std::cerr
      << mt_uti::StrTools::getStringToUppercase(com->getName())
      << ": " << getColoredTag(ERROR) << "Please follow the format.\n\n";
    printCommandList(com);
  }

  void Message::print(CR_INT flag, std::string text, CR_STR comName) {
    if (text.length() > 0) {
      mt_uti::StrTools::changeStringToUppercase(comName);
      if (comName != "") std::cout << comName << ": ";
      std::cout << getColoredTag(flag) << setTextCapitalBeginPeriodEnd(text);
    }
  }

  void Message::printMainHelp(
    Command *comProgram,
    CR_STR author,
    CR_STR version
  ) {
    std::string programName = comProgram->getName();
    int decorDotsCount = programName.length() * 2;
    mt_uti::StrTools::changeStringToUppercase(programName);

    std::cout << "Welcome to '" << programName << "'\n\n";

    if (version != "") std::cout << "version: " << version << std::endl;
    if (author != "") std::cout << "author: " << author;

    printDecoration(decorDotsCount);

    std::cout
      << "HINT:\n"
      << "* Use '-' for parameter and '--' for toggle\n"
      << "* The <req> or <opt> are only available in parameter (text/number)\n"
      << "* The toggle (boolean) always optional, except for main command and classifications\n"
      << "* Please type '--[command] --help' to view detailed information";

    printDecoration(decorDotsCount);

    for (Command *com : comProgram->getItems()) {
      printCommandList(com);
    }
  }

  void Message::printSubHelp(Command *com) {
    
    std::string comName = com->getName();
    int decorDotsCount = comName.length() * 2;
    mt_uti::StrTools::changeStringToUppercase(comName);

    std::cout << comName << ':';
    printDecoration(decorDotsCount);

    std::cout << setTextCapitalBeginPeriodEnd(com->getDescription());
    printDecoration(decorDotsCount);

    Parameters *params = com->getParameters();
    Toggles *togs = com->getToggles();

    printCommandList(com);
    printDecoration(decorDotsCount);

    for (int i = 0; i < params->amount(); i++) {
      std::cout
        << '-' << params->getName(i) << getObligatoryString(params->getObligatory(i))
        << ": " << params->getDescription(i) << "\n\n";
    }

    std::cout << "Optional Toggles:\n\n";

    for (int i = 0; i < togs->amount(); i++) {
      std::cout << "--" << togs->getName(i) << ": " << togs->getDescription(i);
      if (i == togs->amount() - 1) std::cout << std::endl;
      else std::cout << "\n\n";
    }
  }
}

#endif // __CLI_MENU__MESSAGE_CPP__