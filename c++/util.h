#ifndef __CLI_MENU__UTIL_H__
#define __CLI_MENU__UTIL_H__

#include <string>
#include <vector>
#include <functional>
#include <type_traits>

namespace cli_menu {

  // shortened types
  typedef long long unsigned int LLUI;
  typedef std::vector<std::string> VEC_STR;
  typedef std::vector<double> VEC_DOU;
  typedef std::vector<float> VEC_FLO;
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

    std::vector<LLUI> getDifferencesToSize(
      std::vector<LLUI> sizes,
      LLUI targetSize
    ) {
      LLUI max = targetSize;

      for (auto &sz : sizes) {
        if (targetSize > 0) {
          if (sz > max && sz <= targetSize) max = sz;
        }
        else if (sz > max) max = sz;
      }

      std::vector<LLUI> differences;
      for (auto &sz : sizes) { differences.push_back(max - sz); }
      
      return differences;
    }

    namespace { namespace SECRET {
      void changeStringCase(std::string &str, bool isUpper) {
        std::transform(
          str.begin(),
          str.end(),
          str.begin(),
          [&](unsigned char c) {
            if (isUpper) return std::toupper(c);
            else return std::tolower(c);
          }
        );
      }

      void changeStringsCase(VEC_STR *vecStr, bool isUpper) {
        for (int i = 0; i < vecStr->size(); i++) {
          changeStringCase(vecStr->at(i), isUpper);
        }
      }
    }}

    void stringToLowercase(std::string str) {
      SECRET::changeStringCase(str, false);
    }

    void stringToUppercase(std::string str) {
      SECRET::changeStringCase(str, true);
    }

    void stringsToLowercase(VEC_STR *vecStr) {
      SECRET::changeStringsCase(vecStr, false);
    }

    void stringsToUppercase(VEC_STR *vecStr) {
      SECRET::changeStringsCase(vecStr, true);
    }
  };
}

#endif // __CLI_MENU__UTIL_H__