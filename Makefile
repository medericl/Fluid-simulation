CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -fopenmp -Isrc -Iui
DEBUG_FLAGS = -std=c++17 -Wall -Wextra -g -O0 -fopenmp -Isrc -Iui

SRC = $(wildcard src/*.cpp) $(wildcard ui/*.cpp)
OBJ = $(SRC:.cpp=.o)

main: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o ray -lglfw -lGL

debug: $(SRC)
	$(CXX) $(DEBUG_FLAGS) $(SRC) -o ray_debug -lglfw -lGL

clean:
	rm -f src/*.o ui/*.o ray ray_debug
