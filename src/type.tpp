#ifndef __CLI_MENU__TYPE_TPP__
#define __CLI_MENU__TYPE_TPP__

namespace cli_menu {

  /** PARAMETER */

  template <typename T, typename U>
  void Parameter<T, U>::clipboardOutputCopy() {
    if (!output.values.empty()) {
      if constexpr (mt::inspector::isLetter<U>()) {
        Clipboard::copyText(&output.values.back());
      }
      else if constexpr (mt::inspector::isNumber<U>()) {
        Clipboard::copyText(&output.values.back());
      }
    }
    else Langu::ageMessage::printResponse(SENTENCE_EMPTY_OUTPUT_THIS);
  }

  template <typename T, typename U>
  void Parameter<T, U>::printInput() {
    if (input.values.empty()) {
      Langu::ageMessage::printResponse(SENTENCE_EMPTY_INPUT_THIS);
    }
    else input.print(CONSOLE_HINT_1, 0);
  }

  template <typename T, typename U>
  void Parameter<T, U>::printOutput() {
    if (output.values.empty()) {
      Langu::ageMessage::printResponse(SENTENCE_EMPTY_OUTPUT_THIS);
    }
    else output.print(CONSOLE_HINT_1, 0);
  }

  template <typename T, typename U>
  void Parameter<T, U>::printDescendantInputs() {
    if (getChildren()) {
      getChildren()->traverse(
        [&](mt_ds::LinkedList *current)->bool {
          static_cast<Command*>(current)->printKeyword(CONSOLE_HINT_1, 0);
          dynamic_cast<Parameter<T, U>*>(current)->input.print(CONSOLE_HINT_2, 2);
          return true;
        }
      );
    }
  }

  template <typename T, typename U>
  void Parameter<T, U>::printDescendantOutputs() {
    if (getChildren()) {
      getChildren()->traverse(
        [&](mt_ds::LinkedList *current)->bool {
          static_cast<Command*>(current)->printKeyword(CONSOLE_HINT_1, 0);
          dynamic_cast<Parameter<T, U>*>(current)->output.print(CONSOLE_HINT_2, 2);
          return true;
        }
      );
    }
  }

  template <typename T, typename U>
  void Parameter<T, U>::resetInput() {
    input.values.clear();
    Langu::ageMessage::printResponse(SENTENCE_RESET_INPUT_THIS);
  }

  template <typename T, typename U>
  void Parameter<T, U>::resetOutput() {
    output.values.clear();
    Langu::ageMessage::printResponse(SENTENCE_RESET_OUTPUT_THIS);
  }

  template <typename T, typename U>
  void Parameter<T, U>::resetDescendantInputs() {

    if (getChildren()) {
      getChildren()->traverse(
        [&](mt_ds::LinkedList *current)->bool {
          dynamic_cast<Parameter<T, U>*>(current)->input.values.clear();
          return true;
        }
      );
    }

    Langu::ageMessage::printResponse(SENTENCE_RESET_INPUT_DESCENDANTS);
  }

  template <typename T, typename U>
  void Parameter<T, U>::resetDescendantOutputs() {

    if (getChildren()) {
      getChildren()->traverse(
        [&](mt_ds::LinkedList *current)->bool {
          dynamic_cast<Parameter<T, U>*>(current)->output.values.clear();
          return true;
        }
      );
    }

    Langu::ageMessage::printResponse(SENTENCE_RESET_OUTPUT_DESCENDANTS);
  }
}

#endif // __CLI_MENU__TYPE_TPP__