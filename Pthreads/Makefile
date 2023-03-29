CC = gcc
INCLUDE = ./include
SRC = ./src
BIN = ./bin
# CFLAGS = -I$(INCLUDE) -lpthread -pthread
CFLAGS = -I$(INCLUDE) -lpthread
LDFLAGS = -pthread
EXEC = $(BIN)/pi_comp
OBJS = $(BIN)/my_rand.o $(BIN)/pi_comp.o
ARGS = 4 10000000

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

# rule to compile all source files
$(BIN)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f $(EXEC) $(OBJS)
