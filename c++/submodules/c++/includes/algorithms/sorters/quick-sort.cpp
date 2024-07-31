#ifndef __MINI_TOOLS__ALGORITHMS__SORTERS__QUICK_SORT_CPP__
#define __MINI_TOOLS__ALGORITHMS__SORTERS__QUICK_SORT_CPP__

#include <ctime>

namespace mini_tools {
namespace algorithms {
namespace sorters {

  template <typename T>
  int Quick<T>::hoarePartition(
    std::vector<T> *messy,
    CR_INT left,
    CR_INT right,
    bool &ascending
  ) {
    int i = left - 1, j = right + 1;

    while (true) {

      if (ascending) {
        do { i++; } while (messy->at(i) < messy->at(left));
        do { j--; } while (messy->at(j) > messy->at(left));
      }
      else {
        do { i++; } while (messy->at(i) > messy->at(left));
        do { j--; } while (messy->at(j) < messy->at(left));
      }

      if (i >= j) return j;
      std::swap(messy->at(i), messy->at(j));
    }
  }

  template <typename T>
  int Quick<T>::randomPartition(
    std::vector<T> *messy,
    CR_INT left,
    CR_INT right,
    bool &ascending
  ) {
    std::srand(std::time(NULL));
    int random = left + std::rand() % (right - left);
    std::swap(messy->at(random), messy->at(right));
    return Quick<T>::hoarePartition(messy, left, right, ascending);
  }

  template <typename T>
  void Quick<T>::recursion(
    std::vector<T> *messy,
    CR_INT left,
    CR_INT right,
    bool &ascending
  ) {
    if (left < right) {
      int piv = randomPartition(messy, left, right, ascending);
      Quick<T>::recursion(messy, left, piv - 1, ascending);
      Quick<T>::recursion(messy, piv + 1, right, ascending);
    }
  }

  template <typename T>
  void Quick<T>::solve(std::vector<T> *messy, bool ascending) {
    if constexpr (CheckType::isNumber<T>()) {
      Quick<T>::recursion(messy, 0, messy->size() - 1, ascending);
    }
  }

  template <typename T>
  std::vector<T> Quick<T>::solve(std::vector<T> messy, bool ascending) {
    solve(&messy, ascending);
    return messy;
  }
}}}

#endif // __MINI_TOOLS__ALGORITHMS__SORTERS__QUICK_SORT_CPP__