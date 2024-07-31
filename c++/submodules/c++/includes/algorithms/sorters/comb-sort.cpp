#ifndef __MINI_TOOLS__ALGORITHMS__SORTERS__COMB_SORT_CPP__
#define __MINI_TOOLS__ALGORITHMS__SORTERS__COMB_SORT_CPP__

namespace mini_tools {
namespace algorithms {
namespace sorters {

  template <typename T>
  void Comb<T>::solve(std::vector<T> *messy, bool ascending) {
    if constexpr (CheckType::isNumber<T>()) {

      int gap = messy->size();
      bool swapped = true;
      int errCtr = 0;

      while (gap != 1 || swapped) {
        /**
        * Starts with a large value
        * and shrinks by a factor of 1.3
        */
        gap = (gap * 10) / 13;

        if (gap < 1) gap = 1;
        swapped = false;

        for (int i = 0; i < messy->size() - gap; i++) {
          if ((ascending && messy->at(i) > messy->at(i + gap)) ||
            (!ascending && messy->at(i) < messy->at(i + gap))
          ) {
            std::swap(messy->at(i), messy->at(i + gap));
            swapped = true;
          }
        }
      }
    }
  }

  template <typename T>
  std::vector<T> Comb<T>::solve(std::vector<T> messy, bool ascending) {
    Comb<T>::solve(&messy, ascending);
    return messy;
  }
}}}

#endif // __MINI_TOOLS__ALGORITHMS__SORTERS__COMB_SORT_CPP__