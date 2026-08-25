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
  void Data::ragister(Command *comkey) {
    T &unormap = use<T>();
    unormap.comvec[comkey] = {0, {}};
  }

  template <UNORMAP_COMVEC_TYPE T>
  void Data::unragister(Command *comkey) {
    T &unormap = use<T>();
    unormap.comvec.erase(comkey);
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
  mt::VEC<U>::reference Data::getValue(Command *comkey) {
    T &unormap = use<T>();

    if (has<T>(comkey) &&
      unormap.comvec[comkey].first < unormap.comvec[comkey].second.size()
    ) {
      return unormap.comvec[comkey].second[
        unormap.comvec[comkey].first
      ];
    }

    return unormap.defvec.back();
  }

  template <UNORMAP_COMVEC_TYPE T, PRIMITIVE_TYPE U>
  mt::VEC<U>::reference Data::getValue(
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
  mt::VEC<U> Data::getVector(Command *comkey) {
    if (has<T>(comkey)) {
      return use<T>().comvec[comkey].second;
    }
    return {};
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
  bool Data::select(
    Command *comkey,
    mt::CR_INT direction
  ) {
    if (has<T>(comkey)) {
      T &unormap = use<T>();

      // the vector cannot be empty, but the direction can be empty
      if (!unormap.comvec[comkey].second.empty()) {
        unormap.comvec[comkey].first += direction;
        mt::LLI vecsz = static_cast<mt::LLI>(unormap.comvec[comkey].second.size());

        // positive wrap-around
        while (unormap.comvec[comkey].first >= vecsz) {
          unormap.comvec[comkey].first -= vecsz;
        }

        // negative wrap-around
        while (unormap.comvec[comkey].first < 0) {
          unormap.comvec[comkey].first += vecsz;
        }

        return true;
      }
    }

    return false;
  }

  template <UNORMAP_COMVEC_TYPE T>
  mt::VEC_STR Data::vectorStringify(Command *comkey) {
    mt::VEC_STR vecstr;
    T &unormap = use<T>();

    // booleans
    if constexpr (std::is_same_v<T, BooleanMaps>) {
      mt::PAIR<mt::VEC_STR> terms = Langu::ageBooleanizer::getTerms();

      // stringified truthy
      std::string trueTerm = "1";
      if (!terms.first.empty()) trueTerm = terms.first[0];

      // stringified falsy
      std::string falseTerm = "0";
      if (!terms.second.empty()) falseTerm = terms.second[0];

      for (int i = 0; i < unormap.comvec[comkey].second.size(); i++) {

        // select one of the stringified terms
        if (unormap.comvec[comkey].second[i]) vecstr.push_back(trueTerm);
        else vecstr.push_back(falseTerm);
      }
    }
    // numbers
    else if constexpr (std::is_same_v<T, NumberMaps>) {
      for (int i = 0; i < unormap.comvec[comkey].second.size(); i++) {

        // firstly convert the member to string
        vecstr.push_back(std::to_string(
          unormap.comvec[comkey].second[i]
        ));
      }
    }
    // words or texts
    else for (int i = 0; i < unormap.comvec[comkey].second.size(); i++) {

      // disable newline and carriage return escape characters
      vecstr.push_back(mt_uti::StrTool::deactivateNewlines(
        unormap.comvec[comkey].second[i]
      ));
    }

    return vecstr;
  }

  template <UNORMAP_COMVEC_TYPE T>
  std::string Data::stringify(
    Command *comkey,
    mt::CR_STR separator
  ) {
    if (has<T>(comkey)) {
      return mt_uti::StrTool::joinVector(
        vectorStringify<T>(comkey), separator
      );
    }
    return "";
  }

  template <UNORMAP_COMVEC_TYPE T>
  std::string Data::joinColorize(
    Command *comkey,
    mt::CR_VEC_STR vecstr,
    mt::CR<ConlorHighlightCodeSet> conlorSet,
    mt::CR_STR separator
  ) {
    T &unormap = use<T>();
    std::string text;
    bool isSelected;

    for (int i = 0; i < vecstr.size(); i++) {
      isSelected = i == unormap.comvec[comkey].first;

      // combining the members into a string
      text += Color::getString(
        vecstr[i] + separator,
        Console::messageColors[conlorSet[isSelected]]
      );
    }

    return text;
  }

  template <UNORMAP_COMVEC_TYPE T>
  void Data::print(
    Command *comkey,
    mt::CR<Console::Indent> indent,
    mt::CR<ConlorHighlightCodeSet> conlorSet
  ) {
    if (has<T>(comkey)) {
      int index = 0;
      std::string text = indent.get();

      // data is empty
      if (use<T>().comvec[comkey].second.empty()) {
        index = 1;

        text += Color::getString(
          "...\n", Console::messageColors[CONLOR_TITLE]
        );
      }
      else { // data is exist
        text += joinColorize<T>(
          comkey, vectorStringify<T>(comkey), conlorSet, '\n' + indent.get()
        );
      }

      // print the stringified out
      std::cout << text;
    }
  }

  template <UNORMAP_COMVEC_TYPE T>
  void Data::copyInputToOutput(Command *comkey) {
    if (has<T>(comkey)) {
      resetTexts(comkey);
      addTexts(comkey, vectorStringify<T>(comkey));
    }
  }
}

#endif // __CLI_MENU__DATA_TPP__