#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void Allocate_arrays(
                double** local_A, double** local_x, double** local_b,
                double** A, double** x, double** b, int local_n, int n,
                int myrank);
void Destroy_Arrays(
                double* local_A, double* local_x, double* local_b,
                double* A, double* x, double* b, int myrank);
void Generate_matrix(double* A, int n);
void Generate_vector(double* x, int n);
void Col_split_A(double* A, double* local_A, int n, int rank, int local_n);
void Print_matrix(double* A, int lines, int cols, int rank);
void Mat_vect_mult(
                double local_A[], double local_x[], 
                double local_y[], int n, int local_n);

int main(int argc, char* argv[]) {

    double* local_A;
    double* local_x;
    double* local_b;
    double* A;
    double* x;
    double* b;
    int n;
    int local_n;
    int my_rank;
    int comm_sz;
    MPI_Comm comm;
    double start;
    double finish;

    MPI_Init(&argc, &argv);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    n = atoi(argv[1]);
    local_n = n / comm_sz;
    Allocate_arrays(&local_A, &local_x, &local_b, &A, &x, &b, local_n, n, my_rank);

    // Process 0 assigns values to A and x, then splits A and sends
    // each process its part. It also splits x and sends each process
    // its part by calling MPI_Scatter.
    if (my_rank == 0) {
        srand(my_rank);
        Generate_matrix(A, n);
        Generate_vector(x, n);
        printf("\nComplete A matrix\n");
        Print_matrix(A, n, n, my_rank);

        start = MPI_Wtime();
        // Split A to other processes
        for (int rank = 1; rank < comm_sz; rank++) {
            Col_split_A(A, local_A, n, rank, local_n);
            MPI_Send(local_A, n*local_n, MPI_DOUBLE, rank, 0, comm);
        }
        // Process 0 also gets a part
        Col_split_A(A, local_A, n, 0, local_n);
        // Split x
        MPI_Scatter(x, local_n, MPI_DOUBLE, local_x, local_n, MPI_DOUBLE, 0, comm);
        
    }
    // Other processes wait to receive their part of A and x.
    else {
        MPI_Recv(local_A, n*local_n, MPI_DOUBLE, 0, 0, comm, MPI_STATUS_IGNORE);
        MPI_Scatter(x, local_n, MPI_DOUBLE, local_x, local_n, MPI_DOUBLE, 0, comm); 
    }

    printf("\nPart of A that got assigned\n");
    Print_matrix(local_A, n, local_n, my_rank);
    
    MPI_Barrier(comm);
    // Each process does its multiplication.
    Mat_vect_mult(local_A, local_x, local_b, n, local_n ); 
    // Collect and add all results and store them to b.
    MPI_Reduce(local_b, b, n, MPI_DOUBLE, MPI_SUM, 0, comm);

    if (my_rank == 0) {
        finish = MPI_Wtime();
        printf("\nFinal answer for b\n");
        Print_matrix(b, n, 1, 0);
        printf("Elapsed time = %f secs\n", finish-start);   
    }

    Destroy_Arrays(local_A, local_x, local_b, A, x, b, my_rank);
    MPI_Finalize();
}

// Allocates space for local arrays.
// If it's called from process 0, also allocates
// space for the whole matrices and vectors.
void Allocate_arrays(
                double** local_A,
                double** local_x,
                double** local_b,
                double** A,
                double** x,
                double** b,
                int local_n,
                int n,
                int myrank ) {

    *local_A = malloc(n*local_n*sizeof(double));
    *local_x = malloc(local_n*sizeof(double));
    *local_b = malloc(n*sizeof(double));

    if (myrank == 0) {
        *A = malloc(n*n*sizeof(double));
        *x = malloc(n*sizeof(double));
        *b = malloc(n*sizeof(double));
    }

}

// Free the space we allocated for the local arrays.
// If it's called from process 0, also free the space
// of the whole matrices and vectors.
void Destroy_Arrays(
                double* local_A,
                double* local_x,
                double* local_b,
                double* A,
                double* x,
                double* b,
                int myrank) {
    
    free(local_A);
    free(local_x);
    free(local_b);

    if (myrank == 0) {
        free(A);
        free(x);
        free(b);
    }
}

// Generates random values for a square matrix.
void Generate_matrix(double* A, int n) {
    
    for (int line = 0; line < n ; line++)
        for (int col = 0; col < n; col++)
            A[line*n + col] = ((double) random())/((double) RAND_MAX);

}

// Generates random values for a vector.
void Generate_vector(double* x, int n) {

    for (int i = 0; i < n; i++)
        x[i] = ((double) rand())/((double) RAND_MAX);

}

// Returns a part of the matrix A, some of its columns, depending
// on the destination process.
void Col_split_A(double* A, double* local_A, int n, int rank, int local_n) {
    
    int pos = 0;
    for (int i = 0; i < n; i++) {
        for (int j = rank*local_n; j < (rank+1)*local_n; j++) {
            local_A[pos] = A[j + i*n];
            pos++;
        }
    }
}

// Note: Don't expect the order of the print statements to be exact.
//       Also, could see some mixed statements. Remember all processes
//       share the same stdout.
void Print_matrix(double* A, int lines, int cols, int rank) {
    
    printf("Using stdout: rank = %d\n", rank);
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%f ", A[i*cols+j]);
        }
        printf("\n");
    }
}

// Multiplies a matrix and a vector.
void Mat_vect_mult(double *local_A , double *local_x , double *local_b,
             int n ,int local_n ) {

    for (int i = 0; i < n; i++) {
        local_b[i] = 0.0;
        for (int j = 0; j < local_n; j++){
            local_b[i] += local_A[i*local_n+j]*local_x[j];
        }
    }
}
