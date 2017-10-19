#include "Car.h"
#include "Car.cpp"
#include "State.h"
#include "physics.h"
#include <iostream>
#include <string>

int main()
{

    Car test_car("modtest", 1500,750,.58);
    State test = test_car.getState();
    test.set(50,50,50,50);
    State test2 = test_car.getState();
    cout << test_car << endl << test;
    cout << endl << test2;
}