#ifndef LISTNODE_HPP
#define LISTNODE_HPP

namespace gorbunova
{
  template< typename T >
  struct ListNode
  {
    T data_;
    ListNode* next_;
    explicit ListNode(const T& newData):
      data_(newData),
      next_(nullptr)
    {}
  };
}

#endif
