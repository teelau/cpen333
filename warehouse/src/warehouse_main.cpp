#include "warehouse_common.h"
#include <string>
#include <fstream>
#include <thread>
#include <random>
#include <cpen333/process/shared_memory.h>
#include "Robot.h"
#include "safe_printf.h"
#include "Shelf.h"
#include <queue>
#include "DynamicOrderQueue.h"
#include "Order.h"
#include "Truck.h"


using namespace std;

static const char CLIENT_QUIT = '1';
static const char GET_LOW_STOCK = '2';
static const char GET_ITEM_STOCK = '3';
static const char GET_ORDER_STATUS = '4';
static const char ADD_TEST_ORDERS = '5';
static const char ADD_DELIVERY_TRUCK = '6';
static const char ADD_RESTOCK_TRUCK = '7';

// print menu options
void print_menu() {

  safe_printf( "MANAGER MENU\n============\n(1) quit\n(2) get low stock items\n(3) get stock of an item\n(4) get order status\n(5) add test order of 10 apples\n(6) add a delivery truck\n(7) add a restock truck\n");
  safe_printf( "Enter a number:\n");
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
void init_shelves(vector<Shelf>& shelves, const WarehouseInfo& minfo, map<int, string>& product_map, map<string, int>& product_inventory, map<string,int>&product_weight) {
    for(int i = 0; i < minfo.cols; i++){
      for(int j = 0; j < minfo.rows; j++){
        if(minfo.maze[i][j] == SHELF_CHAR)
        {
          cout << "Shelf " << shelves.size() + 1;
          Shelf shelf_init(i,j,product_map, product_inventory,product_weight);
          shelves.push_back(shelf_init);
        }
      }
    }
}

void get_stock(string input, map<string,int>&product_inventory){
  if(product_inventory.find(input) != product_inventory.end())
    cout<< "Product "<<input<<" has "<<product_inventory[input]<< " in stock\n\n"<<endl;
  else
    safe_printf("Product not found\n\n");
}

void get_lowstock(map<string,int>&product_inventory){
  int lowinstock=0;
  for( auto & it : product_inventory){
    if(it.second <= 20){
      cout<< "Warning: " << it.first << " low in stock: " << it.second;
      safe_printf("\n");
      lowinstock = 1;
    }
  }
    if(!lowinstock){
      safe_printf("Nothing low in stock\n");
    }

}

void add_test_order(DynamicOrderQueue& order_queue, vector<Shelf>& shelves, map<string,int>&product_inventory, int& order_count){
    Product p("apple",2);
    int quantity = 10;

    Order o(order_count, ORDER_STATUS_NEW);//initialize order
    o.add(p,quantity);
    safe_printf("Added order (id):%d\n",order_count);
    order_count++;
    for (const auto& p : o.products) {
      safe_printf("Order contains %d %s\n", p.second, p.first.name.c_str());
    }

    //check if stuff is in stock
    for (const auto& p : o.products) {

      if (product_inventory[p.first.name] - p.second < 0) {
        safe_printf("Not enough stock\n");
        return;
      }
      else
        safe_printf("%s in stock\n",p.first.name.c_str());

      for (Shelf& s : shelves) {
        if (s.contains_product(p.first)) {
          int n = s.remove(p.first, quantity);
          quantity -= n;
          product_inventory[p.first.name] -= n;
          o.col_destination1 = s.col_;
          o.row_destination1 = s.row_;
          safe_printf("\t%d taken off shelf at location {%d, %d}\n", n, s.col_, s.row_);
        }
      }

      order_queue.add(o);
    }
}

void get_order_status(DynamicOrderQueue& order_queue){
  for (deque<Order>::iterator it = order_queue.buff_.begin(); it!=order_queue.buff_.end(); ++it)
    safe_printf("Order %d with status %d\n",it->order_id,it->order_status);
}

int main(int argc, char* argv[]) {

    std::string warehouselayout = "data/maze1.txt";
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

    //dock queue
    //WarehouseDockQueue = dock_queue;
    //truck queue
    DynamicOrderQueue order_queue;
    int order_count = 0;
    vector<Truck*> trucks;
    map<int, string> product_map{{0,"apple"},{1,"orange"},{2,"banana"},{3,"carrot"},{4,"potato"}};
    map<string, int> product_weight{{"apple",2},{"orange",2},{"banana",2},{"carrot",2},{"potato",3}};
    vector <Shelf> shelves;
    map<string, int> product_inventory;
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    init_shelves(shelves, memory->minfo, product_map, product_inventory, product_weight);
    safe_printf( "Shelves initialized, products distributed.\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    //initialize robots
    vector<Robot*> robots;
    for( int i = 0; i < 4; i++){
        robots.push_back( new Robot(i/*, order_queue, dock_queue*//*, order_queue*/));
    }
    safe_printf("Locations of robots initialized.\n");

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
        case CLIENT_QUIT:
          break;
        case GET_LOW_STOCK:{
          get_lowstock(product_inventory);
          break;
        }
        case GET_ITEM_STOCK:{
          safe_printf("Enter a product name: \n");
          char input[100];
          cin.getline(input, sizeof(input));
          string str = input;
          get_stock(str, product_inventory);
          break;
        }
        case GET_ORDER_STATUS:{
          get_order_status(order_queue);
          break;
        }
        case ADD_TEST_ORDERS:{
          add_test_order(order_queue,shelves,product_inventory,order_count);
          break;
        }
        case ADD_DELIVERY_TRUCK:
          break;
        case ADD_RESTOCK_TRUCK:
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