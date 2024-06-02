#ifndef __CLI_MENU__UTIL_H__
#define __CLI_MENU__UTIL_H__

#include <tuple>
#include <string>
#include <vector>
#include <functional>
#include <type_traits>

namespace cli_menu {

  // shortened types
  typedef std::vector<std::string> VEC_STR;
  typedef std::vector<double> VEC_DOU;
  typedef std::vector<bool> VEC_BOO;
  typedef std::vector<int> VEC_INT;

  namespace Util {
    template <typename T> bool isIndexOfVector(std::vector<T> *vec, int idx) {
      if (idx < vec->size() && idx >= 0) return true;
      return false;
    }

    template <typename T> T getIndexOfVector(std::vector<T> *vec, int idx, T defaultReturn) {
      if (isIndexOfVector<T>(vec, idx)) return vec->at(idx);
      return defaultReturn;
    }

    template <typename T> T cutVector(std::vector<T> *vec, int idx) {
      T wasted = vec->at(idx);
      std::vector<T> tail = std::vector<T>(vec->begin() + idx + 1, vec->end());
      *vec = std::vector<T>(vec->begin(), vec->begin() + idx);
      vec->insert(vec->end(), tail.begin(), tail.end());
      return wasted;
    }

    template <typename T> std::vector<T> cleanDuplicateInsideVector(
      std::vector<T> *vec,
      // repeated and compared 'T' parameters
      std::function<bool(T,T)> equalRule = [](T rep, T com)->bool { return false; }
    ) {
      std::vector<T> wastedVec;
      bool first = false;

      for (int i = 0; i < vec->size(); i++) {
        for (int j = 0; j < vec->size(); j++) {
          bool isEqual = vec->at(i) == vec->at(j);

          if (i != j && (isEqual || equalRule(vec->at(i), vec->at(j)) )) {

            // same address
            if (isEqual && std::is_pointer<T>::value) {
              cutVector<T>(vec, j);
            }
            // same value
            else wastedVec.push_back(cutVector<T>(vec, j));

            j--;
          }
        }
      }

      return wastedVec;
    }

    template <typename T> std::vector<T> cleanDuplicateToMember(
      std::vector<T> *vec, T mem,
      // repeated and compared 'T' parameters
      std::function<bool(T,T)> equalRule = [](T rep, T com)->bool { return false; }
    ) {
      std::vector<T> wastedVec;
      bool first = false;

      for (int i = 0; i < vec->size(); i++) {
        bool isEqual = vec->at(i) == mem;

        if (isEqual || equalRule(vec->at(i), mem)) {

          if (first) {
            // same address
            if (isEqual && std::is_pointer<T>::value) {
              cutVector<T>(vec, i);
            }
            // same value
            else wastedVec.push_back(cutVector<T>(vec, i));
          }
          else first = true;
        }
      }

      return wastedVec;
    }

    std::vector<int> getDifferencesToBiggestVector(std::vector<int> sizes) {
      int max = 0;
      for (auto &sz : sizes) if (sz > max) max = sz;

      std::vector<int> differences;
      for (auto &sz : sizes) { differences.push_back(max - sz); }
      
      return differences;
    }

    void changeStringsCase(VEC_STR *vecStr, bool isUpper) {
      for (int i = 0; i < vecStr->size(); i++) {
        std::transform(
          vecStr->at(i).begin(),
          vecStr->at(i).end(),
          vecStr->at(i).begin(),
          [&](unsigned char c) {
            if (isUpper) return std::toupper(c);
            else return std::tolower(c);
          }
        );
      }
    }

    void stringsToLowercase(VEC_STR *vecStr) {
      changeStringsCase(vecStr, false);
    }

    void stringsToUppercase(VEC_STR *vecStr) {
      changeStringsCase(vecStr, true);
    }
  };
}

#endif // __CLI_MENU__UTIL_H__