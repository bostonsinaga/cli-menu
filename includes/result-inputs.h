#ifndef __CLI_MENU__RESULT_INPUTS_H__
#define __CLI_MENU__RESULT_INPUTS_H__

#include <memory>
#include "message.h"

namespace cli_menu {

  // inheritance flags
  enum { COMMAND, PROGRAM, PARAMETER, TOGGLE };

  class ResultInputs {
  private:
    mt::VEC_STR names;
    mt::VEC2_STR texts;
    mt::VEC2_LD numbers;
    mt::VEC2_BOL conditions;

  public:
    void printVector(mt::CR_STR name) {

      Message::printBoundaryLine(1, 1);
      std::cout << "'" << name << "' ResultInputs (" << ResultInputs::texts.size() << "):";
      Message::printBoundaryLine(1, 0);

      static const std::string titles[3] = {
        "TEXTS", "NUMBERS", "CONDITIONS"
      };

      for (int i = 0; i < 3; i++) {
        std::cout << "\n" << titles[i] << ": ";

        for (int j = 0; j < names.size(); j++) {
          std::cout << "\n  " << names[j] << ": [";

          for (int k = 0; k < texts[j].size(); k++) {
            if (k > 0) std::cout << "   ";

            std::cout << (i == 0 ? texts[j][k] :
              (i == 1 ? numbers[j][k] : conditions[j][k])
            );

            if (texts[j].size() > 1) {
              if (k < texts[j].size() - 1) {
                std::cout << ",\n";
              }
            }
          }

          std::cout << ']' << (j < names.size() - 1 ? ',' : '\0');
        }

        std::cout << std::endl;
      }

      Message::printBoundaryLine(0, 1);
    }
  };

  // callback formats
  typedef std::function<void(ResultInputs&)> CALLBACK;
  typedef std::function<void()> PLAIN_CALLBACK;
}

#endif // __CLI_MENU__RESULT_INPUTS_H__