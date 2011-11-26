GCC = g++
OPS = -w
LINK_OPS = -lglut -lGL -lGLU -lX11  -lm -L/usr/X11R6/lib
OBJS = main.o libtrimesh.a
TEST_OBJS = 

all: $(OBJS) Makefile
	$(GCC) $(OPS) $(LINK_OPS) $(OBJS) -o tronRace

main.o : main.cpp
	$(GCC) $(OPS) -c main.cpp

clean:
	rm -rf tronRace test .\#* \#* *.o .*~ *~ *.gch semantic.cache
