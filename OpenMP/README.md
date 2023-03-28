# Parallel programing with OpenMP

## Pi calculation

### Description
Here we use the Monte Carlo method to find an approximation of pi. Check
this link for the explanation of the algorithm:
https://www.geeksforgeeks.org/estimating-value-pi-using-monte-carlo/

First we have the serial computation using only one main thread. After
that we calculate pi using multiple threads given by the user. Also the
user needs to provide the number of throws. So to execute the program
you need to run "make" to build the executable and then run
"./pi_comp <number_of_threads> <throw_count>". To build the executable
run "make pi". We can also run the pi_script.py to perform an experiment.
Our program will be executed with different parameters multiple times each.
In that way we will get the average values of the experiment each time since
sometimes individual execution might return unrealistic results
caused by external factors. Feel free to mess with the constants inside
the script.

### Some details
Every thread will count it's own successful hits and then it needs to return
its result. For that we have a shared to all threads variable on which they
can all add their result. Obviously this is a critical segment so we need to
handle it properly. The simpliest but not that elegant way would be to use
the "omp critical" clause that OpenMP provides. That acts like a lock, so
the access to the variable would be serialised. We would also have to define
a local variable for each thread to store the result and then add it to the
shared one or else we would end up serialising the whole program (we wouldn't
want the parallel_estimation function to be executed as a critical segment).
However, in our implementation we use the "reduction" clause. Basically it
does the same thing, under the hood defines a local variable for each thread
and then adds the results in a critical segment.

Note that while increasing the number of threads we won't see a linear
reduction of the execution time. That's because handling threads has an
overhead and could even make our program underperform. That's why
sometimes we need to even consider if using threads is something that would
benefit us. Check the results of pi_scipt.py and come up with your own
conclusions :D

## Upper Matrix-vector multiplication

## Description
The program omp_mat_vect_rand.c is from the book "Introduction to Parallel
Programing" of Pachecco and is an implementation of the parallel multiplication
of a matrix and a vector. We created omp_upper_mat_vect_rand.c where we perform
the same multiplication only that this time the matrix has zeros below its
diagonal. Note that we optimised the multiplication algorithm to ignore the zeros.
We recommend to the user to execute the matrix_script.py where we will
do the multiplication for a different number of threads using different schedules
each time. When executing without the script, don't forget to set the environment
variable "OMP_SCHEDULE" equal to the desirable schedule.

> schedule: how the work load from the parallel for loop will be distributed to
            the available threads

## Results
Of course we can see an improvement for all of the scedules, however it's far
from ideal (linear), with one exception...when the schedule is dynamic. In that
case we see much better execution time. Why is that?

## Explanation
First we need to understand why not all of the elements of the result vector aren't
equally easy to calculate. It's because, as we said before, we have an upper diagonal
matrix and in our implemantation we ignore the zeros since the multiplication with
them will always equal to zero. That means that the more we progress in calculating
the y elements the more zeros we have in the matrix-row which means less multiplications
so less work load.
The default schedule of OpenMP is static, which means that the for loop is evenly divided
among the threads. That means that if we have a loop with 6 iterations and 3 threads then
the first thread will execute for i = 0,1 , the second for i = 2,3 , and the third for
i = 4,5. This implementation is pretty good, since we know before hand which thread will
do what which means it's fast, however in cases like this one where each iteration has a
different workload we need to approach the problem differently. OpenMP supports the dynamic
schedule which is more suitable for us. Basically whenever a thread finishes its
iteration, it gets assigned another one from the runtime system. By doing that, all threads
will be active at the same time for the most part and the workload will get distributed
more evenly. Keep in mind that in other cases dynamic scheduling could worsen by a lot
our execution time, because all that dynamic thread handling comes of course with a cost.

## False sharing
Just like the implementation in Pthreads, we come across the false sharing problem.
Go and read the README at the Pthreads folder for an explanation at false sharing. We
easily solve that by adding a local variable for each thread where all the calculations
will take place and then at the end the result is written at the vector. So we did an
experiment, the results are inside FalseSharingExperiment, we run with and without that
local variable. Of course we expect that without the local variable the execution
time will be higher. We can see that from the results too. However, we also see that
the dynamic schedule is much worse than the rest of the schedules, it's even worse than
the serial execution time.
### Possible explanation
By having dynamic schedule, all of threads terminate almost at the same time. That means
that through the whole execution, they "fight" for the cache line hence false sharing
is adding a lot of overhead. In the other approaches though, some threads finish earlier,
reducing the competition for the cache line.