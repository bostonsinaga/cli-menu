#ifndef __CLI_MENU__DEBUG_H__
#define __CLI_MENU__DEBUG_H__

#include "message.h"

namespace cli_menu {
namespace debug {

  void printParamDataArray(mt::CR_STR name, CLI_MENU_FUNPAR) {

    Message::printBoundaryLine(1, 1);
    std::cout << "'" << name << "' ParamData:";
    Message::printBoundaryLine(1, 0);

    std::cout << "\nTEXTS      : ";

    for (int i = 0; i < TEXTS.size(); i++) {
      std::cout << TEXTS[i];
      if (i < TEXTS.size() - 1) std::cout << ", ";
    }

    std::cout << "\nNUMBERS    : ";

    for (mt::CR_VEC_DBL numVec : NUMBERS) {
      std::cout << "[";

      for (int i = 0; i < numVec.size(); i++) {
        std::cout << numVec[i];
        if (i < numVec.size() - 1) std::cout << ", ";
      }

      std::cout << "]";
    }

    std::cout << "\nCONDITIONS : ";

    for (int i = 0; i < CONDITIONS.size(); i++) {
      std::cout << CONDITIONS[i];
      if (i < CONDITIONS.size() - 1) std::cout << ", ";
    }

    Message::printBoundaryLine(1, 1);
  }
}}

#endif // __CLI_MENU__DEBUG_H__