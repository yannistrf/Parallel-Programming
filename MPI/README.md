# Parallel programing with MPI
In this project we experiment with two different kinds of problems
and face different challenges. It is highly recommended to run the
experiments and see the results for yourself for a better understanding. 
This project was made for my Parallel Systems course at National
Kapodistrian University of Athens, Department of Informatics and
Telecommunications.

## Pi calculation

### General
Here we use the Monte Carlo method to find an approximation of pi. Check
this link for the explanation of the algorithm:
https://www.geeksforgeeks.org/estimating-value-pi-using-monte-carlo/

First the process 0 executes the serial algorithm and then we proceed
to the parallel one using all of the processes. To compile run "make pi"
and to execute "mpiexec -n <num_of_processes> ./bin/pi_comp <throw_count>.
With the proper configuration we could use multiple computers to execute
our program, however for the sake of simplicity we execute locally.
You can also run the pi_script.py to perform an experiment.
Our program will be executed with different parameters multiple times each.
In that way we will get the average values of the experiment each time since
sometimes individual execution might return unrealistic results
caused by external factors. Feel free to mess with the constants inside
the script.

## Matrix-vector multiplication

### General
We will multiply a square matrix and a vector but not with the traditional way.
Someone would expect to do it as inside the mpi_mat_vect_time.c program,
splitting the matrix in block rows from the book "Introduction to Parallel
Programing" of Pachecco. Here we will do the multiplication by splitting
the matrix in block columns. To build the executable run "make mat" and to
execute it run "mpiexec -n <num_of_processes> ./bin/mpi_mat_vect <matrix_dim>"
or "make runmat" to execute with arguments defined inside the Makefile.

We will not explain exactly how the multiplication is
happening but here is an example to help you understand.

          | 1 1 6 | | 2 |     | 1 |     | 1 |     | 6 |   | 7 |
     Ax = | 3 0 3 | | 5 | = 2 | 3 | + 5 | 0 | + 0 | 3 | = | 6 |
          | 1 1 4 | | 0 |     | 1 |     | 1 |     | 4 |   | 7 |

As we can see, each process can get some columns of A, a part of x and
then calculate a fraction of the final x element. After that, process 0
will collect these fractions, add them up and get the final result.
