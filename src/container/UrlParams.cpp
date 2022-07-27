#include "UrlParams.h"

UrlParams::UrlParams() {
    size_ = 0;
    freezed_ = false;
    head_ptr_ = new Node();
    tail_ptr_ = head_ptr_;
}

UrlParams::UrlParams(UrlParams&& another) {
    size_ = another.size_;
    freezed_ = another.freezed_;
    head_ptr_ = another.head_ptr_;
    tail_ptr_ = another.tail_ptr_;
    another.head_ptr_ = nullptr;
    another.tail_ptr_ = nullptr;
}

UrlParams::~UrlParams() {
    _release_after(head_ptr_);
    tail_ptr_ = nullptr;
}

UrlParams::Iterator UrlParams::begin() const {
    return UrlParams::Iterator(head_ptr_->next);
}

UrlParams::Iterator UrlParams::end() const {
    return UrlParams::Iterator(tail_ptr_->next);
}

void UrlParams::push_back(const std::string& val) {
    _freezed_guard();
    size_++;
    tail_ptr_->next = new Node(val);
    tail_ptr_ = tail_ptr_->next;
}

void UrlParams::clear() {
    _freezed_guard();
    _release_after(head_ptr_->next);
    head_ptr_->next = nullptr;
    tail_ptr_ = head_ptr_;
}

void UrlParams::freeze() {
    freezed_ = true;
}

void UrlParams::_release_after(Node* ptr) {
    Node *p, *q = ptr;
    while (q != nullptr)
    {
        p = q;
        q = q->next;
        delete p;
    } 
}

inline bool UrlParams::empty() const {
    return size_ == 0;
}

inline bool UrlParams::operator==(const UrlParams& another) const {
    return head_ptr_ == another.head_ptr_;
}

inline size_t UrlParams::size() const {
    return size_;
}

inline bool UrlParams::is_freezed() const {
    return freezed_;
}

void UrlParams::_freezed_guard() const {
    if (freezed_) {
       throw std::logic_error("attempting to modify freezed UrlParams"); 
    }
}