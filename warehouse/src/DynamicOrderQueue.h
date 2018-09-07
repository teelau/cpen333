#ifndef WAREHOUSE_DYNAMICORDERQUEUE_H
#define WAREHOUSE_DYNAMICORDERQUEUE_H

#include "Order.h"
#include <deque>
#include <condition_variable>
#include <mutex>
using namespace std;

class DynamicOrderQueue{
   public:
  std::deque<Order> buff_;
  std::mutex mutex_;
  std::condition_variable cv_;


  DynamicOrderQueue() :
      buff_(), mutex_(), cv_(){}

  void add(const Order& order) {
    {
    unique_lock<decltype(mutex_)> lock{mutex_};
    buff_.push_back(order);
    cv_.notify_one();
    }

  }

  Order get() {
    {
    unique_lock<decltype(mutex_)> lock{mutex_};
    cv_.wait(lock, [&](){return !buff_.empty(); });
    Order out = buff_.front();
    buff_.pop_front();
    return out;

    }


  }
};

#endif
