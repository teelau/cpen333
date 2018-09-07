#include "warehouse_common.h"
#include <string>
#include <fstream>
#include <thread>
#include <random>
#include <cpen333/process/shared_memory.h>
#include "Robot.h"
#include "safe_printf.h"
#include "Product.h"
#include "Shelf.h"
#include <map>
#include "Order.h"
using namespace std;

class TestException : public std::exception {
  const std::string msg_;

 public:
  TestException(const std::string& msg) : msg_(msg) {}

  const char* what() {
    return msg_.c_str();
  }
};

void test_product_add_name(Product p, string test){
    string result = p.name;

    if( result != test) {
        throw TestException("Failed to add name");
    }
}

void test_product_map(map<int, string>& pmap){
    if(pmap[0] != "apple")
        throw TestException("Failed to access products");
}

void test_add_products_to_order(Order o, Product p1, Product p2, int q1, int q2, int total){
    o.add(p1, q1);
    o.add(p2, q2);

    if( o.products[p1] != total )
        throw TestException("Failed to add product on order");
}
// void test_product_get_weight(Product p, int wt){
//     if(p.getWeight()!=wt){
//         throw TestException("Failed to get weight");
//     }
// }

// void test_product_overload_op(Product p, string s){
//     if( p == s ){
//         throw TestException("Failed to use overloaded operator");  
//     }
// }

// void test_product_overload_product(Product p, Product pp){
//     if(!(p==pp)){
//         throw TestException("Failed to use overloaded product");
//     }
// }
int main(){


    try{
        Product p("hello",2);
        Product p2("goodbye",3);
        Product p1("hello",2);
        Order o(1,0);
        map<int, string> product_map{{0,"apple"},{1,"orange"},{2,"banana"},{3,"carrot"},{4,"potato"}};


        test_product_add_name(p,"hello");
        test_product_add_name(p2,"goodbye");
        test_product_add_name(p1,"hello");
        test_product_map(product_map);

        //cout<<"Size of product map = " << product_map.size() << endl;
        //test_product_get_weight(p,8);
        // test_product_overload_op(p, "hello");
        // test_product_overload_product(p,p2);
        // Shelf a(1,1,product_map);
        // cout<< "Shelf Location: " << a.col_ << " " << a.row_ << endl;
        // cout<< "Random number generated: " << a.random << endl;
        test_add_products_to_order(o,p,p,6,6,12);
        test_add_products_to_order(o,p,p,1,1,2);
        test_add_products_to_order(o,p,p2,6,6,6);



        cout<<"Tests passed";
    } catch(TestException &exc){
        cout << exc.what() << endl;
    }
}