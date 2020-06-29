CXXFLAGS = -g -pedantic -Wall -Wextra -std=c++17
LDFLAGS = -g

SRCS_CPP = main.cpp matrix/Matrix.cpp matrix/AllMatrices.cpp matrix/Stack.cpp annealing/SimulatedAnnealing.cpp annealing/CircularBuffer.cpp 
OBJS = $(SRCS_CPP:.cpp=.o)

.PHONY: all
all: a.out

a.out: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

$(OBJS): matrix/Matrix.hpp matrix/AllMatrices.hpp matrix/Stack.hpp annealing/SimulatedAnnealing.hpp annealing/CircularBuffer.hpp

.PHONY: clean 
clean:
	rm -f *.o matrix/*.o annealing/*.o a.out core.* vgcore.*
