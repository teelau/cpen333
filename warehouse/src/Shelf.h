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

  Shelf(int col, int row, map<int, string>& product_map, map<string, int>&product_inventory, map<string,int>&product_weight) :
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
    int weight = product_weight[product_name];
    Product product_insert(product_name, weight);
    products.insert({product_insert, quantity});

    if(product_inventory.find(product_name) != product_inventory.end())
      product_inventory[product_name] += quantity;
    else
      product_inventory.insert({product_name,quantity});

    //cout << "TESTING PRODUCT INVENTORY: " << product_name << " " << product_inventory[product_name] << endl;
    cout <<" initialized with " << quantity << " " << product_name << "s at {" << col_ << "," << row_ << "} weighs " << product_insert.weight*quantity << endl;
  }

  int remove(Product p, int quantity) {
    auto it = products.find(p);
    if (it != products.end()) {
      int n = it->second - quantity;
      
      if (n <= 0) {
        products.erase(it);
        return n < 0 ? it->second : quantity;
      } else {
        products[p] -= quantity;
        return quantity;
      }
    }

    return 0;
  }

  bool contains_product(Product p) {
    auto it = products.find(p);
    if (it != products.end()) {
      return true;
    }

    return false;
  }

  void addProduct(){

  }

  int getWeight(){
    //return this->quantity*this->weight;
  }
};

#endif