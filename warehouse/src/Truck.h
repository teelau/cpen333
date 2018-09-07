#ifndef TRUCK_H
#define TRUCK_H

#define MAXWEIGHT_ROBOT 50
#define MAXWEIGHT_TRUCK 200
//#define MAXWEIGHT_SHELF 100

#include <iostream>
#include <string>
#include <vector>
#include "Product.h"


class Truck{
  public:
  int max_weight = MAXWEIGHT_TRUCK;
  vector<Product> products;
  bool docked;
};

class DeliveryTruck : public Truck{
  public:
  bool addProduct(Product p){
    ;
  }
};

class RestockingTruck: public Truck{
  public:
  bool removeProduct(Product p){
    ;
  }
};

// class Location{
//   public:
//   int x;
//   int y;

//   Location(int X, int Y){
//     this->x = X;
//     this->y = Y;
//   }
  
//   void setLocation(int X, int Y){
//     this->x = X;
//     this->y = Y;
//     return;
//   }
// }

// class Shelf{
//   public:
//   Location loc;
//   int maxWeight = MAXWEIGHT_SHELF;
//   vector<Product> products;

//   //get the total weight of the products in the shelf
//   int getWeight(){
//     int totalWeight = 0;
//     for(auto& product : this->products ){
//       totalWeight += product.getWeight();
//     }
//     return totalWeight;
//   }
// }


// class Robot{
//   public:
//   bool busy = 0;
//   int maxWeight = MAXWEIGHT_ROBOT;
//   pair<Product, Location> task;

//   void restock(Product p, Location l){

//   }
  
//   void deliver(Product p, Location l){

//   }
// }

#endif