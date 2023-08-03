#Use the g++ compiler
CC = g++

# Compiler flags:
#   -Wall (most warnings enabled)
#   -g (optional, for debugging with gdb)
CFLAGS = -Wall

# Executable name:
TARGET = test

all: interpreter.o interpreter_driver.o $(TARGET)

$(TARGET): interpreter_driver.o interpreter.o
	$(CC) $(CFLAGS) interpreter_driver.o interpreter.o -o $(TARGET)

interpreter_driver.o: interpreter_driver.cpp interpreter.cpp interpreter.h
	$(CC) $(CFLAGS) interpreter_driver.cpp -c

interpreter.o: interpreter.cpp interpreter.h
	$(CC) $(CFLAGS) interpreter.cpp -c


clean:
	rm *.o *~ $(TARGET)
