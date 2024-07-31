#ifndef __MINI_TOOLS__ALGORITHMS__SORTERS__BUBBLE_SORT_CPP__
#define __MINI_TOOLS__ALGORITHMS__SORTERS__BUBBLE_SORT_CPP__

namespace mini_tools {
namespace algorithms {
namespace sorters {

  template <typename T>
  void Bubble<T>::solve(std::vector<T> *messy, bool ascending) {
    int i, j;
    for (i = 0; i < messy->size() - 1; i++) {
      for (j = 0; j < messy->size() - i - 1; j++) {
        if ((ascending && messy->at(j) > messy->at(j + 1)) ||
          (!ascending && messy->at(j) < messy->at(j + 1))
        ) {
          std::swap(messy->at(j), messy->at(j + 1));
        }
      }
    }
  }

  template <typename T>
  std::vector<T> Bubble<T>::solve(std::vector<T> messy, bool ascending) {
    Bubble<T>::solve(&messy, ascending);
    return messy;
  }
}}}

#endif // __MINI_TOOLS__ALGORITHMS__SORTERS__BUBBLE_SORT_CPP__