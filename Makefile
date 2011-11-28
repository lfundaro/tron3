GCC = g++
OPS = -fopenmp
LINK_OPS = -lglut -lGL -lGLU -lX11  -lm -L/usr/X11R6/lib
OBJS = main.o Parser.o Camara.o ModeloDatos.o Elementos.o libtrimesh.a \
tinyxml.a
TEST_OBJS = 

all: $(OBJS) Makefile
	$(GCC) $(OPS) $(LINK_OPS) $(OBJS) -o tronRace
	rm -r *.o

main.o : main.cpp
	$(GCC) $(OPS) -c main.cpp

Parser.o: Parser.cpp
	$(GCC) $(OPS) -c Parser.cpp

ModeloDatos.o: ModeloDatos.cpp
	$(GCC) $(OPS) -c ModeloDatos.cpp

Elementos.o: Elementos.cpp
	$(GCC) $(OPS) -c Elementos.cpp

Camara.o: Camara.cpp
	$(GCC) $(OPS) -c Camara.cpp

clean:
	rm -rf tronRace test .\#* \#* *.o .*~ *~ *.gch semantic.cache
