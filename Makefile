CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -fopenmp
DEBUG_FLAGS = -std=c++17 -Wall -Wextra -g -O0 -fopenmp

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

main: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o ray -lglfw -lGL

debug: $(SRC)
	$(CXX) $(DEBUG_FLAGS) $(SRC) -o ray_debug -lglfw -lGL

clean:
	rm -f *.o ray ray_debug

