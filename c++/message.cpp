#ifndef __CLI_MENU__MESSAGE_CPP__
#define __CLI_MENU__MESSAGE_CPP__

#include "message.h"

namespace cli_menu {

  void Message::printDots(int count) {
    std::cout << std::endl;
    for (int i = 0; i < count; i++) { std::cout << '.'; }
    std::cout << std::endl;
  }

  std::string Message::fixTextBeginEnd(std::string text) {
    if (text.length() > 0) {
      std::toupper(text[0]);
      if (text[text.length() - 1] != '.') text.push_back('.');
    }
    return text;
  }

  std::string Message::getObligatoryString(bool isRequired) {
    if (isRequired) return "<req>";
    else return "<opt>";
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

  void Message::printError(std::string text, std::string comName) {
    if (text.length() > 0) {
      std::cerr << comName << ": Error. " << fixTextBeginEnd(text);
    }
  }

  void Message::printMainHelp(
    VEC_COM &comms,
    std::string programName,
    std::string author,
    std::string version
  ) {
    int dotsCount = programName.length() * 2;
    Util::stringToUppercase(programName);

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
    Util::stringToUppercase(comName);

    std::cout << comName << ':';
    printDots(dotsCount);

    std::cout << fixTextBeginEnd(com->getDescription());
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