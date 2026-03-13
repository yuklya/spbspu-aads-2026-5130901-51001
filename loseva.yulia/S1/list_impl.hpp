#ifndef LIST_IMPL_HPP
#define LIST_IMPL_HPP

#include "list.hpp"
#include <stdexcept>

namespace loseva {
  namespace detail {
    template<typename T>
    typename List<T>::Node* create_node(const T& value, typename List<T>::Node* next = nullptr, typename List<T>::Node* prev = nullptr) {
      return new typename List<T>::Node(value, next, prev);
    }
    template<typename T>
    typename List<T>::Node* create_node(T&& value, typename List<T>::Nod> typename List<T>::Nod>
      return new typename List<T>::Node(std::move(value), next, prev);
    }
  }
  template<typename T>
  List<T>::List() : head(nullptr), tail(nullptr), size(0) {}

  template<typename T>
  List<T>::List(size_t count, const T& value) : head(nullptr), tail(nullptr), size(0) {
    for (size_t i = 0; i < count; ++i) {
      push_back(value);
    }
  }
  template<typename T>
  List<T>::List(std::initializer_list<T> init) : head(nullptr), tail(nullptr), size(0) {
    for (const auto& item : init) {
      push_back(item);
    }
  }
  template<typename T>
  List<T>::~List() {
    clear();
  }
  template<typename T>
  void List<T>::clear() {
    while (head) {
      Node* temp = head;
      head = head->next;
      delete temp;
    }
    tail = nullptr;
    size = 0;
  }
  template<typename T>
  T& List<T>::front() {
    if (!head) {
      throw std::out_of_range("List is empty");
    };
    return head->data;
  }
  template<typename T>
  const T& List<T>::front() const {
    if (!head) {
      throw std::out_of_range("List is empty");
    };
    return head->data;
  }

  template<typename T>
  T& List<T>::back() {
    if (!tail) {
      throw std::out_of_range("List is empty");
    };
    return tail->data;
  }
  template<typename T>
  const T& List<T>::back() const {
    if (!tail) {
    throw std::out_of_range("List is empty");
    };
    return tail->data;
  }

  template<typename T>
  bool List<T>::empty() const noexcept {
    return size == 0;
  }

  template<typename T>
  size_t List<T>::get_size() const noexcept {
    return size;
  }
  template<typename T>
  void List<T>::push_front(const T& value) {
    Node* new_node = detail::create_node<T>(value, head, nullptr);
    if (head) {
      head->prev = new_node;
    } else {
      tail = new_node;
    }
    head = new_node;
    ++size;
  }
  template<typename T>
  void List<T>::push_front(T&& value) {
    Node* new_node = detail::create_node<T>(std::move(value), head, nullptr);
    if (head) {
      head->prev = new_node;
    } else {
      tail = new_node;
    }
    head = new_node;
    ++size;
  }
  template<typename T>
  void List<T>::push_back(const T& value) {
    Node* new_node = detail::create_node<T>(value, nullptr, tail);
    if (tail) {
      tail->next = new_node;
    } else {
      head = new_node;
    }
    tail = new_node;
    ++size;
  }

  template<typename T>
  void List<T>::push_back(T&& value) {
    Node* new_node = detail::create_node<T>(std::move(value), nullptr, tail);
    if (tail) {
      tail->next = new_node;
    } else {
      head = new_node;
    }
    tail = new_node;
    ++size;
  }
  template<typename T>
  void List<T>::pop_front() {
    if (!head) {
      throw std::out_of_range("List is empty");
    };
    Node* temp = head;
    head = head->next;
    if (head) {
      head->prev = nullptr;
    } else {
      tail = nullptr;
    }
    delete temp;
    --size;
  }

  template<typename T>
  void List<T>::pop_back() {
    if (!tail) {
      throw std::out_of_range("List is empty");
    };
    Node* temp = tail;
    tail = tail->prev;
    if (tail) {
      tail->next = nullptr;
    } else {
      head = nullptr;
    }
    delete temp;
    --size;
  }
 template<typename T>
  List<T>::Iterator::Iterator(Node* node) : current(node) {}
  template<typename T>
  List<T>::Iterator::Iterator() : current(nullptr) {}
  template<typename T>
  typename List<T>::Iterator& List<T>::Iterator::operator++() {
    if (current) {
      current = current->next;
    }
    return *this;
 }
  template<typename T>
  typename List<T>::Iterator List<T>::Iterator::operator++(int) {
    Iterator temp = *this;
    ++(*this);
    return temp;
  }

  template<typename T>
  typename List<T>::Iterator& List<T>::Iterator::operator--() {
    if (current) {
      current = current->prev;
    }
    return *this;
  }
  template<typename T>
  typename List<T>::Iterator List<T>::Iterator::operator--(int) {
    Iterator temp = *this;
    --(*this);
    return temp;
  }
  template<typename T>
  T& List<T>::Iterator::operator*() const {
    return current->data;
  }
  template<typename T>
  T* List<T>::Iterator::operator->() const {
    return &(current->data);
  }

  template<typename T>
  bool List<T>::Iterator::operator==(const Iterator& other) const {
    return current == other.current;
  }

  template<typename T>
  bool List<T>::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
  }

  template<typename T>
  List<T>::ConstIterator::ConstIterator(const Node* node) : current(node) {}

  template<typename T>
  List<T>::ConstIterator::ConstIterator() : current(nullptr) {}

  template<typename T>
  List<T>::ConstIterator::ConstIterator(const Iterator& it) : current(it.current) {}

  template<typename T>
  typename List<T>::ConstIterator& List<T>::ConstIterator::operator++() {
    if (current) {
      current = current->next;
    }
    return *this;
  }

  template<typename T>
  typename List<T>::ConstIterator List<T>::ConstIterator::operator++(int) {
    ConstIterator temp = *this;
    ++(*this);
    return temp;
  }
  template<typename T>
  typename List<T>::ConstIterator& List<T>::ConstIterator::operator--() {
    if (current) {
      current = current->prev;
    }
    return *this;
  }

  template<typename T>
  typename List<T>::ConstIterator List<T>::ConstIterator::operator--(int) {
    ConstIterator temp = *this;
    --(*this);
    return temp;
  }

  template<typename T>
  const T& List<T>::ConstIterator::operator*() const {
    return current->data;
  }

  template<typename T>
  const T* List<T>::ConstIterator::operator->() const {
    return &(current->data);
  }

  template<typename T>
  bool List<T>::ConstIterator::operator==(const ConstIterator& other) const {
    return current == other.current;
  }

  template<typename T>
  bool List<T>::ConstIterator::operator!=(const ConstIterator& other) const {
    return !(*this == other);
  }

  template<typename T>
  typename List<T>::iterator List<T>::begin() noexcept {
    return iterator(head);
  }

  template<typename T>
  typename List<T>::const_iterator List<T>::begin() const noexcept {
    return const_iterator(head);
  }
  template<typename T>
  typename List<T>::const_iterator List<T>::cbegin() const noexcept {
    return const_iterator(head);
  }
  template<typename T>
  typename List<T>::iterator List<T>::end() noexcept {
    return iterator(nullptr);
  }
  template<typename T>
  typename List<T>::const_iterator List<T>::end() const noexcept {
    return const_iterator(nullptr);
  }
  template<typename T>
  typename List<T>::const_iterator List<T>::cend() const noexcept {
    return const_iterator(nullptr);
  }

  template<typename T>
  typename List<T>::reverse_iterator List<T>::rbegin() noexcept {
    return reverse_iterator(end());
  }

  template<typename T>
  typename List<T>::const_reverse_iterator List<T>::rbegin() const noexcept {
    return const_reverse_iterator(end());
  }

  template<typename T>
  typename List<T>::const_reverse_iterator List<T>::crbegin() const noexcept {
    return const_reverse_iterator(cend());
  }
  template<typename T>
  typename List<T>::reverse_iterator List<T>::rend() noexcept {
    return reverse_iterator(begin());
  }

  template<typename T>
  typename List<T>::const_reverse_iterator List<T>::rend() const noexcept {
    return const_reverse_iterator(begin());
  }

  template<typename T>
  typename List<T>::const_reverse_iterator List<T>::crend() const noexcept {
    return const_reverse_iterator(cbegin());
  }
  template<typename T>
  List<T>::List(const List& other) : head(nullptr), tail(nullptr), size(0) {
    copy_from(other);
  }

  template<typename T>
  List<T>::List(List&& other) noexcept : head(other.head), tail(other.tail), size(other.size) {
  other.head = nullptr;
  other.tail = nullptr;
  other.size = 0;
  }

  template<typename T>
  void List<T>::copy_from(const List& other) {
    for (const auto& item : other) {
      push_back(item);
    }
  }

  template<typename T>
  List<T>& List<T>::operator=(const List& other) {
    if (this != &other) {
      clear();
      copy_from(other);
    }
  return *this;
  }
  template<typename T>
  List<T>& List<T>::operator=(List&& other) noexcept {
    if (this != &other) {
      clear();
      head = other.head;
      tail = other.tail;
      size = other.size;
      other.head = nullptr;
      other.tail = nullptr;
      other.size = 0;
    }
    return *this;
  }

  template<typename T>
  List<T>& List<T>::operator=(std::initializer_list<T> init) {
    clear();
    for (const auto& item : init) {
      push_back(item);
    }
    return *this;
  }
}

#endif

