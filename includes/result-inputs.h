#ifndef __CLI_MENU__RESULT_INPUTS_H__
#define __CLI_MENU__RESULT_INPUTS_H__

#include <memory>
#include "message.h"

namespace cli_menu {

  // inheritance flags
  enum { COMMAND, PROGRAM, PARAMETER, TOGGLE };

  class ResultInputs {
  private:
    template <typename T>
    static void printList(
      mt::CR_STR title,
      mt::CR_VEC2<T> vec2d
    ) {
      std::cout << title;

      for (int i = 0; i < vec2d.size(); i++) {
        std::cout << "\n  [";

        for (int j = 0; j < vec2d[i].size(); j++) {

          if (j > 0) std::cout << "   ";
          std::cout << vec2d[i][j];

          if (vec2d[i].size() > 1) {
            if (j < vec2d[i].size() - 1) {
              std::cout << ",\n";
            }
          }
        }

        std::cout << ']' << (i < vec2d.size() - 1 ? ',' : '\0');
      }

      std::cout << std::endl;
    }

  public:
    mt::VEC2_STR texts;
    mt::VEC2_LD numbers;
    mt::VEC2_BOL conditions;

    void printVector(mt::CR_STR name) {

      Message::printBoundaryLine(1, 1);
      std::cout << "'" << name << "' ResultInputs (" << ResultInputs::texts.size() << "):";
      Message::printBoundaryLine(1, 0);

      ResultInputs::printList<std::string>("\nTEXTS: ", texts);
      ResultInputs::printList<mt::LD>("\nNUMBERS: ", numbers);
      ResultInputs::printList<bool>("\nCONDITIONS: ", conditions);

      Message::printBoundaryLine(0, 1);
    }
  };

  // callback formats
  typedef std::function<void(ResultInputs&)> CALLBACK;
  typedef std::function<void()> PLAIN_CALLBACK;
}

#endif // __CLI_MENU__RESULT_INPUTS_H__