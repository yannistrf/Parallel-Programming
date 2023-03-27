#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdatomic.h>
#include "my_rand.h"
#include "timer.h"


// Global variables, visible to all threads
long long int throw_count;
int num_of_threads;
_Atomic long long int circle_hits = 0;
// Each thread will store its computation time inside this array
// When writing here is not a critical segment since each thread
// will modify a different element of the array
double* thread_times;

void* pi_thread_estimation(void* thread_id){
    double start;
    double finish;
    // Create a local variable for each thread
    // No need to write to global circle_hits each time
    // More efficient this way
    long long int local_circle_hits = 0;
    
    long tid = (long) thread_id;
    long local_throw_count = throw_count / num_of_threads;
    double x_rand;
    double y_rand;
    
    GET_TIME(start);
    for (long long i = 0; i < local_throw_count; i++) {
        unsigned int seed = time(NULL) + i + tid;
        x_rand = get_rand(&seed);
        y_rand = get_rand(&seed);
        double dist_square = x_rand * x_rand + y_rand * y_rand;

        if(dist_square <= 1)
            local_circle_hits++;
    }
    GET_TIME(finish);

    // Using atoimic operations to write in the critical segment.
    // All threads will modify this variable
    circle_hits += local_circle_hits;

    thread_times[tid] = finish - start;
}


int main(int argc, char** argv) {

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number_of_threads> <throw_count>\n", argv[0]);
        exit(-1);
    }

    num_of_threads = atoi(argv[1]);
    throw_count = atoi(argv[2]);

    // Serial algorithm for the estimation of pi.
    double start_serial;
    double stop_serial;
    long double pi_estimation_serial;

    double x_rand;
    double y_rand;
    GET_TIME(start_serial);
    for (long long int throw = 0; throw < throw_count; throw++) {
        unsigned int seed = time(NULL) + throw;
        x_rand = get_rand(&seed);
        y_rand = get_rand(&seed);
        double dist_square = x_rand * x_rand + y_rand * y_rand;

        if(dist_square <= 1)
            circle_hits++; 
    }

    pi_estimation_serial = 4 * circle_hits / ((double) throw_count);
    GET_TIME(stop_serial);

    double serial_time = stop_serial - start_serial;
    printf("Time for serial   estimation: %f | Pi serial estimation: %Lf\n",serial_time, pi_estimation_serial);


    // Now using pthreads

    circle_hits = 0;
    double start_parallel;
    double stop_parallel;
    double parallel_time;
    long double pi_estimation_parallel;
    thread_times = malloc(num_of_threads * sizeof(double));

    GET_TIME(start_parallel)
    pthread_t* threads = malloc(num_of_threads * sizeof(pthread_t));
    for (long i = 0; i < num_of_threads; i++) {
        
        if(pthread_create(&threads[i], NULL, pi_thread_estimation, (void*) i)){
            printf("Error! Function pthread_create failed.\n");
            exit(-1);
        }
    }
    // Collect threads
    for (int i = 0; i < num_of_threads; i++) {
        if (pthread_join(threads[i], NULL)) {
            fprintf(stderr, "Pthreads join operetion returned exit error.\n");
                exit(-1);
            
        }
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

    free(threads);
    free(thread_times);
    return 0;

}