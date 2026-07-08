#ifndef __CLI_MENU__DATA_TPP__
#define __CLI_MENU__DATA_TPP__

namespace cli_menu {

  template <typename T>
  T Data<T>::latest() {
    if (!values.empty()) return values.back();
    else {
      if constexpr (mt::inspector::isPointer<T>()) {
        return nullptr;
      }
      else return T();
    }
  }

  template <typename T>
  mt::VEC_STR Data<T>::stringify(mt::CR_STR separator) {
    mt::VEC_STR stringified;

    for (int i = 0 ; i < values.size(); i++) {

      if constexpr (std::is_same_v<T, bool>) {
        if (values[i]) stringified.push_back("true" + separator);
        else stringified.push_back("false" + separator);
      }
      else if constexpr (mt::inspector::isLetter<T>()) {
        stringified.push_back(values[i] + separator);
      }
      else if constexpr (mt::inspector::isNumber<T>()) {
        stringified.push_back(std::to_string(values[i]) + separator);
      }
      else break;
    }

    return stringified;
  }

  template <typename T>
  void Data<T>::print(
    mt::CR<CONSOLE_CODE> consoleCode,
    mt::CR_SZ numberOfIndents
  ) {
    for (mt::CR_STR str : stringify()) {
      Console::logString(
        std::string(' ', numberOfIndents) + str,
        Console::messageColors[consoleCode]
      );
    }
  }
}

#endif // __CLI_MENU__DATA_TPP__