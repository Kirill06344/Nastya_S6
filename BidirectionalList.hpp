#ifndef BIDIRECTIONALLIST_HPP
#define BIDIRECTIONALLIST_HPP
#include <cassert>
#include <memory>

namespace gorbunova
{
  namespace detail
  {
    template< typename T >
    struct ListNode
    {
      T data_;
      ListNode* next_;
      ListNode* prev_;
      explicit ListNode(const T& newData):
        data_(newData),
        next_(nullptr),
        prev_(nullptr)
      {}
    };
  }

  template < typename T >
  class BidirectionalList
  {
    public:
    using Node = detail::ListNode< T >;
    class ConstIterator : public std::iterator< std::bidirectional_iterator_tag, T >
    {
      public:
      friend class BidirectionalList;
      ConstIterator():
        currentNode_(nullptr)
      {}
      ConstIterator(const ConstIterator& it):
        currentNode_(it.currentNode_)
      {}
      ~ConstIterator()
      {
        currentNode_ = nullptr;
      }
      ConstIterator& operator++()
      {
        assert(currentNode_ != nullptr);
        currentNode_ = currentNode_->next_;
        return *this;
      }
      ConstIterator operator++(int)
      {
        assert(currentNode_ != nullptr);
        ConstIterator temp(*this);
        ++(*this);
        return temp;
      }
      ConstIterator& operator--()
      {
        assert(currentNode_ != nullptr);
        currentNode_ = currentNode_->prev_;
        return *this;
      }
      ConstIterator operator--(int)
      {
        assert(currentNode_ != nullptr);
        ConstIterator temp(*this);
        --(*this);
        return temp;
      }
      bool operator==(const ConstIterator &anotherIt) const
      {
        return currentNode_ == anotherIt.currentNode_;
      }
      bool operator!=(const ConstIterator &anotherIt) const
      {
        return !(currentNode_ == anotherIt.currentNode_);
      }
      const T& operator*() const
      {
        assert(currentNode_ != nullptr);
        return currentNode_->data_;
      }
      const T* operator->() const
      {
        assert(currentNode_ != nullptr);
        return std::addressof(currentNode_->data_);
      }
      private:
      Node* currentNode_;
      explicit ConstIterator(Node* node):
        currentNode_(node)
      {}
    };

    class Iterator : public std::iterator< std::bidirectional_iterator_tag, T >
    {
      public:
      friend class BidirectionalList;
      Iterator():
        iter_()
      {}
      Iterator(ConstIterator iter):
        iter_(iter)
      {}
      Iterator& operator++()
      {
        ++iter_;
        return *this;
      }
      Iterator operator++(int)
      {
        return Iterator(iter_++);
      }
      Iterator& operator--()
      {
        --iter_;
        return *this;
      }
      Iterator operator--(int)
      {
        return Iterator(iter_--);
      }
      T& operator*()
      {
        return const_cast< T& >(*iter_);
      }
      T* operator->()
      {
        return std::addressof(const_cast< T& >(*iter_));
      }
      const T& operator*() const
      {
        return *iter_;
      }
      const T * operator->() const
      {
        return std::addressof(*iter_);
      }
      bool operator==(const Iterator& rhs) const
      {
        return iter_ == rhs.iter_;
      }
      bool operator!=(const Iterator& rhs) const
      {
        return !(rhs == *this);
      }
      private:
      ConstIterator iter_;
    };

    BidirectionalList();
    BidirectionalList(const BidirectionalList< T >&);
    BidirectionalList(BidirectionalList< T >&&) noexcept;
    ~BidirectionalList();

    BidirectionalList< T >& operator=(const BidirectionalList< T >&);
    BidirectionalList< T >& operator=(BidirectionalList< T >&&) noexcept;

    void swap(BidirectionalList< T >& rhs) noexcept;
    void clear();
    T& getFront();
    T& getBack();
    size_t getSize() const;
    bool isEmpty() const noexcept;

    void pushFront(const T& newData);
    void pushBack(const T& newData);
    void push(Iterator it, const T& newData);

    void popFront();
    void popBack();
    void pop(Iterator it);

    Iterator find(const T& data);

    Iterator begin()
    {
      return ConstIterator(head_);
    }
    Iterator end()
    {
      return ConstIterator(nullptr);
    }
    ConstIterator begin() const
    {
      return ConstIterator(head_);
    }
    ConstIterator end() const
    {
      return ConstIterator(nullptr);
    }
    private:
    Node* head_;
    Node* tail_;
    size_t size_;
  };

  template< typename T >
  BidirectionalList< T >::BidirectionalList():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< typename T >
  BidirectionalList< T >::BidirectionalList(const BidirectionalList< T >& rhs):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    Node* tmpHead = rhs.head_;
    while (tmpHead != nullptr)
    {
      this->pushBack(tmpHead->data_);
      tmpHead = tmpHead->next_;
    }
  }

  template< typename T >
  BidirectionalList< T >::BidirectionalList(BidirectionalList< T >&& rhs) noexcept:
    head_(rhs.head_),
    tail_(rhs.tail_),
    size_(rhs.size_)
  {
    rhs.tail_ = nullptr;
    rhs.head_ = nullptr;
    size_ = 0;
  }

  template< typename T >
  BidirectionalList< T >::~BidirectionalList()
  {
    clear();
  }

  template< typename T >
  BidirectionalList< T >& BidirectionalList< T >::operator=(const BidirectionalList< T >& rhs)
  {
    if (this != std::addressof(rhs))
    {
      BidirectionalList< T > temp(rhs);
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  BidirectionalList< T >& BidirectionalList< T >::operator=(BidirectionalList< T >&& rhs) noexcept
  {
    if (this != std::addressof(rhs))
    {
      BidirectionalList< T > temp(std::move(rhs));
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  void BidirectionalList< T >::swap(BidirectionalList< T >& rhs) noexcept
  {
    std::swap(head_, rhs.head_);
    std::swap(tail_, rhs.tail_);
    std::swap(size_, rhs.size_);
  }

  template< typename T >
  void BidirectionalList< T >::clear()
  {
    while (size_ != 0)
    {
      popFront();
    }
  }

  template< typename T >
  T& BidirectionalList< T >::getFront()
  {
    if (size_ == 0)
    {
      throw std::logic_error("The list is empty");
    }
    else
    {
      return head_->data_;
    }
  }

  template< typename T >
  T& BidirectionalList< T >::getBack()
  {
    if (size_ == 0)
    {
      throw std::logic_error("The list is empty");
    }
    else
    {
      return tail_->data_;
    }
  }

  template< typename T >
  size_t BidirectionalList< T >::getSize() const
  {
    return size_;
  }

  template< typename T >
  bool BidirectionalList< T >::isEmpty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  void BidirectionalList< T >::push(Iterator it, const T& data)
  {
    if (it == begin())
    {
      pushFront(data);
    }
    else if (it == end())
    {
      pushBack(data);
    }
    else
    {
      Node* node = new Node(data);
      Node* previous = it.currentNode_->prev_;
      previous->next_->prev_ = node;
      node->prev_ = previous;
      node->next_ = previous->next_;
      previous->next_ = node;
      size_++;
    }
  }

  template< typename T >
  void BidirectionalList< T >::pushFront(const T& newData)
  {
    Node* newNode = new Node(newData);
    if (!head_)
    {
      head_ = newNode;
      tail_ = newNode;
      size_++;
      return;
    }
    newNode->next_ = head_;
    head_->prev_ = newNode;
    head_ = newNode;
    size_++;
  }

  template< typename T >
  void BidirectionalList< T >::pushBack(const T& newData)
  {
    Node* newNode = new Node(newData);
    if (head_ == nullptr)
    {
      head_ = newNode;
      tail_ = newNode;
    }
    else
    {
      tail_->next_ = newNode;
      newNode->prev_ = tail_;
      tail_ = newNode;
    }
    size_++;
  }

  template< typename T >
  void BidirectionalList< T >::popFront()
  {
    if (isEmpty())
    {
      throw std::logic_error("The list is empty");
    }
    if (!head_->next_)
    {
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
    }
    else
    {
      Node* temp = head_;
      head_ = head_->next_;
      delete temp;
      head_->prev_ = nullptr;
    }
    size_--;
  }

  template< typename T >
  void BidirectionalList< T >::popBack()
  {
    if (isEmpty())
    {
      throw std::logic_error("The list is empty");
    }
    else if (!head_->next_)
    {
      popFront();
    }
    else
    {
      Node* temp = tail_;
      tail_ = tail_->prev_;
      tail_->next_ = nullptr;
      delete temp;
      size_--;
    }
  }

  template< typename T >
  void BidirectionalList< T >::pop(Iterator it)
  {
    if (isEmpty())
    {
      throw std::logic_error("The list is empty");
    }
    if (it == begin())
    {
      popFront();
    }
    else
    {
      Node* current = it.currentNode_;
      Node* previous = current->prev_;
      if (current->next_ == nullptr)
      {
        previous->next_ = nullptr;
        tail_ = previous;
      }
      else
      {
        (current->next_)->prev_ = previous;
        previous->next_ = current->next_;
      }
      delete current;
      size_--;
    }
  }

  template< typename T >
  typename BidirectionalList< T >::Iterator BidirectionalList< T >::find(const T& data)
  {
    auto iter = begin();
    while (iter != end())
    {
      if (*iter == data)
      {
        return iter;
      }
      ++iter;
    }
    return iter;;
  }
}

#endif
