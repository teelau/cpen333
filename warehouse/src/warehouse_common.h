#ifndef WAREHOUSE_COMMON_H
#define WAREHOUSE_COMMON_H

#define WAREHOUSE_MEMORY_NAME "warehouse_mem"
#define WAREHOUSE_MUTEX_NAME "warehouse_robot_mutex"

#define WALL_CHAR 'X'
#define EMPTY_CHAR ' '
#define EXIT_CHAR 'E'
#define SHELF_CHAR 'O'

#define COL_IDX 0
#define ROW_IDX 1

#define MAXWEIGHT_SHELF 50
#define MAX_MAZE_SIZE 80
#define MAX_RUNNERS   5
#define MAX_SHELVES 15
#define MAGIC 99999

// struct Product {

// }
struct WarehouseInfo {
  int rows;           // rows in maze
  int cols;           // columns in maze
  char maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE];  // maze storage
};

struct RunnerInfo {
  int nrunners;      // number runners
  int rloc[MAX_RUNNERS][2];   // runner locations [col][row]
};

struct SharedData {
  WarehouseInfo minfo;    // maze info
  RunnerInfo rinfo;  // runner info
  bool quit;         // tell everyone to quit
  bool magic;
};


#endif //LAB4_MAZE_RUNNER_COMMON_H
