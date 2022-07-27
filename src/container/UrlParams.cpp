#include "UrlParams.h"

UrlParams::UrlParams() {
    head_ptr = new Node();
    tail_ptr = head_ptr;
}

UrlParams::UrlParams(UrlParams&& another) {
    head_ptr = another.head_ptr;
    tail_ptr = another.tail_ptr;
    another.head_ptr = nullptr;
    another.tail_ptr = nullptr;
}

UrlParams::~UrlParams() {
    _release_after(head_ptr);
    tail_ptr = nullptr;
}

UrlParams::Iterator UrlParams::begin() const {
    return UrlParams::Iterator(head_ptr->next);
}

UrlParams::Iterator UrlParams::end() const {
    return UrlParams::Iterator(tail_ptr->next);
}

void UrlParams::push_back(const std::string& val) {
    tail_ptr->next = new Node(val);
    tail_ptr = tail_ptr->next;
}

inline void UrlParams::_release_after(Node* ptr) {
    Node *p, *q = ptr;
    while (q != nullptr)
    {
        p = q;
        q = q->next;
        delete p;
    } 
}

void UrlParams::clear() {
    _release_after(head_ptr->next);
    head_ptr->next = nullptr;
    tail_ptr = head_ptr;
}

bool UrlParams::empty() const {
    return head_ptr == tail_ptr;
}

bool UrlParams::operator==(const UrlParams& another) const {
    return head_ptr == another.head_ptr;
}