//
// Created by devRickLin on 2022/7/25.
//
#ifndef URL_PARAMS_H_
#define URL_PARAMS_H_
#include <string>
#include <iostream>
#include <iterator>
#include <cstddef>
#include <list>
#include <exception>
/**
 * @brief Container for Url parameters.
 * @note only movable, not allow to copy.
 */
class UrlParams
{
private:
    /**
     * @brief Node of inner link list.
     * 
     */
    struct Node {
        std::string param;
        Node* next = nullptr;
        Node() = default;
        Node(const std::string& param): param(param) {};
        Node(Node&& another) {
            param = another.param;
            next = another.next;
            another.next = nullptr;
        };
        ~Node() {
            next = nullptr;
        }
    };

public:
    /**
     * @brief Iterator to iterate link list
     * 
     */
    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using value_type        = Node;
        using pointer           = const std::string*; 
        using reference         = const std::string&;

        Iterator(Node* ptr): m_ptr(ptr) {}
        ~Iterator() { m_ptr = nullptr; }

        reference operator*() const { return m_ptr->param; }
        pointer operator->() const { return &m_ptr->param; }

        Iterator& operator++() { 
            if ( m_ptr == nullptr ) {
                throw std::out_of_range("attempting to iterator nullptr (may have reached end)");
                return *this;
            }
            m_ptr = m_ptr->next;
            return *this;
        }

        Iterator& operator++(int) {
            return operator++();
        }

        bool operator==(const Iterator& another ) const {
            return m_ptr == another.m_ptr; 
        }

        bool operator!=(const Iterator& another ) const {
            return ! operator==(another);
        }

    private:
        Node* m_ptr; 
    };
    
    UrlParams();
    UrlParams(const UrlParams&) = delete;
    UrlParams(UrlParams&&);
    ~UrlParams();
    void push_back(const std::string& val);
    void clear();
    /**
     * @brief after freeze, UrlParams can't be modified.
     * 
     * @details any non-const method will raise exception if UrlParams have benn freezed.
     */
    void freeze();
    inline bool is_freezed() const;
    Iterator begin() const;
    Iterator end() const;
    inline bool empty() const;
    inline size_t size() const;
    inline bool operator==(const UrlParams& another) const;
private:
    void _release_after(Node* ptr);
    void _freezed_guard() const;
    size_t size_;
    bool freezed_;
    Node* head_ptr_;
    Node* tail_ptr_;
};



#endif // URL_PARAMS_H_
