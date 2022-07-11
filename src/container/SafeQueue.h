//
// Created by OctAutumn on 2022/7/4.
//

#ifndef LHS_SAFEQUEUE_H
#define LHS_SAFEQUEUE_H


#include <queue>
#include <mutex>

template <typename T>
class SafeQueue
{
private:
    std::queue<T> m_queue; // 任务队列
    
    std::mutex m_mutex; // 访问互斥信号量
    
public:
    SafeQueue() = default;
    SafeQueue(SafeQueue &&other)  noexcept {}
    ~SafeQueue() = default;

    bool empty() // 返回队列是否为空
    {
        std::unique_lock<std::mutex> lock(m_mutex); // 互斥信号变量加锁，防止m_queue被改变

        return m_queue.empty();
    }

    int size()
    {
        std::unique_lock<std::mutex> lock(m_mutex); // 互斥信号变量加锁，防止m_queue被改变

        return m_queue.size();
    }
    
    // 队列添加元素
    void enqueue(T &t)    // 队列添加元素
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.emplace(t);
    };

    bool dequeue(T &t)    // 队列取出元素
    {
        std::unique_lock<std::mutex> lock(m_mutex); // 队列加锁

        if (m_queue.empty())
            return false;
        t = std::move(m_queue.front()); // 取出队首元素，返回队首元素值，并进行右值引用

        m_queue.pop(); // 弹出入队的第一个元素

        return true;
    }
};


#endif //LHS_SAFEQUEUE_H
