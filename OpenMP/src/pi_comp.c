#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include "my_rand.h"
#include "timer.h"


//Global variables.
long long int throw_count;
int num_of_threads;
double* thread_times;

long long int parallel_estimation(int throw_sample) {
    double start, finish;

    GET_TIME(start);
    int my_rank = omp_get_thread_num();
    long local_throw_count = throw_count / num_of_threads;
    long long int local_circle_hits = 0;

    for(long long i = 0; i < local_throw_count; i++) {
        unsigned int seed = time(NULL) + i + my_rank;
        double x_rand = get_rand(&seed);
        double y_rand = get_rand(&seed);
        double dist_square = x_rand * x_rand + y_rand * y_rand;
        if(dist_square <= 1) {
            local_circle_hits++;
        }
    }
    GET_TIME(finish);
    thread_times[my_rank] = finish - start;   
    
    return local_circle_hits;
}


int main(int argc, char** argv){

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number_of_threads> <throw_count>\n", argv[0]);
        exit(-1);
    }

    num_of_threads = atoi(argv[1]);
    throw_count = atoi(argv[2]);
    long long int circle_hits = 0;


    // Serial algorithm for the estimation of pi.
    long double pi_estimation_serial;
    double start_serial;
    double stop_serial;

    GET_TIME(start_serial);
    for(long long int throw = 0; throw < throw_count; throw++){
        unsigned int seed = time(NULL) + throw;
        double x_rand = get_rand(&seed);
        double y_rand = get_rand(&seed);
        double dist_square = x_rand * x_rand + y_rand * y_rand;
        if(dist_square <= 1) circle_hits++; 
    }
    pi_estimation_serial = 4 * circle_hits / ((double) throw_count);
    GET_TIME(stop_serial);
    
    double serial_time = stop_serial - start_serial;
    printf("Time for serial   estimation: %f | Pi serial estimation: %Lf\n",serial_time, pi_estimation_serial);
    
    // Now using OpenMP

    circle_hits = 0; // Setting the global variable to 0 before the threads execution takes place.
    thread_times = malloc(num_of_threads * sizeof(double));
    long double pi_estimation_parallel;
    double start_parallel;
    double stop_parallel;
    double parallel_time;

    GET_TIME(start_parallel)
    // Adding to circle hits is a critical segment, use reduction provided by OpenMP
    #pragma omp parallel for num_threads(num_of_threads) reduction(+:circle_hits)
        for(int i = 0; i < num_of_threads; i++){
            circle_hits += parallel_estimation(throw_count);
        }
    pi_estimation_parallel = 4 * circle_hits / ((double) throw_count);
    GET_TIME(stop_parallel)

    parallel_time = stop_parallel - start_parallel;
    
    // Average thread time calculation
    double avg_thread_time = 0;
    for (int i = 0; i < num_of_threads; i++)
        avg_thread_time += thread_times[i];
    avg_thread_time = avg_thread_time / num_of_threads;

    printf("Time for parallel estimation: %f | Pi parallel estimation: %Lf\n",parallel_time, pi_estimation_parallel);
    printf("Average thread time: %f\n", avg_thread_time);

    free(thread_times);
    return 0;
}
