/* 
 * File:   Safe_queue.h
 * Author: jesse
 *
 * Created on den 2 januari 2016, 12:32
 */

#ifndef SAFE_QUEUE_H
#define	SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <class T>
class SafeQueue
{
public:
  SafeQueue(void)
    : q()
    , m()
    , c()
  {}

  ~SafeQueue(void)
  {}

  void enqueue(T t)
  {
    // the thread owns the mutex (for the duration of the scope)
    std::lock_guard<std::mutex> lock(m);
    q.push(t);
    // notify the condition variable that something is added to the queue
    c.notify_one();
  }

  T dequeue(void)
  {
    // unique_lock enables us to wait for the queue to be filled/notified
    std::unique_lock<std::mutex> lock(m);
    // while loop is needed incase the condition variable c wakes up before notify
    while(q.empty())
    {
      // wait until the condition variables is notified
      c.wait(lock);
    }
    T val = q.front();
    q.pop();
    return val;
  }

private:
  std::queue<T> q;
  mutable std::mutex m;
  std::condition_variable c;
};

#endif	/* SAFE_QUEUE_H */

