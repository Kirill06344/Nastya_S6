#ifndef QUICKSORT_HPP
#define QUICKSORT_HPP
#include <iterator>

namespace {
  template< typename Iter, typename Compare >
  Iter partition(Iter first, Iter last, Compare cmp) {
    Iter pivot = first;
    Iter i = first;
    for (auto j = std::next(i); j != last; ++j) {
      if (cmp(*j, *pivot)) {
        std::swap(*j, *(++i));
      }
    }
    std::swap(*i, *first);
    return i;
  }
}

namespace gorbunova {

  template< typename Iter, typename Compare >
  void quickSort(Iter first, Iter last, Compare cmp)
  {
    if (first == last || std::next(first) == last) {
      return;
    }

    Iter pivot = partition(first, last, cmp);
    quickSort(first, pivot, cmp);
    quickSort(std::next(pivot), last, cmp);
  }
}

#endif