//
// Created by OctAutumn on 2022/7/5.
//

#ifndef LHS_THREADPOOL_H
#define LHS_THREADPOOL_H

#include <functional>
#include <thread>
#include <condition_variable>
#include <future>
#include <unistd.h>

#include "container/SafeQueue.h"

class ThreadPool
{
private:
    class ThreadWorker // 内置线程工作类
    {
    private:
        int worker_id; // 工作id

        ThreadPool *parent_pool; // 所属线程池
    public:
        // 构造函数
        ThreadWorker(ThreadPool *pool, const int id) : parent_pool(pool), worker_id(id)
        {}

        // 析构函数（释放资源）
        ~ThreadWorker() = default;

        // 重载()操作
        void operator()();
    };

    bool pool_shutdown; // 线程池是否关闭

    SafeQueue<std::function<void()>> *task_queue; // 执行函数安全队列，即任务队列

    std::vector<std::thread> *worker_threads; // 工作线程队列

    std::mutex m_conditional_mutex; // 线程休眠锁互斥变量

    std::condition_variable m_conditional_lock; // 线程环境锁，可以让线程处于休眠或者唤醒状态

public:
    /**
     *
     * @param n_threads 线程池size（默认为可用cpu核心数）
     */
    explicit ThreadPool(int n_threads = 0);

    ThreadPool(const ThreadPool &) = delete;

    ThreadPool(ThreadPool &&) = delete;

    ~ThreadPool();

    ThreadPool &operator=(const ThreadPool &) = delete;

    ThreadPool &operator=(ThreadPool &&) = delete;

    int size();

    // 初始化线程池
    void init();

    // 关闭线程池（等待各线程完成工作后Join）
    void shutdown();

    // 任务提交函数
    template<typename F, typename... Args>
    auto submit(F &&f, Args &&...args) -> std::future<decltype(f(args...))>
    {
        // 创建一个函数，并绑定参数
        std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f),
                                                               std::forward<Args>(args)...); // 连接函数和参数定义，特殊函数类型，避免左右值错误

        // 将其封装为共享指针以便能够复制构造
        auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

        // 将封装好的函数再包装入一个void函数
        std::function<void()> warpper_func = [task_ptr]() {
            (*task_ptr)();
        };

        // 队列通用安全封包函数，并压入安全队列
        task_queue->enqueue(warpper_func);

        // 唤醒一个等待中的线程
        m_conditional_lock.notify_one();

        // 返回先前注册的任务指针
        return task_ptr->get_future();
    }
};


#endif //LHS_THREADPOOL_H
