# Parallel programing with Pthreads
In this project we experiment with two different kinds of problems
and face different challenges. It is highly recommended to run the
experiments and see the results for yourself for a better understanding. 
This project was made for my Parallel Systems course at National
Kapodistrian University of Athens, Department of Informatics and
Telecommunications.

## Pi calculation

### Description
Here we use the Monte Carlo method to find an approximation of pi. Check
this link for the explanation of the algorithm:
https://www.geeksforgeeks.org/estimating-value-pi-using-monte-carlo/

First we have the serial computation using only one main thread. After
that we calculate pi using multiple threads given by the user. Also the
user needs to provide the number of throws. So to execute the program
you need to run "make" to build the executable and then run
"./pi_comp <number_of_threads> <throw_count>". Or you can run "make run"
for automatic execution with arguments defined inside the Makefile.
We can also run the pi_script.py to perform an experiment. Our program
will be executed with different parameters multiple times each. In that
way we will get the average values of the experiment each time since
sometimes individual execution might return unrealistic results
caused by external factors. Feel free to mess with the constants inside
the script. 

### Some details
Every thread at some point will have to return their result to the main
program. This is done by adding their successful hits at the circle_hits
variable. However what if multiple threads want to change that value
at the same time? That's what we call a critical segment and if we don't
handle it properly then we could get some very wrong and unexpected results.
In this implementation, we use an atomic operation. An atomic operation is
guaranteed to get executed without getting interrupted. An operation like
that can be used only when we modify the contents of a specific address, not
more, so it's kinda perfect for our problem. We could also use pthread mutexes
that act as lock for some commands but I chose an atomic operation as it is
more low level hence more efficient.

Note that while increasing the number of threads we won't see a linear
reduction of the execution time. That's because handling threads has an
overhead and could even make our program underperform. That's why
sometimes we need to even consider if using threads is something that would
benefit us. Check the results of pi_scipt.py and come up with your own
conclusions :D

## Matrix-vector multiplication

### Description
The program pth_mat_vect_rand.c is from the book "Introduction to Parallel
Programing" of Pachecco and is an implementation of the parallel multiplication
of a matrix and a vector. Our challenge is to understand why for certain
inputs it's strangely slow and to find a way to improve that.
We recommend to the user to execute matrix_script.py where we will perform
the multiplication with three different methods for different matrix-vector
dimensions. You will observe that when running with a small number of rows
and a big number of columns the program is much slower in comparison with the
other methods and the other inputs.

### Problem
This is happening because of something called false sharing. First we need to
understand that we are able to do the multiplication in parallel because each thread
can be responsible for the calculation of a certain part of the vector-result, without
interfering with the rest of the threads. So that means that there is no critical
segment, since each thread will be writing to different addresses.
Now we need to recall some of our computer architecture knowledge. Each CPU doesn't
communicate directly with the memory, it communicates first with its cache memory where
are stored the most recently used variables. This block of memory stores information
in the form of cache lines (64 bytes usually). So when one thread will want to write
at y[i] then the whole cache line will be marked as modified. That means that
the rest of the threads will have to update their own cache line and mark it as dirty.
Then the new updated cache line will have to be brought from the main memory in order
achieve memory coherence. So when the rows are not many, then that means that the
result vector will be able to be stored in a few cache lines. Since in every loop
iteration these cache lines will be set to dirty, then the main memory will be accessed
very often which slows things down.

### Solution
The first solution, implemented in pth_mat_vect_rand1.c is to store some trash information
inside each cache line. By doing that, each thread will "occupie" its own cache line and
won't interfere with the other ones. The only downside is the abuse of memory.

The second solution and probably the simpliest one, implemented in pth_mat_vect_rand2.c is
to create a private variable for each thread in which all the calculations will happen. In
that way, each thread will write inside the y array only once, when it has calculated the
result, reducing by a lot the false sharing effect.