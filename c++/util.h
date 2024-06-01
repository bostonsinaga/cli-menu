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

    namespace {
      namespace SECRET {
        // calculate differences to the most vector
        std::vector<int> continueDifferencesToBiggestVector(
          int indexSize, bool calculate, bool reload
        ) {
          static std::vector<int> sizes;
          if (reload) sizes = {};
          sizes.push_back(indexSize);

          if (calculate) {
            int max = 0;
            for (auto &sz : sizes) { if (sz > max) max = sz; }
            std::vector<int> differences;
            for (auto &sz : sizes) { differences.push_back(max - sz); }
            return differences;
          }
          
          return std::vector<int>{};
        }

        // helper meta function to convert a tuple to a tuple of vectors
        template <typename... Types>
        std::tuple<std::vector<Types>...> tuple_to_tuple_of_vectors(std::tuple<Types...>);
      }
    }

    /**
    * Templated class to represent vertices.
    * Aims to hold tuple of vectors.
    */
    template <typename Tuple>
    class Vertices {
    public:
      using tuple_type = decltype(tuple_to_tuple_of_vectors(std::declval<Tuple>()));
      tuple_type v;
    };

    namespace {
      namespace SECRET {
        /** Continuation of 'offsetBiggestVector' */
        
        template<std::size_t I = 0, typename... T>
        inline typename std::enable_if<I == sizeof...(T), void>::type
        pushOffsetBiggestVector(
          Vertices<std::tuple<T...>> *tupVecs,
          std::tuple<T...> *tupDefVals,
          std::vector<int> &differences
        ) {}

        template<std::size_t I = 0, typename... T>
        inline typename std::enable_if<I < sizeof...(T), void>::type
        pushOffsetBiggestVector(
          Vertices<std::tuple<T...>> *tupVecs,
          std::tuple<T...> *tupDefVals,
          std::vector<int> &differences
        ) {
          if (I < sizeof...(T) - 1) {
            for (auto &dif : differences) {
              for (int i = 0; i < dif; i++) {
                std::get<I>(tupVecs->v).push_back(std::get<I>(*tupDefVals));
              }
            }
          }

          pushOffsetBiggestVector<I + 1, T...>(tupVecs, tupDefVals, differences);
        }
      }
    }

    template<std::size_t I = 0, typename... T>
    inline typename std::enable_if<I == sizeof...(T), void>::type
    offsetBiggestVector(
      Vertices<std::tuple<T...>> *tupVecs,
      std::tuple<T...> *tupDefVals
    ) {
      std::vector<int> differences = SECRET::continueDifferencesToBiggestVector(
        std::get<I - 1>(tupVecs->v).size(), true, false
      );

      SECRET::pushOffsetBiggestVector<0, T...>(tupVecs, tupDefVals, differences);
    }

    /**
    * Usage example:
    *
    *   Vertices<std::tuple<int, std::string, char>> tupVecs;
    *   std::tuple<int, std::string, char> tupDefVals = std::make_tuple(0, "", '\0');
    *
    *   std::get<0>(tupVecs.v) = {1, 2, 3};
    *   std::get<1>(tupVecs.v) = {"one", "two", "three"};
    *   std::get<2>(tupVecs.v) = {'a', 'b', 'c'};
    *
    *   offsetBiggestVector<0, int, std::string, char>(&tupVecs, &tupDefVals);
    */
    template<std::size_t I = 0, typename... T>
    inline typename std::enable_if<I < sizeof...(T), void>::type
    offsetBiggestVector(
      Vertices<std::tuple<T...>> *tupVecs,
      std::tuple<T...> *tupDefVals
    ) {
      /** 'tupVecs->v' as a tuple of vectors */

      if (I < sizeof...(T) - 1) {
        SECRET::continueDifferencesToBiggestVector(std::get<I>(tupVecs->v).size(), false, I == 0);
      }

      offsetBiggestVector<I + 1, T...>(tupVecs, tupDefVals);
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