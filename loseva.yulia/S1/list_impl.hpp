#ifndef LIST_IMPL_HPP
#define LIST_IMPL_HPP

namespace loseva
{

template<class T>
class List;

template<class T>
class LIter
{
  friend class List<T>;

private:
  struct Node* node_;

public:
  LIter(): node_(nullptr) {}

  T& operator*() const { return node_->data; }

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
    return !(*this == other);
  }
};

template<class T>
class LCIter
{
  friend class List<T>;

private:
  const struct Node* node_;

public:
  LCIter(): node_(nullptr) {}

  const T& operator*() const { return node_->data; }

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
    return !(*this == other);
  }
};

}

#endif
