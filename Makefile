
TARGET = edit
DEBUG = 
FLAGS = -Wall $(DEBUG)
OBJECTS = object/

all: $(TARGET)

$(TARGET): main.o editor.o buffer.o converter.o evaluate.o
	g++ -lncurses $(OBJECTS)main.o $(OBJECTS)editor.o $(OBJECTS)buffer.o $(OBJECTS)converter.o $(OBJECTS)evaluate.o -o $(TARGET)

# Main Editor
main.o: main.cpp editor.h buffer.h
	g++ $(FLAGS) -c main.cpp -o $(OBJECTS)main.o

editor.o: editor.cpp editor.h buffer.h
	g++ $(FLAGS) -c editor.cpp -o $(OBJECTS)editor.o

buffer.o: buffer.cpp buffer.h
	g++ $(FLAGS) -c buffer.cpp -o $(OBJECTS)buffer.o


# Calculator Classes
converter.o: calculator/converter.cpp calculator/converter.h
	g++ $(FLAGS) -c calculator/converter.cpp -o $(OBJECTS)converter.o

evaluate.o: calculator/evaluate.cpp calculator/evaluate.h
	g++ $(FLAGS) -c calculator/evaluate.cpp -o $(OBJECTS)evaluate.o


# Clean
clean:
	rm $(TARGET) *~ $(OBJECTS)*.o
