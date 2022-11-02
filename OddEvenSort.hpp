#ifndef ODDEVENSORT_HPP
#define ODDEVENSORT_HPP

#include <iterator>

namespace gorbunova {

  template< typename Iter, typename Compare >
  void oddEvenSort(Iter first, Iter last, Compare cmp)
  {
    int size = std::distance(first, last);
    for (int i = 0; i < size; ++i) {
      Iter it = (i % 2 == 0) ? std::next(first, 2) : std::next(first);
      for (; it != last; it = (std::next(it) != last) ? std::next(it, 2) : last) {
        if (!cmp(*std::prev(it), *it)) {
          std::swap(*it, *std::prev(it));
        }
      }
    }
  }
}

#endif
