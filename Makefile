#flags for the compiler
CC = gcc
CFLAGS = -Wall -g

#source files
SRCS = rush.c builtins.c helpers.c parallel.c

#object files
OBJS = $(SRCS:.c=.o)

#executable name
TARGET = rush

#build the project
all: $(TARGET)

#link the object files to the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

#compile the source files into object files
rush.o: rush.c rush.h builtins.h helpers.h parallel.h
	$(CC) $(CFLAGS) -c rush.c -o rush.o

builtins.o: builtins.c builtins.h rush.h
	$(CC) $(CFLAGS) -c builtins.c -o builtins.o

helpers.o: helpers.c helpers.h rush.h
	$(CC) $(CFLAGS) -c helpers.c -o helpers.o

parallel.o: parallel.c parallel.h
	$(CC) $(CFLAGS) -c parallel.c -o parallel.o

#organize the files
clean:
	rm -f $(OBJS) $(TARGET)