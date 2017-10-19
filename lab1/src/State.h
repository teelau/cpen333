#ifndef STATE_H
#define STATE_H

#include <iostream>

class State {
 public:
    double position;
    double velocity;
    double acceleration;
    double time;

    State(){
      position = 0;
      velocity = 0;
      acceleration = 0;
      time = 0;
    }
    State(double position, double velocity, double acceleration, double time){
      this->position = position;
      this->velocity = velocity;
      this->acceleration = acceleration;
      this->time = time;
    }
    void set(double pos, double vel, double acc, double time){
      this->position = pos;
      this->velocity = vel;
      this->acceleration = acc;
      this->time = time;
    }
  // implementation
};

inline std::ostream& operator<<(std::ostream& os, State& state) {  
  os << "t: " << state.time << ", x: " << state.position 
     << ", v: " << state.velocity << ", a: " << state.acceleration;
  return os;
}  

#endif