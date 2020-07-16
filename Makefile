CXXFLAGS = -g -pedantic -Wall -Wextra -std=c++17
LDFLAGS = -g

SRCS_CPP = main.cpp matrix/Matrix.cpp matrix/AllMatrices.cpp annealing/SimulatedAnnealing.cpp annealing/CircularBuffer.cpp io/IOFunctions.cpp
OBJS = $(SRCS_CPP:.cpp=.o)

.PHONY: all
all: Train_SA

Train_SA: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

$(OBJS): matrix/Matrix.hpp matrix/AllMatrices.hpp annealing/SimulatedAnnealing.hpp annealing/CircularBuffer.hpp io/IOFunctions.hpp

.PHONY: clean 
clean:
	rm -f *.o matrix/*.o annealing/*.o io/*.o Train_SA core.* vgcore.* G_results E_results

run:
	Train_SA -G files/Gfile -E files/Efile -X files/Xfile -Y files/Yfile -g files/GR_trfile
