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
}

#endif
