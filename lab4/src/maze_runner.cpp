#include "maze_runner_common.h"

#include <cpen333/process/shared_memory.h>
#include <cpen333/process/mutex.h>
#include <cstring>
#include <chrono>
#include <thread>
#include <vector>
#define VISITED '.'
using namespace std;
class MazeRunner {

  cpen333::process::shared_object<SharedData> memory_;
  cpen333::process::mutex mutex_;

  // local copy of maze
  MazeInfo minfo_;

  // runner info
  size_t idx_;   // runner index
  int loc_[2];   // current location 

  //visited
  //vector<vector<bool>> visited;
 public:

  MazeRunner() : memory_(MAZE_MEMORY_NAME), mutex_(MAZE_MUTEX_NAME),
                 minfo_(), idx_(0), loc_() {
    // {
    //   std::lock_guard<decltype(mutex_)> lock(mutex_);
    //   if(memory_->magic != MAGIC){
    //     std::cout << "uninitialzied memory";
    //     return;
    //   }
    // }
    // copy maze contents
    minfo_ = memory_->minfo;
    //vector<vector<bool>> visited(minfo_.rows, vector<bool>(minfo_.cols,false));
    {
      // protect access of number of runners
      std::lock_guard<decltype(mutex_)> lock(mutex_);
      idx_ = memory_->rinfo.nrunners;
      memory_->rinfo.nrunners++;
    }

    // get current location
    loc_[COL_IDX] = memory_->rinfo.rloc[idx_][COL_IDX];
    loc_[ROW_IDX] = memory_->rinfo.rloc[idx_][ROW_IDX];

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
    memory_->rinfo.rloc[idx_][COL_IDX] = c;
    memory_->rinfo.rloc[idx_][ROW_IDX] = r;

    //run into exit / wall / visited
    if(minfo_.maze[c][r] == EXIT_CHAR)
    {
      returnval = 1;
      return;
    }
    else if(minfo_.maze[c][r] == WALL_CHAR || minfo_.maze[c][r] == VISITED)
    {
      return;
    }

    //mark visited
    minfo_.maze[c][r] = VISITED;
    //recursive call
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    recurse(c+1, r, returnval);
    recurse(c, r+1, returnval);
    recurse(c, r-1, returnval);
    recurse(c-1, r, returnval);

    //handle back tracking
    minfo_.maze[c][r] = EMPTY_CHAR;
    if(returnval) return;
    //update ui
    memory_->rinfo.rloc[idx_][COL_IDX] = c;
    memory_->rinfo.rloc[idx_][ROW_IDX] = r;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
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

};

int main() {

  MazeRunner runner;
  cout << "status: " << runner.go();

  return 0;
}