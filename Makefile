.PHONY:clean
CC=gcc
CFLAGS=-Wall -g
BIN=qshell
OBJS=main.o
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)
