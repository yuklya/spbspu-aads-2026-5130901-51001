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

        Node(const T& val) : data(val), next(0), prev(0) {}
    };

    Node* head_;
    Node* tail_;
    size_t size_;
};

}

#endif
