# C compiler
CC = gcc
# include files
INCLUDE = ./include
# source code
SRC = ./src
# binary files
BIN = ./bin
# compiler flags
CFLAGS = -I$(INCLUDE) -fopenmp
# linker flags
LDFLAGS = -fopenmp
# pi_comp object files
OBJS1 = $(BIN)/my_rand.o $(BIN)/pi_comp.o
# omp_mat_vect_rand_split object files
OBJS2 = $(BIN)/omp_upper_mat_vect_rand_split.o

EXEC1 = $(BIN)/pi_comp
EXEC2 = $(BIN)/omp_upper_mat_vect_rand_split
EXECS = $(EXEC1) $(EXEC2)

# build both executables
all: $(EXECS)

# build pi_comp executable
pi: $(EXEC1)

# build omp_mat_vect_rand_split executable
mat: $(EXEC2)

# build pi_comp, link the object files
$(EXEC1): $(OBJS1)
	$(CC) $(LDFLAGS) $(OBJS1) -o $(EXEC1)

# build omp_mat_vect_rand_split, link the object files
$(EXEC2): $(OBJS2)
	$(CC) $(LDFLAGS) $(OBJS2) -o $(EXEC2)

# rule to compile all source files
$(BIN)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

# clean all binaries
clean:
	rm -f $(OBJS1) $(OBJS2) $(EXECS)