#ifndef MERGESORT_HPP
#define MERGESORT_HPP

#include <iterator>
#include "BidirectionalList.hpp"

namespace {
  template< typename Iter, typename Compare >
  void merge(Iter first, Iter mid, Iter last, Compare cmp)
  {
    using value = typename Iter::value_type;
    Iter it1 = first;
    Iter it2 = mid;
    gorbunova::BidirectionalList< value > arr;
    while (it1 != mid && it2 != last) {
      if (cmp(*it1, *it2)) {
        arr.pushBack(*it1);
        ++it1;
      } else {
        arr.pushBack(*it2);
        ++it2;
      }
    }

    while (it2 != last) {
      arr.pushBack(*it2);
      ++it2;
    }

    while (it1 != mid) {
      arr.pushBack(*it1);
      ++it1;
    }

    auto bit = arr.begin();
    for (auto it = first ; it != last; ++it, ++bit){
      *it = *bit;
    }
  }
}

namespace gorbunova {

  template< typename Iter, typename Compare >
  void mergeSort(Iter first, Iter last, Compare cmp)
  {
    if (std::next(first) == last) {
      return;
    }

    Iter mid = std::next(first, std::distance(first, last) / 2);
    mergeSort(first, mid, cmp);
    mergeSort(mid, last, cmp);
    merge(first, mid, last, cmp);
  }

}


#endif