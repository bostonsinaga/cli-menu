#ifndef __CLI_MENU__RESULT_INPUTS_H__
#define __CLI_MENU__RESULT_INPUTS_H__

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

    static mt::VEC_STR cutLongText(
      mt::CR_VEC_STR textVec,
      mt::CR_INT mostCharsCount
    );

    template <typename T>
    static void printList(
      mt::CR_VEC<T> vec,
      mt::CR_INT mostCharsCount
    ) {
      for (int i = 0; i < vec.size(); i++) {

        if (i > 0) {
          std::cout << std::string(mostCharsCount + 6, ' ');
        }

        std::cout << vec[i];

        if (vec.size() > 1 && i < vec.size() - 1) {
          std::cout << ",\n";
        }
      }
    }

  public:
    static std::string title;

    void printVector();
    void rename(mt::CR_INT i, mt::CR_STR name);

    void addName(mt::CR_STR name);
    void addTexts(mt::CR_STR name, mt::CR_VEC_STR vec);
    void addNumbers(mt::CR_STR name, mt::CR_VEC_LD vec);
    void addConditions(mt::CR_STR name, mt::CR_VEC_BOL vec);

    void clearText(mt::CR_INT index);
    void clearNumber(mt::CR_INT index);
    void clearCondition(mt::CR_INT index);

    void pushText(mt::CR_INT index, mt::CR_STR value);
    void pushNumber(mt::CR_INT index, mt::CR_LD value);
    void pushCondition(mt::CR_INT index, mt::CR_BOL value);

    void pushTexts(mt::CR_INT i, mt::CR_VEC_STR vec);
    void pushNumbers(mt::CR_INT i, mt::CR_VEC_LD vec);
    void pushConditions(mt::CR_INT i, mt::CR_VEC_BOL vec);

    void cutName(mt::CR_INT i);
    void cutText(mt::CR_INT i, mt::CR_INT j);
    void cutNumbers(mt::CR_INT i, mt::CR_INT j);
    void cutCondition(mt::CR_INT i, mt::CR_INT j);

    void popName();
    void popText(mt::CR_INT i);
    void popNumbers(mt::CR_INT i);
    void popCondition(mt::CR_INT i);

    std::string getName(mt::CR_INT i);
    std::string getText(mt::CR_INT i, mt::CR_INT j);
    mt::LD getNumber(mt::CR_INT i, mt::CR_INT j);
    bool getCondition(mt::CR_INT i, mt::CR_INT j);

    mt::VEC_STR getTexts(mt::CR_INT i);
    mt::VEC_LD getNumbers(mt::CR_INT i);
    mt::VEC_BOL getConditions(mt::CR_INT i);

    int getSize() { return names.size(); }
    int getLastIndex() { return names.size() - 1; }
  };

  // callback formats
  typedef std::function<void(ResultInputs&)> RESULT_CALLBACK;
  typedef std::function<void()> PLAIN_CALLBACK;
}

#endif // __CLI_MENU__RESULT_INPUTS_H__