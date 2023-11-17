assignment 5 - multithreader

in this assignment we had to implement a multithreader using lambda functions which were passed from the vector and matrix files.
and print the parallel execution time for each thread

we created a threads vector to stopre the threads during execution and defined 2 different functions for matrix and vector. i defined the parallel_for function which is called in matrix and vector,
iterated through the number of threads and created a threads using threadfunction by passing the necessary arguments
and joined the threads and performed necessary memory cleanup and error handling

in th ethread lambda functions, i iterated through the low and high values and called the lambda function repeatedly

also, printed the parallel execution time for eacg thread


github repository link - https://github.com/ananya-dot/os-multithreader.git

