LLDEPS=-lGLEW -lGLFW3
HLDEPS=-framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL
EXENAME=run
FLAGS=-std=c++11
DEBUG=-g

all:
	g++ *.cpp lib/*.cpp $(FLAGS) $(DEBUG) -o $(EXENAME) $(LLDEPS) $(HLDEPS)
