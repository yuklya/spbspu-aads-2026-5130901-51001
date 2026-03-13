#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

namespace loseva {
template <typename T>
class List {
private:
  struct Node {
    T data;
    Node* next;
    Node* prev;

    Node(const T& value, Node* n = nullptr, Node* p = nullptr) : data(value), next(n), prev(p) {}
    Node(T&& value, Node* n = nullptr, Node* p = nullptr) : data(std::move(value)), next(n), prev(p) {}
  };

  Node* head;
  Node* tail;
  size_t size;
