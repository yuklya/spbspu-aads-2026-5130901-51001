#ifndef LOSEVA_LIST_HPP
#define LOSEVA_LIST_HPP

#include <cstddef>

namespace loseva
{

template<class T>
class List;

template<class T>
class LIter
{
  friend class List<T>;

private:
  struct Node
  {
    T data;
    Node* next;

    Node(const T& d, Node* n = nullptr):
      data(d),
      next(n)
    {}
  };

  Node* node_;

  explicit LIter(Node* node):
    node_(node)
  {}

public:
  LIter():
    node_(nullptr)
  {}

  T& operator*()
  {
    return node_->data;
  }

  LIter& operator++()
  {
    node_ = node_->next;
    return *this;
  }

  bool operator==(const LIter& other) const
  {
    return node_ == other.node_;
  }

  bool operator!=(const LIter& other) const
  {
    return node_ != other.node_;
  }
};


template<class T>
class LCIter
{
  friend class List<T>;

private:
  typename LIter<T>::Node* node_;

  explicit LCIter(typename LIter<T>::Node* node):
    node_(node)
  {}

public:
  LCIter():
    node_(nullptr)
  {}

  const T& operator*() const
  {
    return node_->data;
  }

  LCIter& operator++()
  {
    node_ = node_->next;
    return *this;
  }

  bool operator==(const LCIter& other) const
  {
    return node_ == other.node_;
  }

  bool operator!=(const LCIter& other) const
  {
    return node_ != other.node_;
  }
};


template<class T>
class List
{
private:

  using Node = typename LIter<T>::Node;

  Node* head_;

  Node* copyNodes(Node* other)
  {
    if (!other)
      return nullptr;

    Node* newHead = new Node(other->data);
    Node* cur = newHead;
    Node* src = other->next;

    while (src)
    {
      cur->next = new Node(src->data);
      cur = cur->next;
      src = src->next;
    }

    return newHead;
  }

public:

  using iterator = LIter<T>;
  using const_iterator = LCIter<T>;

  List():
    head_(nullptr)
  {}

  ~List()
  {
    clear();
  }

  List(const List& other):
    head_(copyNodes(other.head_))
  {}

  List(List&& other) noexcept:
    head_(other.head_)
  {
    other.head_ = nullptr;
  }

  List& operator=(const List& other)
  {
    if (this != &other)
    {
      clear();
      head_ = copyNodes(other.head_);
    }

    return *this;
  }

  List& operator=(List&& other) noexcept
  {
    if (this != &other)
    {
      clear();
      head_ = other.head_;
      other.head_ = nullptr;
    }

    return *this;
  }

  bool empty() const
  {
    return head_ == nullptr;
  }

  iterator begin()
  {
    return iterator(head_);
  }

  iterator end()
  {
    return iterator(nullptr);
  }

  const_iterator begin() const
  {
    return const_iterator(head_);
  }

  const_iterator end() const
  {
    return const_iterator(nullptr);
  }

  T& front()
  {
    return head_->data;
  }

  const T& front() const
  {
    return head_->data;
  }

  void push_front(const T& value)
  {
    head_ = new Node(value, head_);
  }

  void pop_front()
  {
    if (!head_)
      return;

    Node* tmp = head_;
    head_ = head_->next;
    delete tmp;
  }

  iterator insert_after(iterator pos, const T& value)
  {
    if (!pos.node_)
      return end();

    Node* n = new Node(value, pos.node_->next);
    pos.node_->next = n;

    return iterator(n);
  }

  void push_back(const T& value)
  {
    if (!head_)
    {
      push_front(value);
      return;
    }

    Node* cur = head_;

    while (cur->next)
      cur = cur->next;

    cur->next = new Node(value);
  }

  void pop_back()
  {
    if (!head_)
      return;

    if (!head_->next)
    {
      delete head_;
      head_ = nullptr;
      return;
    }

    Node* cur = head_;

    while (cur->next->next)
      cur = cur->next;

    delete cur->next;
    cur->next = nullptr;
  }

  void clear()
  {
    while (head_)
    {
      Node* tmp = head_;
      head_ = head_->next;
      delete tmp;
    }
  }
};

}

#endif
