#ifndef PRODUCT_H
#define PRODUCT_H
#include <string>
using namespace std;

class Product{
  public:
  string name;
  int weight;

  Product(string name, int weight){
    this->name = name;
    this->weight = weight;
  }
  
  void setName(string s){
    this->name = s;
  }
  friend bool operator<(const Product& a, const Product& b) {
    return a.name < b.name;
  }

  friend bool operator==(const Product& a, const Product& b) {
    return a.name == b.name;
  }
};

#endif