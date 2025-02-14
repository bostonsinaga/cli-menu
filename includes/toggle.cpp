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

  void Toggle::setCondition(mt::CR_BOL condition) {
    overwrite();

    if (!used) {
      initData({condition});
    }
    else ResultInputs::pushCondition(
      resultInputsIndex, condition
    );
  }

  void Toggle::setConditions(mt::CR_VEC_BOL conditions) {
    overwrite();

    if (!used) {
      if (!accumulating) initData({conditions.back()});
      else initData(conditions);
    }
    // always in accumulation
    else ResultInputs::pushConditions(
      resultInputsIndex, conditions
    );
  }

  void Toggle::setData(mt::CR_STR input) {
    Util::BOOL_FLAG boolFlag = Util::booleanTest(input);

    if (boolFlag != Util::BOOL_OTHER) {
      setCondition(Util::revealBoolean(boolFlag));
    }
    else initDefaultData();
  }

  void Toggle::resetData(RESET_FLAG resetFlag) {
    if (used) {

      // backup registered vector
      if (resetFlag == RESET_FLAG::BACKUP) {
        conditionsBackup = ResultInputs::getConditions(resultInputsIndex);
      }
      // clean the backup
      else if (resetFlag == RESET_FLAG::DISCARD) {
        resetBackupData();
      }

      // pop registered vector
      Command::resetData(resetFlag);

      // clean registered vector
      if (resetFlag != RESET_FLAG::DISCARD && !accumulating) {
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

  void Toggle::printTypeError() {
    Message::printNeatDialog(
      Message::ERROR_FLAG,
      "only accepts boolean values",
      !Command::matching
    );
  }

  mt::USI Toggle::match() {

    if (!directInputs.empty()) {
      std::string copyName, copyInput;

      // copy to secure original strings
      copyMatchStrings(
        copyName, copyInput, directInputs.back()
      );

      // e.g. '--name condition'
      if (copyName == DashTest::cleanDouble(copyInput)) {
        Command *firstChild;

        /**
         * Must be before the 'popBackSet' call
         * to avoid question in the middle error.
         */
        if (isParent()) {
          required = false;
          firstChild = static_cast<Cm*>(children.front());
        }

        const mt::USI flag = popBackSet();

        // redirected to first child
        if (flag == PASSED_FLAG) {
          return matchTo(firstChild);
        }
        // 'directInputs' may now be empty (dialog may already invoked)
        else if (flag != CONTINUE_FLAG) {
          return flag;
        }

        // implicit condition
        initDefaultData();

        if (isToddler()) return COMPLETED_FLAG;
        return matchTo(firstChild);
      }

      // not matched point to neighbor
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

    Util::BOOL_FLAG boolFlag = Util::booleanTest(bufferStr);

    // condition input
    if (boolFlag != Util::BOOL_OTHER) setCondition(
      Util::revealBoolean(boolFlag)
    );
    else printTypeError();

    return downTheChannel();
  }

  void Toggle::clipboardAction() {

    mt::VEC_BOL conditionsRef;
    Clipboard::pasteConditions(conditionsRef);

    if (!conditionsRef.empty()) {
      setConditions(conditionsRef);
    }
    else printTypeError();
  }

  void Toggle::viewAction() {
    ResultInputs::printAt(
      ResultInputs::RESULT_CONDITION,
      resultInputsIndex
    );
  }

  /**
   * Might call 'question' for arguments
   * before selection in 'dialog'.
   */
  mt::USI Toggle::dialog() {

    const bool needQuestion = (
      parent && !Command::selecting && isToddler()
    );

    if (conditionsBackup.empty()) {
      if (needQuestion) {
        return question();
      }
    }
    // remember the past
    else initData(conditionsBackup);

    // no need to set condition exclusively on parent
    initDefaultData();

    return Command::dialog();
  }
}

#endif // __CLI_MENU__TOGGLE_CPP__