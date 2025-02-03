#ifndef __CLI_MENU__TOGGLE_CPP__
#define __CLI_MENU__TOGGLE_CPP__

#include "toggle.h"

namespace cli_menu {

  Toggle::Toggle(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    mt::CR_BOL accumulating_in,
    CM_CALLBACK callback_in,
    mt::CR_BOL propagatingCallback_in
  ) : Command(
    name_in, description_in,
    required_in, parent_in,
    accumulating_in, callback_in,
    propagatingCallback_in
  ) {}

  Toggle::Toggle(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    mt::CR_BOL accumulating_in
  ) : Command(
    name_in, description_in,
    required_in, parent_in,
    accumulating_in
  ) {}

  void Toggle::initData(mt::CR_VEC_BOL data) {
    ResultInputs::addConditions(name, data);
    useResultInputsIndex();
  }

  void Toggle::initDefaultData() {
    if (!used) {
      initData({defaultCondition});
    }
  }

  void Toggle::setData(mt::CR_BOL condition) {
    if (!used) {
      initData({condition});
    }
    else ResultInputs::pushCondition(
      resultInputsIndex, condition
    );
  }

  void Toggle::resetData(mt::CR_BOL discarded) {
    if (used) {

      // backup registered vector
      conditionsBackup = ResultInputs::getConditions(resultInputsIndex);

      // pop registered vector
      Command::resetData(discarded);

      // clean registered vector
      if (!(discarded || accumulating)) {
        ResultInputs::clearCondition(resultInputsIndex);
      }
    }
  }

  std::string Toggle::getDashedName() {
    return "--" + name;
  }

  std::string Toggle::getFullName(
    mt::CR_BOL useDash,
    mt::CR_BOL useInputType,
    mt::CR_BOL useLevelName,
    CR_CLR nameColor
  ) {
    std::string nameStr, text;

    // choose name
    if (useDash) {
      nameStr = getDashedName();
    }
    else nameStr = name;

    // set 'nameStr' color
    nameStr = Color::getString(
      nameStr, nameColor
    );

    // final assignments
    text = nameStr;

    // add level name
    if (useLevelName) {
      text += getLevelLabel();
    }

    return text;
  }

  mt::USI Toggle::match() {

    if (!directInputs.empty()) {
      std::string copyName, copyInput;

      // copy to secure original strings
      copyMatchStrings(
        copyName, copyInput, directInputs.back()
      );

      if (copyName == DashTest::cleanDouble(copyInput)) {

        directInputs.pop_back();
        Command::lastCom = this;
        resetData(false);

        if (isParent()) {
          setData(true);

          // redirected to first child
          return matchTo(
            static_cast<Cm*>(children.front())
          );
        }
        // toddler
        else {
          // actually there is no need for argument
          if (directInputs.size() > 0) {

            int boolFlag = Control::booleanTest(
              mt_uti::StrTools::getStringToLowercase(
                *(directInputs.end() - 2)
              )
            );

            // between 1 or 2 is true
            if (boolFlag) {
              setData(Control::revealBoolean(boolFlag));
            }
            else setData(true);
          }

          return matchTo(getUnusedNeighbor(this));
        }
      }

      // point to neighbor if input not matched
      return askNeighbor();
    }
    // 'directInputs' completion
    else if (isMatchNeedDialog()) {
      return dialogTo(
        static_cast<Cm*>(parent)
      );
    }
    // invoke callback
    else if (ultimate && getRequiredCount() == 0) {
      Command::lastCom = ultimate;
      return COMPLETED_FLAG;
    }
    // print error of incompleteness
    return FAILED_FLAG;
  }

  mt::USI Toggle::answerSpecial(mt::CR_STR bufferStr) {

    int boolFlag = Control::booleanTest(bufferStr);

    // condition input
    if (boolFlag) {

      Command::lastCom = chooseLastCommand();
      setData(Control::revealBoolean(boolFlag));

      // inside ultimate only
      if (isSupporter()) return questionTo(
        getUnusedNeighbor(this)
      );
      // dead end
      else if (isToddler()) {
        return COMPLETED_FLAG;
      }
      // back to this dialog
      return Command::dialog();
    }
    else Message::printNeatDialog(
      Message::ERROR_FLAG,
      "only accepts boolean values"
    );

    return PASSED_FLAG;
  }

  void Toggle::viewAction() {
    ResultInputs::printAt(
      ResultInputs::RESULT_CONDITION,
      resultInputsIndex
    );
  }

  mt::USI Toggle::dialog() {
    const bool needQuestion = parent && !selecting && isToddler();

    if (conditionsBackup.empty()) {
      if (needQuestion) {
        return question();
      }
    }
    // remember the past
    else initData(conditionsBackup);

    // inverted in base method
    selecting = false;

    // no need to set condition exclusively on parent
    if (!used) setData(true);

    return Command::dialog();
  }
}

#endif // __CLI_MENU__TOGGLE_CPP__