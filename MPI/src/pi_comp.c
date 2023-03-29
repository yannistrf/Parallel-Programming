#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include "my_rand.h"

int main(int argc, char** argv){
    
    long long int throw_count, local_throw_count, circle_hits, local_circle_hits = 0;
    double start, finish, MPI_time, serial_time, local_MPI_time = 0;
    long double pi_serial, pi_MPI;
    int my_rank, num_of_proc;
    
    // Initialise MPI, get neccessary info
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // For illustrative purposes we get throw count
    // from stdin so we can use MPI_Bcast later
    // We could simply get it as a command line argument
    /*
    if (my_rank == 0) {
        printf("Enter number of throws: ");
        scanf("%lld", &throw_count);
    }
    MPI_Bcast(&throw_count, 1, MPI_LONG, 0, MPI_COMM_WORLD);
    */
    
    throw_count = atoi(argv[1]);
    
    // Process one will execute the serial algorithm
    if (my_rank == 0) {
        start = MPI_Wtime();
        for(long long i = 0; i < throw_count; i++) {
            unsigned int seed = time(NULL) + i + my_rank;
            double x_rand = get_rand(&seed);
            double y_rand = get_rand(&seed);
            double dist_square = x_rand * x_rand + y_rand * y_rand;
            
            if (dist_square <= 1) {
                circle_hits++;
            }
    }
        finish = MPI_Wtime();
        serial_time = finish - start;
        pi_serial = 4 * circle_hits / ((double) throw_count);
        printf("Time for serial estimation: %f | Pi serial estimation: %Lf\n", serial_time, pi_serial);

    }

    start = 0;
    finish = 0;
    circle_hits = 0;
    local_throw_count = throw_count / num_of_proc;
   
    // Wait for all process to arrive here
    MPI_Barrier(MPI_COMM_WORLD);
    
    start = MPI_Wtime();
    for (long long i = 0; i < local_throw_count; i++) {
        unsigned int seed = time(NULL) + i + my_rank;
        double x_rand = get_rand(&seed);
        double y_rand = get_rand(&seed);
        double dist_square = x_rand * x_rand + y_rand * y_rand;
        
        if (dist_square <= 1) {
            local_circle_hits++;
        }
    }

    // Process 0 collects results from other processes and add them together
    MPI_Reduce(&local_circle_hits, &circle_hits, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    finish = MPI_Wtime();
    local_MPI_time = finish - start;

    if (my_rank == 0) {
        // Total time is process 0 local time since it's the one collecting the results
        MPI_time = local_MPI_time;
        pi_MPI = 4 * circle_hits / ((double) throw_count);
        printf("Time for parallel estimation: %f | Pi parallel estimation: %Lf\n", MPI_time, pi_MPI);
    }

    MPI_Finalize();
    return 0;
}
