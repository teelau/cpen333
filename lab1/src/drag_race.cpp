#include "Car.h"
#include "Car.cpp"
#include <iostream>
#include <string>
#include "State.h"

static const double QUARTER_MILE = 402.3;
//const double AIR_DENSITY = 1.225;

int main() {

  Car car1("Mazda 3", 1600, 790, 0.61);
  Car car2("Toyota Prius", 1450, 740, 0.58);

  // drive for 60 seconds
  double dt = 0.01;

  // GO!!!!
  car1.accelerate(true);
  car2.accelerate(true);
  for (double t = 0; t <= 60; t += dt) 
  {
    car1.drive(dt);
    car2.drive(dt);
    
    if(car1.getState().position >= QUARTER_MILE)
    {
      State temp = car1.getState();
      cout << car1 << endl << temp;// << " " << car1.getState() << endl;
      break;
    }
    else if(car2.getState().position >= QUARTER_MILE){
      State temp = car2.getState();
      cout << car2 << endl << temp;
      break;// << " " << car2.getState() << endl;
      // XXX print out who's in the lead
    }
  }

  return 0;
}