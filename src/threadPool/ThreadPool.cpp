//
// Created by OctAutumn on 2022/7/5.
//

#include "ThreadPool.h"

void ThreadPool::ThreadWorker::operator()()
{
    std::function<void()> func; // 定义基础函数类func

    bool dequeued; // 是否正在取出队列中元素

    while (!parent_pool->pool_shutdown)
    {
        {
            // 为线程环境加锁，互访问工作线程的休眠和唤醒
            std::unique_lock<std::mutex> lock(parent_pool->m_conditional_mutex);

            // 如果任务队列为空，阻塞当前线程
            if (parent_pool->task_queue->empty())
            {
                parent_pool->m_conditional_lock.wait(lock); // 等待条件变量通知，开启线程
            }

            // 取出任务队列中的元素
            dequeued = parent_pool->task_queue->dequeue(func);
        }

        // 如果成功取出，执行工作函数
        if (dequeued)
            func();
    }
}

ThreadPool::ThreadPool(const int n_threads) :
        task_queue(new SafeQueue<std::function<void()>>),
        pool_shutdown(false)
{
    if (n_threads == 0)
    {
        int available_cpu_core_num = (int) sysconf(_SC_NPROCESSORS_ONLN);
        worker_threads = new std::vector<std::thread>(available_cpu_core_num);
    } else
        worker_threads = new std::vector<std::thread>(n_threads);
}

ThreadPool::~ThreadPool()
{
    delete task_queue;
    delete worker_threads;
}

int ThreadPool::size()
{
    return (int) worker_threads->size();
}

void ThreadPool::init()
{
    for (int i = 0; i < worker_threads->size(); i++)
        worker_threads->at(i) = std::thread(ThreadWorker(this, i)); // 分配工作线程
}

void ThreadPool::shutdown()
{
    pool_shutdown = true;
    m_conditional_lock.notify_all(); // 通知，唤醒所有工作线程

    for (auto &worker_thread: *worker_threads)
    {
        if (worker_thread.joinable()) // 判断线程是否在等待
        {
            worker_thread.join(); // 将线程加入到等待队列
        }
    }
}
