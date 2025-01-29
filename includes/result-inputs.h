#ifndef __CLI_MENU__RESULT_INPUTS_H__
#define __CLI_MENU__RESULT_INPUTS_H__

#include "message.h"

namespace cli_menu {

  class ResultInputs {
  private:
    static mt::VEC_STR names;
    static mt::VEC2_STR texts;
    static mt::VEC2_LD numbers;
    static mt::VEC2_BOL conditions;

    static const std::string subtitles[3];

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
    enum RESULT_TYPE {
      RESULT_TEXT, RESULT_NUMBER, RESULT_CONDITION
    };

    static std::string title;

    static void printVector();
    static void printAt(RESULT_TYPE type, mt::CR_INT index);
    static void rename(mt::CR_INT i, mt::CR_STR name);

    static void addName(mt::CR_STR name);
    static void addTexts(mt::CR_STR name, mt::CR_VEC_STR vec);
    static void addNumbers(mt::CR_STR name, mt::CR_VEC_LD vec);
    static void addConditions(mt::CR_STR name, mt::CR_VEC_BOL vec);

    static void clearText(mt::CR_INT index);
    static void clearNumber(mt::CR_INT index);
    static void clearCondition(mt::CR_INT index);

    static void pushText(mt::CR_INT index, mt::CR_STR value);
    static void pushNumber(mt::CR_INT index, mt::CR_LD value);
    static void pushCondition(mt::CR_INT index, mt::CR_BOL value);

    static void pushTexts(mt::CR_INT i, mt::CR_VEC_STR vec);
    static void pushNumbers(mt::CR_INT i, mt::CR_VEC_LD vec);
    static void pushConditions(mt::CR_INT i, mt::CR_VEC_BOL vec);

    static void cutName(mt::CR_INT i);
    static void cutText(mt::CR_INT i, mt::CR_INT j);
    static void cutNumbers(mt::CR_INT i, mt::CR_INT j);
    static void cutCondition(mt::CR_INT i, mt::CR_INT j);

    static void popName();
    static void popText(mt::CR_INT i);
    static void popNumbers(mt::CR_INT i);
    static void popCondition(mt::CR_INT i);

    static std::string getName(mt::CR_INT i);
    static std::string getText(mt::CR_INT i, mt::CR_INT j);
    static mt::LD getNumber(mt::CR_INT i, mt::CR_INT j);
    static bool getCondition(mt::CR_INT i, mt::CR_INT j);

    static mt::VEC_STR getTexts(mt::CR_INT i);
    static mt::VEC_LD getNumbers(mt::CR_INT i);
    static mt::VEC_BOL getConditions(mt::CR_INT i);

    static int getSize() { return names.size(); }
    static int getLastIndex() { return names.size() - 1; }
    static bool doesAnyVectorContain(mt::CR_INT index);
  };
}

#endif // __CLI_MENU__RESULT_INPUTS_H__