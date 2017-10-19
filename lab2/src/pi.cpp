#include <thread>
#include <iostream>
#include <random>
#define RAND_TIME_SEED chrono::system_clock::now().time_since_epoch().count()
#define CURRENT_TIME_HIGH_RES std::chrono::high_resolution_clock::now()
using namespace std;

double estimate_pi(int nsamples) {
  double x,y;
  int p = 0;
  default_random_engine rnd(RAND_TIME_SEED);
  uniform_real_distribution<double> dist(-1.0,1.0);

  for( int i = 0; i < nsamples; i++)
  {
    x = dist(rnd);
    y = dist(rnd);
    if(sqrt(x*x + y*y) <= 1.0)
      p++;
  }
  return p*4.0/nsamples;
}

// generates a random sample and sets hits[idx]
// to true if within the unit circle
void pi_sampler(std::vector<bool>& hits, int idx) {

  // single instance of random engine and distribution
  static std::default_random_engine rnd(RAND_TIME_SEED);
  static std::uniform_real_distribution<double> dist(-1.0, 1.0);
  double x,y;
  x = dist(rnd);
  y = dist(rnd);
  hits[idx] = sqrt(x*x+y*y) <= 1.0;
}

// naively uses multithreading to try to speed up computations
double estimate_pi_multithread_naive(int nsamples) {
  // stores whether each sample falls within circle
  std::vector<bool> hits(nsamples, false);

  // create and store all threads
  std::vector<std::thread> threads;
  for (int i=0; i<nsamples; ++i) {
    threads.push_back(std::thread(pi_sampler, std::ref(hits), i));
  }

  // wait for all threads to complete
  for (int i=0; i<nsamples; ++i) {
    threads[i].join();
  }

  // estimate our value of PI
  double pi = 0;
  for (int i=0; i<nsamples; ++i) {
    if (hits[i]) {
      pi = pi + 1;
    }
  }
  pi = pi / nsamples*4;

  return pi;
}

// count number of hits using nsamples, populates hits[idx]
void pi_hits(std::vector<int>& hits, int idx, int nsamples) {

  // single instance of random engine and distribution
  static std::default_random_engine rnd(RAND_TIME_SEED);
  static std::uniform_real_distribution<double> dist(-1.0, 1.0);
  double x,y;

  for( int i = 0; i < nsamples; i++)
  {
    x = dist(rnd);
    y = dist(rnd);
    if(sqrt(x*x + y*y) <= 1.0)
      hits[idx]++;
  }
}

// divides work among threads intelligently
double estimate_pi_multithread(int nsamples) {

  // number of available cores
  int nthreads = std::thread::hardware_concurrency();

  // hit counts
  std::vector<int> hits(nthreads, 0);

  // create and store threads
  std::vector<std::thread> threads;
  int msamples = 0; // samples accounted for
  for (int i=0; i<nthreads-1; ++i) {
    threads.push_back(
        std::thread(pi_hits, std::ref(hits), i, nsamples/nthreads));
    msamples += nsamples/nthreads;
  }
  // remaining samples
  threads.push_back(
      std::thread(pi_hits, std::ref(hits), nthreads-1, nsamples-msamples));

  // wait for threads to finish
  for (int i=0; i<nthreads; ++i) {
    threads[i].join();
  }

  // estimate pi
  double pi = 0.0;
  for (int i=0; i<nthreads; ++i) {
    pi += hits[i];
    //cout << hits[i] << endl;
  }
  pi = pi/nsamples*4.0;

  return pi;
}

int main() {

  auto t1 = CURRENT_TIME_HIGH_RES;
  double pi = estimate_pi(1000);
  std::cout << "My estimate of PI no threads is: " << pi << std::endl;
  auto t2 = CURRENT_TIME_HIGH_RES;
  auto t_diff = t2-t1;
  auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t_diff);
  long ms = duration_ms.count();
  std::cout << "The operation took " << ms << " ms" << std::endl;

  t1 = CURRENT_TIME_HIGH_RES;
  pi = estimate_pi_multithread_naive(1000);
  std::cout << "My estimate of PI naive is: " << pi << std::endl;
  t2 = CURRENT_TIME_HIGH_RES;
  t_diff = t2-t1;
  duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t_diff);
  ms = duration_ms.count();
  std::cout << "The operation took " << ms << " ms" << std::endl;

  t1 = CURRENT_TIME_HIGH_RES;
  pi = estimate_pi_multithread(1000);
  std::cout << "My estimate of PI multithread is: " << pi << std::endl;
  t2 = CURRENT_TIME_HIGH_RES;
  t_diff = t2-t1;
  duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t_diff);
  ms = duration_ms.count();
  std::cout << "The operation took " << ms << " ms" << std::endl;
  return 0;
}