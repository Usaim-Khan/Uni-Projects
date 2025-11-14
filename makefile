CC = gcc
CFLAGS = -Wall

# Final program name
TARGET = main

# Object files
OBJS = main.o ui.o

# Default rule
all: $(TARGET)

# Linking rule
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compilation rules
main.o: main.c ui.h
	$(CC) $(CFLAGS) -c main.c

ui.o: ui.c ui.h
	$(CC) $(CFLAGS) -c ui.c

# Clean rule
clean:
	rm -f *.o $(TARGET)
