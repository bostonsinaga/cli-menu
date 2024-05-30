#ifndef __CLI_MENU__MESSAGE_H__
#define __CLI_MENU__MESSAGE_H__

#include <iostream>
#include <vector>
#include "command.h"

namespace cli_menu {
  class Message {
  private:
    void printDots(int count) {
      std::cout << std::endl;
      for (int i = 0; i < count; i++) { std::cout << '.'; }
      std::cout << std::endl;
    }

    std::string fixTextBeginEnd(std::string text) {
      if (text.length() > 0) {
        std::toUpper(text[0]);
        if (text[text.length - 1] != '.') text += '.';
      }
    }

    std::string getObligatoryString(bool isRequired) {
      if (isRequired) return "<req>";
      else return "<opt>";
    }

    void printCommand(Command *com) {

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

  public:
    void printError(std::string text, std::string comName = "CLI Menu") {
      if (text.length() > 0) {
        std::cerr << comName << ": Error. " << fixTextBeginEnd(text);
      }
    }

    void printMainHelp(
      std::vector<Command*> &comms,
      std::string programName,
      std::string author = "",
      std::string version = "1.0"
    ) {
      int dotsCount = programName.length() * 2;

      std::cout << "Welcome to '" << std::toUpper(programName) << "'\n\n";

      if (version != "") std:cout << "version: " << version << std::endl;
      if (author != "") std:cout << "author: " << author;

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

    void printSubHelp(Command *com) {
      
      std::string comName = com->getName();
      int dotsCount = comName.length() * 2;

      std::cout << std::toUpper(comName) << ':';
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
  };
}

#endif // __CLI_MENU__MESSAGE_H__