# wrapper script
CC = mpicc
RUN = mpiexec
INCLUDE = ./include
BIN = ./bin
SRC = ./src

CFLAGS = -Wall -Werror -I$(INCLUDE)

OBJS1 = $(BIN)/pi_comp.o $(BIN)/my_rand.o
OBJS2 = $(BIN)/mpi_mat_vect.o

EXEC1 = $(BIN)/pi_comp
EXEC2 = $(BIN)/mpi_mat_vect
EXECS = $(EXEC1) $(EXEC2)

# arguments for pi program
ARGS1 = 1000000
# arguments for matrix program
ARGS2 = 4
# number of processes to create
PROCNUM = 4

all: $(EXECS)
pi: $(EXEC1)
mat: $(EXEC2)

# build pi executable
$(EXEC1): $(OBJS1)
	$(CC) $(OBJS1) -o $(EXEC1)

# build matrix executable
$(EXEC2): $(OBJS2)
	$(CC) $(OBJS2) -o $(EXEC2)

# rule to build every object file
$(BIN)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

runpi: $(EXEC1)
	$(RUN) -n $(PROCNUM) $(EXEC1) $(ARGS1)

runmat: $(EXEC2)
	$(RUN) -n $(PROCNUM) $(EXEC2) $(ARGS2)

clean:
	rm -f $(BIN)/*
