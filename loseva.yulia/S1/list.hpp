#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace loseva {

  template <typename T>
  class List {
  private:
    struct Node {
      T data;
      Node* next;
      Node* prev;
      Node(const T& value, Node* n = nullptr, Node* p = nullptr)
        : data(value), next(n), prev(p) {}
      Node(T&& value, Node* n = nullptr, Node* p = nullptr)
        : data(std::move(value)), next(n), prev(p) {}
  };
  Node* head;
  Node* tail;
  size_t size;
  void clear() noexcept;
  void copy_from(const List& other);
  public:
    class Iterator {
      friend class List<T>;
      Node* current;
      explicit Iterator(Node* node);
    public:
      using iterator_category = std::bidirectional_iterator_tag;
      using value_type = T;
      using difference_type = std::ptrdiff_t;
      using pointer = T*;
      using reference = T&;
      Iterator();
      Iterator& operator++();
      Iterator& operator--();
      Iterator operator++(int);
      Iterator operator--(int);
      reference operator*() const;
      pointer operator->() const;
      bool operator==(const Iterator& other) const;
      bool operator!=(const Iterator& other) const;
    };
  class ConstIterator {
    friend class List<T>;
    const Node* current;
    explicit ConstIterator(const Node* node);
    public:
      using iterator_category = std::bidirectional_iterator_tag;
      using value_type = const T;
      using difference_type = std::ptrdiff_t;
      using pointer = const T*;
      using reference = const T&;
      ConstIterator();
      ConstIterator(const Iterator& it);
      ConstIterator& operator++();
      ConstIterator& operator--();
      ConstIterator operator++(int);
      ConstIterator operator--(int);
      reference operator*() const;
      pointer operator->() const;
      bool operator==(const ConstIterator& other) const;
      bool operator!=(const ConstIterator& other) const;
    };
    using iterator = Iterator;
    using const_iterator = ConstIterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    List();
    explicit List(size_t count, const T& value = T());
    List(std::initializer_list<T> init);
    template<typename InputIt>
    List(InputIt first, InputIt last);
    List(const List& other);
    List(List&& other) noexcept;
    ~List();
    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;
    List& operator=(std::initializer_list<T> init);
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;
    bool empty() const noexcept;
    size_t get_size() const noexcept;
    void clear();
    iterator insert(const_iterator pos, const T& value);
    iterator insert(const_iterator pos, T&& value);
    iterator insert(const_iterator pos, size_t count, const T& value);
    template<typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last);
    iterator insert(const_iterator pos, std::initializer_list<T> init);
    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    void push_front(const T& value);
    void push_front(T&& value);
    void push_back(const T& value);
    void push_back(T&& value);
    template<typename... Args>
    void emplace_front(Args&&... args);
    template<typename... Args>
    void emplace_back(Args&&... args);
    void pop_front();
    void pop_back();
    void resize(size_t count);
    void resize(size_t count, const T& value);
    void swap(List& other) noexcept;
    void splice(const_iterator pos, List& other);
    void splice(const_iterator pos, List&& other);
    void splice(const_iterator pos, List& other, const_iterator it);
    void splice(const_iterator pos, List&& other, const_iterator it);
    void splice(const_iterator pos, List& other, const_iterator first, const_iterator last);
    void splice(const_iterator pos, List&& other, const_iterator first, const_iterator last);
    void remove(const T& value);
    template<typename Predicate>
    void remove_if(Predicate pred);
    void reverse() noexcept;
    void unique();
    template<typename BinaryPredicate>
    void unique(BinaryPredicate pred);
    void sort();
    template<typename Compare>
    void sort(Compare comp);
    bool operator==(const List& other) const;
    bool operator!=(const List& other) const;
    bool operator<(const List& other) const;
    bool operator<=(const List& other) const;
    bool operator>(const List& other) const;
    bool operator>=(const List& other) const;
  private:
    void merge_sort(Node*& start);
    Node* merge(Node* left, Node* right);
    template<typename Compare>
    Node* merge(Node* left, Node* right, Compare comp);
    void split(Node* source, Node*& front, Node*& back);
  };

  template<typename T>
  void swap(List<T>& lhs, List<T>& rhs) noexcept;

}

#include "list_impl.hpp"

#endif

