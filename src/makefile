CC=g++-7 
CXXFLAGS=-std=c++17 -g -O3 -I.
LIBS = -fopenmp
DEPS = cell.h pin.h net.h def.h 
OBJ = main.o cell.o pin.o net.o def.o 

%.o: %.cpp $(DEPS)
	    $(CC) -c -o $@ $< $(CXXFLAGS)

main: $(OBJ)
	    $(CC) $(CXXFLAGS) $(LIBS) -o $@ $^

clean:
	    rm -f $(binaries) *.o

