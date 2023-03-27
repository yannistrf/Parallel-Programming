CC = gcc
INCLUDE = ./include
CFLAGS = -I$(INCLUDE) -lpthread
EXEC = pi_comp
OBJS = ./src/my_rand.o ./src/pi_comp.o
ARGS = 4 10000000

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

clean:
	rm -f $(EXEC) $(OBJS)
