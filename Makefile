LLDEPS=-lGLEW -lGLFW3
HLDEPS=-framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL
EXENAME=run
DEBUG=-g

all:
	g++ *.cpp lib/*.cpp $(DEBUG) -o $(EXENAME) $(LLDEPS) $(HLDEPS)
