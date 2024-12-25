#ifndef __CLI_MENU__PARAM_DATA_H__
#define __CLI_MENU__PARAM_DATA_H__

#include "message.h"

namespace cli_menu {

  class ParamData;
  typedef const ParamData& CR_PD;

  // callback formats
  typedef std::function<void(CR_PD)> CALLBACK;
  typedef std::function<void()> PLAIN_CALLBACK;
  typedef std::shared_ptr<CALLBACK> SP_CALLBACK;
  typedef std::shared_ptr<PLAIN_CALLBACK> SP_PLAIN_CALLBACK;
  typedef const SP_CALLBACK& CR_SP_CALLBACK;
  typedef const SP_PLAIN_CALLBACK& CR_SP_PLAIN_CALLBACK;

  class ParamData {
  private:
    template <typename T>
    static void printList(
      mt::CR_STR title,
      mt::CR_VEC2<T> vec2d
    );

  public:
    mt::VEC2_STR texts;
    mt::VEC2_LD numbers;
    mt::VEC2_BOL conditions;

    void printVector(mt::CR_STR name) {

      Message::printBoundaryLine(1, 1);
      std::cout << "'" << name << "' ParamData (" << ParamData::texts.size() << "):";
      Message::printBoundaryLine(1, 0);

      ParamData::printList<std::string>("\nTEXTS: ", texts);
      ParamData::printList<mt::LD>("\nNUMBERS: ", numbers);
      ParamData::printList<bool>("\nCONDITIONS: ", conditions);

      Message::printBoundaryLine(0, 1);
    }
  };

  template <typename T>
  void ParamData::printList(
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
}

#endif // __CLI_MENU__PARAM_DATA_H__