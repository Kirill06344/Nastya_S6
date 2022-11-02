#ifndef FORWARDLIST_HPP
#define FORWARDLIST_HPP

#include <memory>
#include <cassert>
#include <stdexcept>
#include "ListNode.hpp"

namespace gorbunova {
  template< typename T >
  class ForwardList {
    public:
    using Node = ListNode< T >;

    class Iterator: public std::iterator< std::forward_iterator_tag, T > {
      public:
      friend class ForwardList;

      Iterator():
        currentNode_(nullptr)
      {}

      Iterator(const Iterator& it):
        currentNode_(it.currentNode_)
      {}

      ~Iterator()
      {
        currentNode_ = nullptr;
      }

      Iterator& operator=(const Iterator& it)
      {
        currentNode_ = it.currentNode_;
        return *this;
      }

      Iterator& operator++()
      {
        assert(currentNode_ != nullptr);
        currentNode_ = currentNode_->next_;
        return *this;
      }

      Iterator operator++(int)
      {
        assert(currentNode_ != nullptr);
        Iterator temp(*this);
        ++(*this);
        return temp;
      }

      bool operator==(const Iterator& it)
      {
        return currentNode_ == it.currentNode_;
      }

      bool operator!=(const Iterator& it)
      {
        return !(currentNode_ == it.currentNode_);
      }

      T& operator*()
      {
        assert(currentNode_ != nullptr);
        return currentNode_->data_;
      }

      T* operator->()
      {
        assert(currentNode_ != nullptr);
        return std::addressof(currentNode_->data_);
      }

      private:
      Node* currentNode_;

      explicit Iterator(Node* node):
        currentNode_(node)
      {}
    };

    ForwardList();
    ForwardList(const ForwardList< T >&);
    ForwardList(ForwardList< T >&&) noexcept;
    ~ForwardList();
    ForwardList< T >& operator=(const ForwardList< T >&);
    ForwardList< T >& operator=(ForwardList< T >&&) noexcept;
    void swap(ForwardList< T >& rhs) noexcept;
    void clear();
    T& getFront();
    size_t getSize();
    bool isEmpty() const noexcept;
    void pushFront(const T& newData);
    void pushBack(const T& newData);
    void pushAfter(Iterator it, const T& newData);
    void popFront();
    void pop(Iterator it);
    Iterator begin()
    {
      return Iterator(head_);
    }
    Iterator end()
    {
      return Iterator(nullptr);
    }

    private:
    Node* head_;
    Node* tail_;
    size_t size_;
  };

  template< typename T >
  ForwardList< T >::ForwardList():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< typename T >
  ForwardList< T >::ForwardList(const ForwardList< T >& rhs):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    Node* tmp_head = rhs.head_;
    while (tmp_head != nullptr) {
      this->pushBack(tmp_head->data_);
      tmp_head = tmp_head->next_;
    }
  }

  template< typename T >
  ForwardList< T >::ForwardList(ForwardList< T >&& rhs) noexcept:
    head_(rhs.head_),
    tail_(rhs.tail_),
    size_(rhs.size_)
  {
    rhs.tail_ = nullptr;
    rhs.head_ = nullptr;
    size_ = 0;
  }

  template< typename T >
  ForwardList< T >::~ForwardList()
  {
    clear();
  }

  template< typename T >
  ForwardList< T >& ForwardList< T >::operator=(const ForwardList< T >& rhs)
  {
    if (this != std::addressof(rhs)) {
      ForwardList< T > temp(rhs);
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  ForwardList< T >& ForwardList< T >::operator=(ForwardList< T >&& rhs) noexcept
  {
    if (this != std::addressof(rhs)) {
      ForwardList< T > temp(std::move(rhs));
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  void ForwardList< T >::swap(ForwardList< T >& rhs) noexcept
  {
    std::swap(head_, rhs.head_);
    std::swap(tail_, rhs.tail_);
    std::swap(size_, rhs.size_);
  }


  template< typename T >
  void ForwardList< T >::clear()
  {
    while (size_ != 0) {
      popFront();
    }
  }

  template< typename T >
  T& ForwardList< T >::getFront()
  {
    if (size_ == 0) {
      throw std::logic_error("The list is empty");
    } else {
      return head_->data_;
    }
  }

  template< typename T >
  size_t ForwardList< T >::getSize()
  {
    return size_;
  }

  template< typename T >
  bool ForwardList< T >::isEmpty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  void ForwardList< T >::pushAfter(Iterator it, const T& data)
  {
    if (it == begin()) {
      pushFront(data);
    } else if (it == end()) {
      pushBack(data);
    } else {
      Iterator temp = begin();
      Node* current = head_;
      ++temp;
      while (temp != it) {
        ++temp;
        current = current->next_;
      }
      Node* node = new Node(data);
      node->next_ = current->next_;
      current->next_ = node;
      size_++;
    }
  }

  template< typename T >
  void ForwardList< T >::pushFront(const T& newData)
  {
    Node* newNode = new Node(newData);
    if (!head_) {
      head_ = newNode;
      tail_ = newNode;
      size_++;
      return;
    }
    newNode->next_ = head_;
    head_ = newNode;
    size_++;
  }

  template< typename T >
  void ForwardList< T >::pushBack(const T& newData)
  {
    Node* newNode = new Node(newData);
    if (head_ == nullptr) {
      head_ = newNode;
      tail_ = newNode;
    } else {
      tail_->next_ = newNode;
      tail_ = newNode;
    }
    size_++;
  }

  template< typename T >
  void ForwardList< T >::popFront()
  {
    if (isEmpty()) {
      throw std::logic_error("The list is empty");
    }
    if (!head_->next_) {
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
    } else {
      Node* temp = head_;
      head_ = head_->next_;
      delete temp;
    }
    size_--;
  }

  template< typename T >
  void ForwardList< T >::pop(Iterator it)
  {
    if (isEmpty()) {
      throw std::logic_error("The list is empty");
    }
    if (it == begin()) {
      popFront();
    } else {
      Node* prev = head_;
      Iterator temp = begin();
      while (++temp != it) {
        prev = prev->next_;
      }
      Node* rem = prev->next_;
      prev->next_ = rem->next_;
      delete rem;
      size_--;
    }
  }
}

#endif