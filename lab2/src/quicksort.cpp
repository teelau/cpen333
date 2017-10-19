#include <iostream>
#include <vector>
#include <chrono>
#include <iostream>
#include <thread>
#define CURRENT_TIME_HIGH_RES std::chrono::high_resolution_clock::now()
using namespace std;

// test if an array of size > 0 is sorted
bool check_sorted( vector<int>&data, int size)
{
  for(int i = 1; i < size; i++)
  {
    if(data[i-1] > data[i])
      return false;
  }
  return true;
}

//swap position of two characters
void swap_characters( int &a, int &b)
{
  int temp = a;
  a = b;
  b = temp;
}

// partitions elements low through high (inclusive)
// around a pivot and returns the pivot index
size_t partition(std::vector<int>& data, int low, int high) {
  int pivot = data[high];
  int i = low-1;
  for(int j = low; j < high;j++)
  {
    if( data[j] < pivot )
    {
      i++;
      swap_characters(data[i], data[j]);
    }
  }
  if( data[high] < data[i+1] )
  {
    swap_characters( data[i+1], data[high] );
  }
  return i+1;
}

//sorts elements low through high (inclusive) using a single thread
void quicksort(std::vector<int>& data, int low, int high) {

  if( low < high )
  {
    int p = partition( data, low, high );    
    quicksort(data,low,p-1);
    quicksort(data,p+1,high);
  }
  
}

// sorts elements low through high (inclusive) using multiple threads
void parallel_quicksort(std::vector<int>& data, int low, int high) {
  if( low < high )
  {
    int p = partition( data, low, high);
    std::thread* thread_sort = new std::thread(quicksort, ref(data), low, p - 1);
    quicksort( data, p +1 , high);
    thread_sort->join();
    delete thread_sort;
    thread_sort = nullptr;
  }

}

int main() {

  // create two copies of random data
  //const int VECTOR_SIZE = 1000000;
  const int VECTOR_SIZE = 1000000;
  std::vector<int> v1(VECTOR_SIZE, 0);
  // fill with random integers
  for (int i=0; i<VECTOR_SIZE; ++i) {
    v1[i] = rand();
  }
  std::vector<int> v2 = v1;  // copy all contents

  // sort v1 using sequential algorithm
  auto t1 = CURRENT_TIME_HIGH_RES;
  quicksort(v1, 0, v1.size()-1);
  auto t2 = CURRENT_TIME_HIGH_RES;
  auto t_diff = t2-t1;
  auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t_diff);
  long ms = duration_ms.count();
  std::cout << "The operation took " << ms << " ms" << std::endl;

  t1 = CURRENT_TIME_HIGH_RES;
  parallel_quicksort(v2, 0, v2.size()-1);
  t2 = CURRENT_TIME_HIGH_RES;
  t_diff = t2-t1;
  duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t_diff);
  ms = duration_ms.count();
  std::cout << "The operation took " << ms << " ms" << std::endl;

  ///*print ALL ELEMENTS  of an array*/
  // cout << endl;
  // for(auto&v1_it : v1){
  //   cout << v1_it << " ";
  // }
  // cout << endl << endl;
  // for(auto&v2_it : v2){
  //   cout << v2_it << " ";
  // }
  // cout << endl << endl;

  if(check_sorted(v1,VECTOR_SIZE))
    cout << "sequential is sorted" << endl;
  else
    cout << "sequential is not sorted" << endl;

  if(check_sorted(v2,VECTOR_SIZE))
    cout << "parallel is sorted" << endl;
  else
    cout << "parallel is not sorted" << endl;

  ///*good for testing if the swap character function worked!*/
  // vector<int> test = {2,3,4,5,6,7,8,9};

  // swap_characters(test[0],test[1]);
  // for(int i = 0; i < test.size(); i++){
  //   cout << test[i];
  // }
  //system("pause");

  return 0;
}