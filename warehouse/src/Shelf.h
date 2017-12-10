#ifndef SHELF_H
#define SHELF_H
#include <string>
#include <random>
#include <fstream>
#include "warehouse_common.h"
#include "Product.h"
#include <map>
using namespace std;

class Shelf{
  public:
  map< Product, int> products; // product & quantity
  int maxWeight = MAXWEIGHT_SHELF;
  int col_;
  int row_;
  int random;

  Shelf(int col, int row, map<int, string>& product_map) :
  col_(col), row_(row)
  {
    //random number generation
    std::default_random_engine rnd((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    //std::default_random_engine rnd2((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(0, product_map.size() - 1);
    //std::uniform_int_distribution<int> dist2(5, 10);

    int quantity = 10;
    random = dist(rnd);

    //insert product and quantity into map
    string product_name = product_map[random];
    Product product_insert(product_name, 5);
    products.insert({product_insert, quantity});

    cout <<" initialized with " << quantity << " " << product_name << "s at {" << col_ << "," << row_ << "}" << endl;
  }

  void addProduct(){

  }

  int getWeight(){
    //return this->quantity*this->weight;
  }
};

#endif