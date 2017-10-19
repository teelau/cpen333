#ifndef CAR_H
#define CAR_H

#include <string>
#include <iostream>
#include "State.h"
#include "physics.h"
using namespace std;

class Car 
{
protected:
  // member variables
  string model;
  double mass;
  double engine_force;
  double drag_area;
  bool accelerating;
  State state;
 public:
  // constructor
  Car(std::string model, double mass, double engine_force, double drag_area);
  
  std::string getModel();    // gets the model name
  double getMass();          // mass of the car
  double getEngineForce();
  double getDragArea();
  void accelerate(bool on);  // turn the accelerator on/off
  //virtual void drive(double dt);
  void drive(double dt);     // drive the car for an amount of time dt
  State * getState();          // returns the car's state
};

 inline std::ostream& operator<<(std::ostream& os, Car& car) {  
   os << "model: " << car.getModel() << ", mass: " << car.getMass() 
      << ", engine force: " << car.getEngineForce() << ", drag area: " << car.getDragArea()
     // << ", State: " << car.getState();
     ;
   return os;
 }  

 class Prius : public Car {
 public:
  Prius(): Car("Toyota Prius", 1450, 740, 0.58) {}
 };

 class Mazda3 : public Car {
public:
 Mazda3(): Car("Mazda 3", 1600, 790, 0.61) {}
};

class Tesla3 : public Car {
public:
 Tesla3(): Car("Tesla Model 3", 1450, 740, 0.58) {}
};

 class Herbie: public Car {
 public:
  Herbie(): Car("Herbie",1450, 740, 0.58) {}
  void drive(double dt);
 };
#endif