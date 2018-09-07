#ifndef WAREHOUSE_ORDER_H
#define WAREHOUSE_ORDER_H

#include <map>
#include "Product.h"
/**
 * Basic order information containing a customer id and item id
 */
#define ORDER_STATUS_NEW 0
#define ORDER_STATUS_PLACED 1
#define ORDER_STATUS_DELIVERED 2




class Order {
  public:
  int order_id;
  int order_status;
  map<Product, int> products;

  //shelf
  int col_destination1;
  int row_destination1;

  //truck
  int col_destination2;
  int row_destination2;

  Order(int id, int status) : 
  order_id(id), order_status(status)//, products_(products),
  { }

  bool add(Product p, int quantity) {
    auto it = products.find(p);
    if (it != products.end()) {
      products[p] += quantity;
      return false;
    }

    products.insert({p, quantity});
    return true;
  }

  void remove(Product p) {
    auto it = products.find(p);

    if (it != products.end()) {
      products.erase(it);
    }
  }

  friend bool operator<(const Order& a, const Order& b) {
    return a.order_id < b.order_id;
  }

  friend bool operator==(const Order& a, const Order& b) {
    return a.order_id == b.order_id;
  }

};

#endif //LAB6_ORDER_H
