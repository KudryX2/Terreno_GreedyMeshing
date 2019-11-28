TARGET  = SENG
INCL    = -g -I/usr/include -I. 
LIBS    = -lGLU -lGL -lglut -lsfml-graphics -lsfml-window -lsfml-system
CC      = g++ 
OBJECTS = main.o Object3D.o Display.o Camera.o Cube.o Chunk.o World.o auxLib.hpp WorldProperties.hpp

$(TARGET) : $(OBJECTS)
	$(CC) -o $@ $(INCL) $(OBJECTS) $(LIBS)

clean:
	rm -f *.o
	rm -f $(TARGET)


