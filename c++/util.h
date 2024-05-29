#ifndef __CLI_MENU__UTIL_H__
#define __CLI_MENU__UTIL_H__

#include <string>
#include <vector>
#include <cctype>
#include <typeinfo>
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

    template <typename T> void cutVector(
      std::vector<T> *vec,
      int idx, bool ptrDel = true
    ) {
      if (ptrDel && std::is_pointer<T>::value) {
        delete vec->at(idx);
        vec->at(idx) = nullptr;
      }

      std::vector<T> tail = std::vector<T>(vec->begin() + idx + 1, vec->end());
      *vec = std::vector<T>(vec->begin(), vec->begin() + idx);
      vec->insert(vec->end(), tail.begin(), tail.end());
    }

    template <typename T> void cleanDuplicateInsideVector(std::vector<T> *vec) {
      for (int i = 0; i < vec->size(); i++) {
        for (int j = 0; j < vec->size(); j++) {
          if (i != j && vec->at(i) == vec->at(j)) {
            cutVector<T>(vec, j);
            j--;
          }
        }
      }
    }

    template <typename T> void cleanDuplicateToMember(
      std::vector<T> *vec, T mem,
      // repeated and compared 'T' parameters
      std::function<bool(T,T)> equalRule = [](T rep, T com)->bool { return false; }
    ) {
      bool first = false, isObject,
           isPointer = std::is_pointer<T>::value;

      int objectDigitIndex = 0;
      if (isPointer) objectDigitIndex = 1;

      isObject = std::isdigit(typeid(mem).name()[objectDigitIndex]);

      for (int i = 0; i < vec->size(); i++) {
        bool isEqual = vec->at(i) == mem;

        if (isObject && (isEqual || equalRule(vec->at(i), mem))) {
          bool ptrDel = true;
          if (isEqual && isPointer) ptrDel = false;
          if (first) cutVector<T>(vec, i, ptrDel);
          else first = true;
        }
        else if (!isObject && isEqual) {
          if (first) cutVector<T>(vec, i);
          else first = true;
        }
      }
    }

    std::vector<int> getToMostVectorDifferences(std::vector<int> sizes) {
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