#include "Car.h"

// implementation details..
Car::Car(std::string model, double mass, double engine_force, double drag_area)
{
    this->model = model;
    this->mass = mass;
    this->engine_force = engine_force;
    this->drag_area = drag_area;
}

string Car::getModel()
{
    return this->model;
}

double Car::getMass()
{
    return this->mass;
}

double Car::getEngineForce()
{
    return this->engine_force;
}

double Car::getDragArea()
{
    return this->drag_area;
}

void Car::accelerate(bool on)
{
    this->accelerating = on;
}

void Car::drive(double dt)
{
    const double AIR_DENSITY = 1.225;

    double drag_force = physics::compute_dragforce(AIR_DENSITY,this->drag_area,this->state.velocity);
    double net_force = this->engine_force - drag_force;
    if(!this->accelerating)
        net_force = 0 - drag_force;
    this->state.set(
         physics::compute_position(this->state.position,this->state.velocity,dt)
        ,physics::compute_velocity(this->state.velocity,this->state.acceleration,dt)
        ,physics::compute_acceleration(net_force,this->mass)
        ,this->state.time + dt
    );
}

State * Car::getState()
{
    return &this->state;
}

void Herbie::drive(double dt)
{
    const double AIR_DENSITY = 0;

    double drag_force = physics::compute_dragforce(AIR_DENSITY,this->drag_area,this->state.velocity);
    double net_force = this->engine_force - drag_force;
    if(!this->accelerating)
        net_force = 0 - drag_force;

    this->state.set(
         physics::compute_position(this->state.position,this->state.velocity,dt)
        ,physics::compute_velocity(this->state.velocity,this->state.acceleration,dt)
        ,physics::compute_acceleration(net_force,this->mass)
        ,this->state.time + dt
    );
}