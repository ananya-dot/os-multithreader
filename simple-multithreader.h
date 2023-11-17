#include <iostream>
#include <list>
#include <functional>
#include <stdlib.h>
#include <cstring>
#include <vector>


using namespace std;


int user_main(int argc, char **argv);
vector<pthread_t> threads;        // a thread vector for storing all the threads
clock_t start_time;               // start time for printing the time for parallel execution
clock_t sum_of_times = 0;
int no_of_threads;


std::function<void(int)> threadLambda1;             // variable to store vector lambda function
std::function<void(int, int)> threadLambda2;        // variable to stpre matrix lambda function


void demonstration(std::function<void()> &&lambda) {
   lambda();
}


static void* threadFunction(void *args);



void* threadFunction(void *args) {
   int* argArray = static_cast<int*>(args);
   int low = argArray[0];                       // taking parameters in the form of an array.
   int high = argArray[1];
   int threadId = argArray[2];


    delete[] argArray;                        // deleting it after use to avoid taking up extra space


   for (int i = low + threadId; i <= high; i += threads.size()) {
       threadLambda1(i);                              // execute the lambda function ewhich was captured in parallel for
   }


   return nullptr;
}


void* threadFunction2(void *args) {
  int* argArray = static_cast<int*>(args);
   int low1 = argArray[0];
   int high1 = argArray[1];                           // similar to threadFunction1 but for 2d vector
   int low2 = argArray[2];
   int high2 = argArray[3];
   int threadId = argArray[4];
   


   delete[] argArray;


   for (int i = low1 + threadId; i < high1; i += threads.size()) {
       for (int j = low2; j < high2; ++j) {
           threadLambda2(i, j);
       }
   }


   return nullptr;
}


int main(int argc, char **argv) {
   int x = 5, y = 1;


   auto lambda1 = [x, &y]() {
       y = 5;
       std::cout << "====== Welcome to Assignment-" << y << " of the CSE231(A) ======\n";
   };


   demonstration(lambda1);


   int rc = user_main(argc, argv);
   // std::cout << "Total parallel execution time: " << sum_of_times << " seconds" << std::endl;




   auto lambda2 = []() {
       std::cout << "====== Hope you enjoyed CSE231(A) ======\n";
   };


   demonstration(lambda2);


   return rc;
}


#define main user_main


void parallel_for(int low, int high, std::function<void(int)> &&lambda, int numThreads) {
   start_time = clock();


   threads.resize(numThreads);
   threadLambda1 = std::move(lambda); // storing lambda function to pass to thread function


   for (int i = 0; i < numThreads; ++i) {
      //  int* args = new int[3]{low, high, i};
       int* args = new (std::nothrow) int[3]{low, high, i};
if (!args) {
    cerr << "Error allocating memory" << endl;
    exit(EXIT_FAILURE); // Or handle the error accordingly
}

      
      //  pthread_create(&threads[i], nullptr, threadFunction, static_cast<void*>(args));
       int result = pthread_create(&threads[i], nullptr, threadFunction, static_cast<void*>(args));
if (result != 0) {
    cerr << "Error creating thread: " << strerror(result) << endl;
    exit(EXIT_FAILURE); 

   }

   }
   for (pthread_t& thread : threads) {
    int result = pthread_join(thread, nullptr);
if (result != 0) {
    cerr << "Error joining thread: " << strerror(result) << endl;
    exit(EXIT_FAILURE); 
}

      //  pthread_join(thread, nullptr);
   }


   double duration = static_cast<double>(clock() - start_time) / CLOCKS_PER_SEC;


   sum_of_times += duration;
   std::cout << "Parallel execution time: " << duration * 1000 << " miliseconds" << std::endl;
}


void parallel_for(int low1, int high1, int low2, int high2,
std::function<void(int, int)> &&lambda, int numThreads) {
   start_time = clock();


   threads.resize(numThreads);
   threadLambda2 = std::move(lambda);


   for (int i = 0; i < numThreads; ++i) {
       int* args = new int[5]{low1, high1, low2, high2, i};
      //  pthread_create(&threads[i], nullptr, threadFunction2, static_cast<void*>(args));
       int result = pthread_create(&threads[i], nullptr, threadFunction2, static_cast<void*>(args));
if (result != 0) {
    cerr << "Error creating thread: " << strerror(result) << endl;
    exit(EXIT_FAILURE); // Or handle the error accordingly
}

   }


   for (pthread_t& thread : threads) {
      //  pthread_join(thread, nullptr);
       int result = pthread_join(thread, nullptr);
if (result != 0) {
    cerr << "Error joining thread: " << strerror(result) << endl;
    exit(EXIT_FAILURE); // Or handle the error accordingly
}

   }


   double duration = static_cast<double>(clock() - start_time) / CLOCKS_PER_SEC;
   // sum_of_times += duration;
   std::cout << "Parallel execution time: " << duration * 1000 << " miliseconds" << std::endl;
}

