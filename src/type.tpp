#ifndef __CLI_MENU__TYPE_TPP__
#define __CLI_MENU__TYPE_TPP__

namespace cli_menu {

  template <UNORMAP_COMVEC_TYPE T>
  void Parameter::printInput_temp() {
    if (Data::isEmpty<T>(this)) {
      Langu::ageMessage::printResponse(SENTENCE_EMPTY_INPUT_THIS);
    }
    else Data::print<T>(this, Console::StickedCodes, IndentSticked());
  }

  template <UNORMAP_COMVEC_TYPE T>
  void Parameter::printChildrenInputs_temp() {
    if (getChildren()) {
      getChildren()->head()->forEach(
        [&](mt_ds::LinkedList *current)->bool {

          if (!static_cast<Command*>(current)->isPseudo()) {
            std::cout << '\r';

            static_cast<Command*>(current)->printKeyword(
              CONSOLE_HINT_1, IndentSticked()
            );

            Data::print<T>(
              static_cast<Command*>(current),
              Console::BranchedCodes,
              IndentBranched()
            );
          }

          return true;
        }
      );
    }
  }

  template <UNORMAP_COMVEC_TYPE T>
  void Parameter::resetInput_temp() {
    Data::reset<T>(this);
    Langu::ageMessage::printResponse(SENTENCE_RESET_INPUT_THIS);
  }

  template <UNORMAP_COMVEC_TYPE T>
  void Parameter::resetDescendantInputs_temp() {
    if (getChildren()) {
      getChildren()->traverse(
        [&](mt_ds::LinkedList *current)->bool {
          Data::reset<T>(static_cast<Command*>(current));
          return true;
        }
      );
    }

    Langu::ageMessage::printResponse(SENTENCE_RESET_INPUT_DESCENDANTS);
  }
}

#endif // __CLI_MENU__TYPE_TPP__