#ifndef __MINI_TOOLS__ALGORITHMS__SORTERS__INSERTION_SORT_CPP__
#define __MINI_TOOLS__ALGORITHMS__SORTERS__INSERTION_SORT_CPP__

namespace mini_tools {
namespace algorithms {
namespace sorters {

  template <typename T>
  void Insertion<T>::solve(
    std::vector<T> *messy,
    bool ascending,
    int start, int end
  ) {
    int i, j;
    T key;

    if (start < 0) start = 0;
    if (end == -1 || end > messy->size()) end = messy->size();

    for (i = start + 1; i < end; i++) {
      j = i - 1;
      key = messy->at(i);

      while (j >= start && (
        (ascending && messy->at(j) > key) ||
        (!ascending && messy->at(j) < key)
      )) {
        messy->at(j + 1) = messy->at(j);
        j--;
      }

      messy->at(j + 1) = key;
    }
  }
  
  template <typename T>
  std::vector<T> Insertion<T>::solve(
    std::vector<T> messy,
    bool ascending,
    int start, int end
  ) {
    Insertion<T>::solve(&messy, ascending, start, end);
    return messy;
  }
}}}

#endif // __MINI_TOOLS__ALGORITHMS__SORTERS__INSERTION_SORT_CPP__