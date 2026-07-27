#ifndef __CLI_MENU__DATA_TPP__
#define __CLI_MENU__DATA_TPP__

namespace cli_menu {

  template <UNORMAP_COMVEC_TYPE T>
  T &Data::use() {
    if constexpr (std::is_same_v<T, TextMaps>) return textMaps;
    else if constexpr (std::is_same_v<T, WordMaps>) return wordMaps;
    else if constexpr (std::is_same_v<T, NumberMaps>) return numberMaps;
    else return booleanMaps;
  }

  template <UNORMAP_COMVEC_TYPE T>
  bool Data::has(Command *comkey) {
    T &unormap = use<T>();
    return unormap.comvec.find(comkey) != unormap.comvec.end();
  }

  template <UNORMAP_COMVEC_TYPE T>
  bool Data::isEmpty(Command *comkey) {
    bool haskey = has<T>(comkey);
    return !haskey || (haskey && use<T>().comvec[comkey].second.empty());
  }

  template <UNORMAP_COMVEC_TYPE T, PRIMITIVE_TYPE U>
  mt::VEC<U>::reference Data::get(Command *comkey) {
    T &unormap = use<T>();

    if (has<T>(comkey)) {
      return unormap.comvec[comkey].second[
        unormap.comvec[comkey].first
      ];
    }

    return unormap.defvec.back();
  }

  template <UNORMAP_COMVEC_TYPE T, PRIMITIVE_TYPE U>
  mt::VEC<U>::reference Data::get(
    Command *comkey,
    int &index
  ) {
    T &unormap = use<T>();

    if (has<T>(comkey)) {
      if (index < 0) {
        index = unormap.comvec[comkey].second.size() - index;
      }

      if (index < unormap.comvec[comkey].second.size()) {
        return unormap.comvec[comkey].second[index];
      }
    }

    return unormap.defvec.back();
  }

  template <UNORMAP_COMVEC_TYPE T, PRIMITIVE_TYPE U>
  void Data::set(
    Command *comkey,
    mt::CR<U> value
  ) {
    if (has<T>(comkey)) {
      T &unormap = use<T>();

      unormap.comvec[comkey].second[
        unormap.comvec[comkey].first
      ] = value;
    }
  }

  template <UNORMAP_COMVEC_TYPE T, PRIMITIVE_TYPE U>
  void Data::set(
    Command *comkey,
    mt::CR<U> value,
    int &index
  ) {
    if (has<T>(comkey)) {
      T &unormap = use<T>();

      if (index < 0) {
        index = unormap.comvec[comkey].second.size() - index;
      }

      if (index < unormap.comvec[comkey].second.size()) {
        unormap.comvec[comkey].second[index] = value;
      }
    }
  }

  template <UNORMAP_COMVEC_TYPE T, PRIMITIVE_TYPE U>
  void Data::replace(
    Command *comkey,
    mt::CR_VEC<U> values
  ) {
    if (has<T>(comkey)) {
      T &unormap = use<T>();
      unormap.comvec[comkey].first = 0;
      unormap.comvec[comkey].second = values;
    }
  }

  template <UNORMAP_COMVEC_TYPE T, PRIMITIVE_TYPE U>
  void Data::add(
    Command *comkey,
    mt::CR_VEC<U> values
  ) {
    if (has<T>(comkey)) {
      T &unormap = use<T>();

      mt_uti::VecTool<U>::concatCopy(
        unormap.comvec[comkey].second, values
      );
    }
  }

  template <UNORMAP_COMVEC_TYPE T, PRIMITIVE_TYPE U>
  void Data::push(
    Command *comkey,
    mt::CR<U> value
  ) {
    if (has<T>(comkey)) {
      use<T>().comvec[comkey].second.push_back(value);
    }
  }

  template <UNORMAP_COMVEC_TYPE T, PRIMITIVE_TYPE U>
  U Data::pop(Command *comkey) {
    T &unormap = use<T>();

    if (isEmpty<T>(comkey)) {
      return unormap.defvec.back();
    }

    U retval = unormap.comvec[comkey].second.back();
    unormap.comvec[comkey].second.pop_back();
    return retval;
  }

  template <UNORMAP_COMVEC_TYPE T>
  void Data::reset(Command *comkey) {
    T &unormap = use<T>();
    if (has<T>(comkey)) unormap.comvec[comkey].second.clear();
  }

  template <UNORMAP_COMVEC_TYPE T>
  void Data::select(
    Command *comkey,
    mt::CR_INT direction
  ) {
    if (direction != 0 && has<T>(comkey)) {

      T &unormap = use<T>();
      int valsign = std::abs(direction) / direction;
      unormap.comvec[comkey].first += direction;

      while (std::abs(unormap.comvec[comkey].first) >= unormap.comvec[comkey].second.size()) {
        unormap.comvec[comkey].first -= valsign * unormap.comvec[comkey].second.size();
      }
    }
  }

  template <UNORMAP_COMVEC_TYPE T>
  std::string Data::stringify(
    Command *comkey,
    mt::CR_STR separator
  ) {
    std::string text;
    T &unormap = use<T>();

    // booleans
    if constexpr (std::is_same_v<T, BooleanMaps>) {
      mt::PAIR<mt::VEC_STR> terms = Langu::ageBooleanizer::getTerms();

      // true term
      std::string trueTerm = "1";
      if (!terms.first.empty()) trueTerm = terms.first[0];

      // false term
      std::string falseTerm = "0";
      if (!terms.second.empty()) trueTerm = terms.second[0];

      for (const auto& [com, vec] : unormap.comvec) {
        for (mt::CR_BOL v : vec.second) {
          if (v) text += trueTerm + separator;
          else text += falseTerm + separator;
        }
      }
    }
    else for (const auto& [com, vec] : unormap.comvec) {
      // numbers
      if constexpr (std::is_same_v<T, NumberMaps>) {
        for (mt::CR_DBL v : vec.second) {
          text += std::to_string(v) + separator;
        }
      }
      // words
      else for (mt::CR_STR v : vec.second) {
        text += v + separator;
      }
    }

    return text;
  }

  template <UNORMAP_COMVEC_TYPE T>
  void Data::print(
    Command *comkey,
    mt::CR<CONSOLE_CODE> consoleCode,
    mt::CR_SZ numberOfIndents
  ) {
    if (has<T>(comkey)) {
      std::string indents = std::string(numberOfIndents, ' ');

      Console::logString(
        indents + stringify<T>(comkey, '\n' + indents),
        Console::messageColors[consoleCode]
      );
    }
  }
}

#endif // __CLI_MENU__DATA_TPP__