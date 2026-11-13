CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-unused-parameter -O2
SRC = $(wildcard src/*.cpp)
HDR = $(wildcard src/*.hpp)

tournoi: $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) $(SRC) -o tournoi

clean:
	rm -f tournoi
