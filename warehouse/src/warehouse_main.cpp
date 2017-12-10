#include "warehouse_common.h"
#include <string>
#include <fstream>
#include <thread>
#include <random>
#include <cpen333/process/shared_memory.h>
#include "Robot.h"
#include "safe_printf.h"
#include "Shelf.h"


using namespace std;

static const char GET_LOW_STOCK = '1';
static const char GET_ORDER_STATUS = '2';
static const char CLIENT_QUIT = '3';

// print menu options
void print_menu() {

  safe_printf( "MENU\n");
  safe_printf( "====\n");
  safe_printf( " (1) get low stock items\n");
  safe_printf( " (2) get order status\n");
  safe_printf( " (3) get stock of an item\n");
  safe_printf( " (3) quit\n");
  safe_printf( " (4) add order of 5 apples\n");
  safe_printf( " (5) add order of 2 oranges\n");
  safe_printf( "Enter number: \n");
  std::cout.flush();

}

void load_warehouse(const std::string& filename, WarehouseInfo& minfo) {

  // initialize number of rows and columns
  minfo.rows = 0;
  minfo.cols = 0;

  std::ifstream fin(filename);
  std::string line;

  // read maze file
  if (fin.is_open()) {
    int row = 0;  // zeroeth row
    while (std::getline(fin, line)) {
      int cols = line.length();
      if (cols > 0) {
        // longest row defines columns
        if (cols > minfo.cols) {
          minfo.cols = cols;
        }
        for (size_t col=0; col<cols; ++col) {
          minfo.maze[col][row] = line[col];
        }
        ++row;
      }
    }
    minfo.rows = row;
    fin.close();
  }

}

void init_robots(const WarehouseInfo& minfo, RunnerInfo& rinfo) {
  rinfo.nrunners = 0;

  // fill in random placements for future runners
  std::default_random_engine rnd(
      (unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<size_t> rdist(0, minfo.rows);
  std::uniform_int_distribution<size_t> cdist(0, minfo.cols);
  for (size_t i=0; i<MAX_RUNNERS; ++i) {
    // generate until on an empty space
    size_t r,c;
    do {
      r = rdist(rnd);
      c = cdist(rnd);
    } while (minfo.maze[c][r] != EMPTY_CHAR);
    rinfo.rloc[i][COL_IDX] = c;
    rinfo.rloc[i][ROW_IDX] = r;
  }
}

//find shelves and mark location
void init_shelves(vector<Shelf>& shelves, const WarehouseInfo& minfo, map<int, string>& product_map) {
    for(int i = 0; i < minfo.cols; i++){
      for(int j = 0; j < minfo.rows; j++){
        if(minfo.maze[i][j] == SHELF_CHAR)
        {
          cout << "Shelf " << shelves.size() + 1;
          Shelf shelf_init(i,j,product_map);
          shelves.push_back(shelf_init);
        }
      }
    }
}

int main(int argc, char* argv[]) {

    std::string warehouselayout = "data/warehouse.txt";
    if (argc > 1) {
        warehouselayout = argv[1];
    }
    cpen333::process::shared_object<SharedData> memory(WAREHOUSE_MEMORY_NAME); // initializes shared memory, creates memory of type SharedData which contains
    memory->quit = false;
    //initialize warehouse layout
    load_warehouse(warehouselayout,memory->minfo);
    //initialize robot positions
    init_robots(memory->minfo,memory->rinfo);
    //initialize shelves

    const int nrobots = 4;
    const int nshelves = 15;

    //order queue
    //WarehouseOrderQueue = order_queue;
    //dock queue
    //WarehouseDockQueue = dock_queue;
    //task_queue
    //TaskQueue = task_queue;
    map<int, string> product_map{{0,"apple"},{1,"orange"},{2,"banana"},{3,"carrot"},{4,"potato"}};
    vector <Shelf> shelves;
    init_shelves(shelves, memory->minfo, product_map);
    cout << "Shelves initialized, products distributed.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    //initialize robots
    vector<Robot*> robots;
    for( int i = 0; i < 4; i++){
        robots.push_back( new Robot(i/*, order_queue, dock_queue*/));
    }
    cout<< "Locations of robots initialized.\n";

    for (auto& r : robots) {
        r->start();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    //main menu activity
    char cmd = 0;
    while (cmd != CLIENT_QUIT) {
      print_menu();

      // get menu entry
      std::cin >> cmd;
      // gobble newline
      std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');

      switch(cmd) {
        case GET_LOW_STOCK:
          //do_add(api);
          break;
        case GET_ORDER_STATUS:
          //do_remove(api);
          break;
        case CLIENT_QUIT:
          //do_goodbye(api);
          break;
        default:
          std::cout << "Invalid command number " << cmd << std::endl << std::endl;
      }
    }
    std::cout << "goodbye" << std::endl << std::endl;
    std::cin.get();
    memory->quit = true;
    memory->magic = 0;
    return 0;
}