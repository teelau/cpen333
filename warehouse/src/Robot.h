#ifndef ROBOT_H
#define ROBOT_H

#include <cpen333/thread/thread_object.h>
#include <cpen333/process/shared_memory.h>
#include <cpen333/process/mutex.h>
#include <iostream>
#include <thread>
#define VISITED '.'
#include "Order.h"
// #include "Order.h"
// #include "OrderQueue.h"
#include "safe_printf.h"
#include "DynamicOrderQueue.h"
#include "Order.h"

/**
 * The Chef grabs orders from a queue, cooks them,
 * then adds the cooked dishes to a new queue for
 * the servers to serve
 */
class Robot : public cpen333::thread::thread_object {
  // OrderQueue& orders_;
  // OrderQueue& serve_;
  cpen333::process::shared_object<SharedData> memory_;
  cpen333::process::mutex mutex_;
  vector<Order> order;
  // local copy of maze
  WarehouseInfo minfo_;
  int id_; //robot index
  int loc_[2]; //current location
 public:
  /**
   * Create a new chef
   * @param id the chef's id
   * @param orders queue to read orders from
   * @param serve queue to add completed orders to
   */
  Robot(int id/* DynamicOrderQueue order_queue*//*, OrderQueue& orders, OrderQueue& serve*/) :

      id_(id), memory_(WAREHOUSE_MEMORY_NAME), mutex_(WAREHOUSE_MUTEX_NAME)/*, orders_(orders), serve_(serve)*/ 
      {
        minfo_ = memory_->minfo;
        {
        // protect access of number of runners
        std::lock_guard<decltype(mutex_)> lock(mutex_);
        memory_->rinfo.nrunners++;
        }

        loc_[COL_IDX] = memory_->rinfo.rloc[id_][COL_IDX];
        loc_[ROW_IDX] = memory_->rinfo.rloc[id_][ROW_IDX];
      }

  int id() {
    return id_;
  }

  void recurse(int c, int r, int& returnval)
  {
    if(memory_->quit)
    {
      returnval = -1;
      return;
    }

    if(returnval)return;

    //update ui
    memory_->rinfo.rloc[id_][COL_IDX] = c;
    memory_->rinfo.rloc[id_][ROW_IDX] = r;

    //run into exit / wall / visited
    if(minfo_.maze[c][r] == WALL_CHAR || minfo_.maze[c][r] == VISITED || minfo_.maze[c][r] == SHELF_CHAR)
    {
      return;
    }
    else if(minfo_.maze[c][r] == EXIT_CHAR)
    {
      
      return;
    }

    //mark visited
    minfo_.maze[c][r] = VISITED;
    //recursive call
    std::this_thread::sleep_for(std::chrono::milliseconds(350));
    recurse(c+1, r, returnval);
    recurse(c, r+1, returnval);
    recurse(c, r-1, returnval);
    recurse(c-1, r, returnval);

    //handle back tracking
    minfo_.maze[c][r] = EMPTY_CHAR;
    if(returnval) return;
    //update ui
    memory_->rinfo.rloc[id_][COL_IDX] = c;
    memory_->rinfo.rloc[id_][ROW_IDX] = r;
    std::this_thread::sleep_for(std::chrono::milliseconds(350));
    if(memory_->quit)
    {
      returnval = -1;
      return;
    }
    return;
  }
  /**
   * Solves the maze, taking time between each step so we can actually see progress in the UI
   * @return 1 for success, 0 for failure, -1 to quit
   */
  int go() {
    if (memory_->quit) return -1;
    // current location
    int c = loc_[COL_IDX];
    int r = loc_[ROW_IDX];

    int returnval = 0;
    recurse(c,r,returnval);
    // exit status
    return returnval;
  }
  /**
   * Main execution function
   * @return 0 if completed
   */
  int main() {

    safe_printf("Robot %d started at ( %d, %d)\n", id_,loc_[COL_IDX],loc_[ROW_IDX]);

    //safe_printf("status: %d ",go());
    // Order order = orders_.get();

    while (true) {
      // if(order.poison == 1)
      //   break;
      // // process order
      // safe_printf("Chef %d starting order {%d,%d}\n", id_, order.customer_id, order.item_id);
      // std::this_thread::sleep_for(std::chrono::seconds(5));
      // safe_printf("Chef %d completed order {%d,%d}\n", id_, order.customer_id, order.item_id);

      // // add to those to serve
      // serve_.add(order);

      // // next order
      // order = orders_.get();
    }
    safe_printf("Robot %d done\n", id_);

    return 0;
  }
};

#endif //LAB6_CHEF_H
