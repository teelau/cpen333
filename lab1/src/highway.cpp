#include <vector>
#include <iostream>
#include "Car.h"
#include "Car.cpp"
#include "State.h"
#include "physics.h"

int main() {

  vector <Car*> fleet(160);
  //vector <Car> models({Prius(), Tesla3(), Mazda3(), Herbie()});
  //models[0] // == Prius()
  for(int i = 0; i < fleet.size(); i++)
  {
    switch(i%4)
    {
      case 0 :
        fleet[i] = new Prius();
        break;
      case 1 :
        fleet[i] = new Tesla3();
        break;
      case 2 :
        fleet[i] = new Mazda3();
        break;
      default:
	fleet[i] = new Herbie();

    }
    
  }

  int minutes = 10;
  int minute_count = 0;
  double dt = 0.01;
  for (double t = 0; t <= 60*minutes; t += dt) 
  {
    for(Car *car: fleet)
    {
      if(car->getState()->velocity > 27.8)
        car->accelerate(false);
      else
        car->accelerate(true);
      car->drive(dt);
    }

    if( t > 60*minute_count )
    {
      //printy
      State temp = *fleet[0]->getState();
      cout << *fleet[0] << endl << temp << endl << endl;
      State temp2 = *fleet[1]->getState();
      cout << *fleet[1] << endl << temp2 << endl << endl;
      State temp3 = *fleet[2]->getState();
      cout << *fleet[2] << endl << temp3 << endl << endl;
      State temp4 = *fleet[3]->getState();
      cout << *fleet[3] << endl << temp4 << endl << endl;
      cout<<"------"<<endl;
      //
      minute_count++;
    }

  }

  for(Car *car: fleet)
  {
    delete car;
  }
  // for(Car& car: fleet){
  //   cout<<car.getModel() << endl;
  // }
  return 0; 
}
