#include <iostream>
#include <random>
#include <thread>
#include <cmath>   // for exp, abs
#include <math.h>
#define RAND_TIME_SEED chrono::system_clock::now().time_since_epoch().count()
#define CURRENT_TIME_HIGH_RES std::chrono::high_resolution_clock::now()
#define DENSITY_FUNCTION Density3
#define TIME_START t1 = CURRENT_TIME_HIGH_RES;
#define TIME_END   t2 = CURRENT_TIME_HIGH_RES;\
                    t_diff = t2-t1;\
                    duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t_diff);\
                    ms = duration_ms.count();\
                    std::cout << "The operation took " << ms << " ms" << std::endl;
using namespace std;

// three-dimensional point
struct Point {
  double x,y,z;
};

// virtual base class for functions
class Function {
 public:
  virtual double operator()(double x, double y, double z) = 0;
};

// computes x*fn(x,y,z)
class XFunction : public Function {
  Function& fn;
 public:
  XFunction(Function& fn) : fn(fn){};
  double operator()(double x, double y, double z) {
    return x*fn(x,y,z);
  }
};

// computes y*fn(x,y,z)
class YFunction : public Function {
  Function& fn;
 public:
  YFunction(Function& fn) : fn(fn){};
  double operator()(double x, double y, double z) {
    return y*fn(x,y,z);
  }
};

// computes z*fn(x,y,z)
class ZFunction : public Function {
  Function& fn;
 public:
  ZFunction(Function& fn) : fn(fn){};
  double operator()(double x, double y, double z) {
    return z*fn(x,y,z);
  }
};

// new function for representing density 1
class Density1 : public Function {
 public:
  double operator()(double x, double y, double z) {
    double norm2 = x*x+y*y+z*z;
    return norm2 > 1 ? 0 : exp(-norm2);
  }
};

class Density2 : public Function {
public:
  double operator()(double x, double y, double z) {
    double temp = x*x+y*y+z*z;
    return temp > 1 ? 0 : abs(x+y+z);
  }
};

class Density3 : public Function {
public:
  double operator()(double x, double y, double z) {
    double temp = x*x+y*y+z*z;
    return temp > 1 ? 0 : pow(x-1,2) + pow(y-2,2) + pow(z-3,2);
  }
};

void estimate_cx(int nsamples, vector<Point>& points, vector<double>& sums, int idx, Function& fn)
{
  double sum = 0;
  XFunction xd(fn);
  for(int i = 0; i < nsamples; i++)
  {
    sum += xd(points[i].x,points[i].y,points[i].z);
  }
  sums[idx] = sum * (4 / 3) * M_PI / nsamples;
}

void estimate_cy(int nsamples, vector<Point>& points, vector<double>& sums, int idx, Function& fn)
{
  double sum = 0;
  YFunction yd(fn);
  for(int i = 0; i < nsamples; i++)
  {
    sum += yd(points[i].x,points[i].y,points[i].z);
  }
  sums[idx] = sum * (4 / 3) * M_PI / nsamples;
}
void estimate_cz(int nsamples, vector<Point>& points, vector<double>& sums, int idx, Function& fn)
{
  double sum = 0;
  ZFunction zd(fn);
  for(int i = 0; i < nsamples; i++)
  {
    sum += zd(points[i].x,points[i].y,points[i].z);
  }
  sums[idx] = sum * (4 / 3) * M_PI / nsamples;
}


//threaded density function
void estimate_c_density_threaded(int nsamples, Function& fn) 
{
  //generate random bullshit
  default_random_engine rnd(RAND_TIME_SEED);
  uniform_real_distribution<double> dist(-1.0,1.0);
  vector <Point> points;
  Point point;
  for(int i = 0; i < nsamples; i++){
    point.x = dist(rnd);
    point.y = dist(rnd);
    point.z = dist(rnd);
    points.push_back(point);
  }

  double integral_p = 0;
  //integral of p(x)dx = 1/N sum( vol(V)*f(x) )
  for(int i = 0; i < nsamples; i++){
    integral_p += fn(points[i].x,points[i].y,points[i].z);
  }
  integral_p *= (4 / 3) * M_PI / nsamples;
  
  //hold data for the result of 3 sums
  vector <double> sums;
  for(int i = 0; i < 3; i++){
    sums.push_back(0);
  }

  vector<thread>threads;
  threads.push_back(thread(estimate_cx, nsamples, ref(points), ref(sums), 0, ref(fn)));
  threads.push_back(thread(estimate_cy, nsamples, ref(points), ref(sums), 1, ref(fn)));
  threads.push_back(thread(estimate_cz, nsamples, ref(points), ref(sums), 2, ref(fn)));
  for(int i =0; i < 3; i++){
    threads[i].join();
  }
  cout << "cx: " << sums[0]/integral_p << endl;
  cout << "cy: " << sums[1]/integral_p << endl;
  cout << "cz: " << sums[2]/integral_p << endl;

}

void estimate_c_density_iterative(int nsamples, Function& fn) 
{
  //generate random bullshit
  default_random_engine rnd(RAND_TIME_SEED);
  uniform_real_distribution<double> dist(-1.0,1.0);
  vector <Point> points;
  Point point;
  for(int i = 0; i < nsamples; i++){
    point.x = dist(rnd);
    point.y = dist(rnd);
    point.z = dist(rnd);
    points.push_back(point);
  }

  double integral_p = 0;
  //integral of p(x)dx = 1/N sum( vol(V)*f(x) )
  for(int i = 0; i < nsamples; i++){
    integral_p += fn(points[i].x,points[i].y,points[i].z);
  }
  integral_p *= (4 / 3) * M_PI / nsamples;
  
  //hold data for the result of 3 sums
  vector <double> sums;
  for(int i = 0; i < 3; i++){
    sums.push_back(0);
  }

  estimate_cx(nsamples, points, sums, 0, fn);
  estimate_cy(nsamples, points, sums, 1, fn);
  estimate_cz(nsamples, points, sums, 2, fn);

  cout << "cx: " << sums[0]/integral_p << endl;
  cout << "cy: " << sums[1]/integral_p << endl;
  cout << "cz: " << sums[2]/integral_p << endl;

}

int main() {
  int nsamples = 1000000;
  
  cout<<"Density function 1" << endl;
  Density1 density_function_1;
  cout<<"threaded: " << endl;
  auto t1 = CURRENT_TIME_HIGH_RES;
  estimate_c_density_threaded(nsamples, density_function_1 );
  auto t2 = CURRENT_TIME_HIGH_RES;
  auto t_diff = t2-t1;
  auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t_diff);
  long ms = duration_ms.count();
  std::cout << "The operation took " << ms << " ms" << std::endl;

  cout<<"iterative: " << endl;
  TIME_START
  estimate_c_density_iterative(nsamples, density_function_1 );
  TIME_END

  
  Density2 density_function_2;
  cout<< endl << "Density function 2" << endl;
  cout<<"threaded: "<< endl;
  TIME_START
  estimate_c_density_threaded(nsamples,density_function_2);
  TIME_END
  cout <<"iterative: "<< endl;
  TIME_START
  estimate_c_density_iterative(nsamples,density_function_2);
  TIME_END

  Density3 density_function_3;
  cout<< endl << "Density function 3" << endl;
  cout<<"threaded: "<< endl;
  TIME_START
  estimate_c_density_threaded(nsamples,density_function_3);
  TIME_END
  cout <<"iterative: "<< endl;
  TIME_START
  estimate_c_density_iterative(nsamples,density_function_3);
  TIME_END
  // estimate_c_density2(nsamples,points);
  // estimate_c_density3(nsamples,points);



  // sample usage of functions
  // Density1 d1;
  // XFunction xd1(d1);  // x*d1(x,y,z)
  // YFunction yd1(d1);  // y*d1(x,y,z)
  // ZFunction zd1(d1);  // z*d1(x,y,z)

  // std::cout << "d1(0.1,0.2,0.3): " << d1(0.1,0.2,0.3) << std::endl;
  // std::cout << "xd1(0.1,0.2,0.3): " << xd1(0.1,0.2,0.3) << std::endl;
  // std::cout << "yd1(0.1,0.2,0.3): " << yd1(0.1,0.2,0.3) << std::endl;
  // std::cout << "zd1(0.1,0.2,0.3): " << zd1(0.1,0.2,0.3) << std::endl;

  // YOUR CODE HERE
  //populate random points


  return 0;
}

