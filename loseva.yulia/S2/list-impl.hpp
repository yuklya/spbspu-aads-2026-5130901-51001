#ifndef LIST_IMPL_HPP
#define LIST_IMPL_HPP

namespace loseva {

template <typename T>
List<T>::List() : head_(0), tail_(0), size_(0) {}

template <typename T>
List<T>::~List() {
    while (head_) {
        Node* tmp = head_;
        head_ = head_->next;
        delete tmp;
    }
}

template <typename T>
void List<T>::push_back(const T& val) {
    Node* n = new Node(val);

    if (!tail_) {
        head_ = tail_ = n;
    } else {
        tail_->next = n;
        n->prev = tail_;
        tail_ = n;
    }
    ++size_;
}

template <typename T>
bool List<T>::empty() const {
    return size_ == 0;
}

template <typename T>
size_t List<T>::size() const {
    return size_;
}

}

#endif
