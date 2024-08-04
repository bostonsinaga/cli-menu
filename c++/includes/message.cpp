#ifndef __CLI_MENU__MESSAGE_CPP__
#define __CLI_MENU__MESSAGE_CPP__

#include "message.h"

namespace cli_menu {

  void Message::printDots(int count) {
    std::cout << std::endl;
    for (int i = 0; i < count; i++) { std::cout << '.'; }
    std::cout << std::endl;
  }

  std::string Message::setTextCapitalBeginPeriodEnd(std::string text) {
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

  std::string Message::getObligatoryString(bool isRequired) {
    if (isRequired) return "<req>";
    else return "<opt>";
  }

  std::string Message::getColoredTag(int flag) {
    switch (flag) {
      case WARNING: {
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
      default: return "\033[35mCANCELED. \033[0m";
    }
  }

  void Message::printCommand(Command *com) {

    std::cout << "--" << com->getName();
    Parameters *params = com->getParameters();
    Toggles *togs = com->getToggles();

    for (int i = 0; i < params->amount(); i++) {
      std::cout << params->getName(i)
        << '[' << params->getType(i) << ']'
        << '<' << getObligatoryString(params->getObligatory(i)) << "> ";
    }

    for (int i = 0; i < togs->amount(); i++) {
      std::cout << togs->getName(i) << ' ';
    }

    std::cout << "\n\n";
  }

  void Message::printCommandError(Command *com) {
    std::cerr
      << mt_uti::StrTools::getStringToUppercase(com->getName())
      << ": " << getColoredTag(ERROR) << "Please follow the command format.\n\n";

    printCommand(com);
  }

  void Message::print(int flag, std::string text, std::string comName) {
    if (text.length() > 0) {
      mt_uti::StrTools::changeStringToUppercase(comName);
      if (comName != "") std::cout << comName << ": ";
      std::cout << getColoredTag(flag) << setTextCapitalBeginPeriodEnd(text);
    }
  }

  void Message::printMainHelp(
    VEC_COM &comms,
    std::string programName,
    std::string author,
    std::string version
  ) {
    int dotsCount = programName.length() * 2;
    mt_uti::StrTools::stringToUppercase(programName);

    std::cout << "Welcome to '" << programName << "'\n\n";

    if (version != "") std::cout << "version: " << version << std::endl;
    if (author != "") std::cout << "author: " << author;

    printDots(dotsCount);

    std::cout
      << "HINT:\n"
      << "* Use '-' for parameter and '--' for toggle\n"
      << "* The <req> or <opt> are only available in parameter\n"
      << "* The toggle (boolean) always optional, except for the command name in front\n"
      << "* Please type '--[command] --help' to view detailed information";

    printDots(dotsCount);
    for (auto &com : comms) { printCommand(com); }
  }

  void Message::printSubHelp(Command *com) {
    
    std::string comName = com->getName();
    int dotsCount = comName.length() * 2;
    mt_uti::StrTools::stringToUppercase(comName);

    std::cout << comName << ':';
    printDots(dotsCount);

    std::cout << setTextCapitalBeginPeriodEnd(com->getDescription());
    printDots(dotsCount);

    Parameters *params = com->getParameters();
    Toggles *togs = com->getToggles();

    printCommand(com);
    printDots(dotsCount);

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