#ifndef SORTCOLLECTIONS_HPP
#define SORTCOLLECTIONS_HPP

#include <cstdlib>
#include <deque>
#include <functional>
#include "QuickSort.hpp"
#include "OddEvenSort.hpp"
#include "MergeSort.hpp"
#include "ForwardList.hpp"
#include "BidirectionalList.hpp"

namespace {
  template< typename T >
  T getRandom()
  {
    return static_cast< T >((std::rand() * 100.0 / RAND_MAX));
  }

  template< typename Iterator >
  std::ostream& printCollection(std::ostream& out, Iterator begin, Iterator end)
  {
    for (auto it = begin; it != end;) {
      out << *it;
      if (++it != end) {
        out << ' ';
      }
    }
    return out;
  }

  template< typename T >
  void fillBidirectional(gorbunova::BidirectionalList< T >& b, std::deque< T >& d)
  {
    for (auto it = d.begin(); it != d.end(); ++it) {
      b.pushBack(*it);
    }
  }

  template< typename T >
  void fillForward(gorbunova::ForwardList< T >& b, std::deque< T >& d)
  {
    for (auto it = d.begin(); it != d.end(); ++it) {
      b.pushBack(*it);
    }
  }
}


namespace gorbunova {
  template< typename T >
  class SortCollections {
    public:
    SortCollections() = delete;

    SortCollections(size_t size,const std::string& direction)
    {
      if (direction == "ascending") {
        cmp = std::less<>();
      } else if (direction == "descending") {
        cmp = std::greater<>();
      } else {
        throw std::logic_error("Invalid direction!");
      }
      for (size_t i = 0; i < size; ++i) {
        oddEven1.push_back(getRandom< T >());
      }
      merge1.insert(merge1.begin(), oddEven1.begin(), oddEven1.end());
      quick1.insert(quick1.begin(), oddEven1.begin(), oddEven1.end());
      fillBidirectional(quick2, oddEven1);
      fillBidirectional(oddEven2, oddEven1);
      fillForward(quick3, oddEven1);
    }

    std::ostream& printCollections(std::ostream& out)
    {
      printCollection(out, oddEven1.begin(), oddEven1.end()) << "\n";
      sortCollections();
      printCollection(out, oddEven1.begin(), oddEven1.end()) << "\n";
      printCollection(out, oddEven2.begin(), oddEven2.end()) << "\n";
      printCollection(out, quick1.begin(), quick1.end()) << "\n";
      printCollection(out, quick2.begin(), quick2.end()) << "\n";
      printCollection(out, quick3.begin(), quick3.end()) << "\n";
      return printCollection(out, merge1.begin(), merge1.end());
    }

    private:
    std::deque< T > oddEven1;
    std::deque< T > merge1;
    std::deque< T > quick1;
    gorbunova::BidirectionalList< T > oddEven2;
    gorbunova::BidirectionalList< T > quick2;
    gorbunova::ForwardList< T > quick3;
    std::function< bool(T, T) > cmp;

    void sortCollections() {
      gorbunova::oddEvenSort(oddEven1.begin(), oddEven1.end(), cmp);
      gorbunova::oddEvenSort(oddEven2.begin(), oddEven2.end(), cmp);
      gorbunova::quickSort(quick1.begin(), quick1.end(), cmp);
      gorbunova::quickSort(quick2.begin(), quick2.end(), cmp);
      gorbunova::quickSort(quick3.begin(), quick3.end(), cmp);
      gorbunova::mergeSort(merge1.begin(), merge1.end(), cmp);
    }
  };
}

#endif