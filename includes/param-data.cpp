#ifndef __CLI_MENU__DEBUG_H__
#define __CLI_MENU__DEBUG_H__

#include "message.h"

namespace cli_menu {

  class Debug {
  private:
    template <typename T>
    static void printList(
      mt::CR_STR title,
      mt::VEC2<T> vec2d
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
    static void printParamDataArray(mt::CR_STR name, CLI_MENU_FUNPAR) {

      Message::printBoundaryLine(1, 1);
      std::cout << "'" << name << "' ParamData (" << TEXTS.size() << "):";
      Message::printBoundaryLine(1, 0);

      Debug::printList<std::string>("\nTEXTS: ", TEXTS);
      Debug::printList<mt::LD>("\nNUMBERS: ", NUMBERS);
      Debug::printList<bool>("\nCONDITIONS: ", CONDITIONS);

      Message::printBoundaryLine(0, 1);
    }
  };
}

#endif // __CLI_MENU__DEBUG_H__