#Use the g++ compiler
CC = g++

# Compiler flags:
#   -Wall (most warnings enabled)
#   -g (optional, for debugging with gdb)
CFLAGS = -Wall

# Executable name:
TARGET = converter

all: converter.o converter_driver.o $(TARGET)

$(TARGET): converter_driver.o converter.o evaluate.o
	$(CC) $(CFLAGS) converter_driver.o converter.o evaluate.o -o $(TARGET)

converter_driver.o: converter_driver.cpp converter.cpp converter.h
	$(CC) $(CFLAGS) converter_driver.cpp -c

converter.o: converter.cpp converter.h
	$(CC) $(CFLAGS) converter.cpp -c

evaluate.o: evaluate.cpp evaluate.h
	$(CC) $(CFLAGS) evaluate.cpp -c

clean:
	rm *.o *~ $(TARGET)
