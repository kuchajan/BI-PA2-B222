CXX = g++
CF = -Wall -pedantic -Wextra -I/usr/include/SDL2 -D_REENTRANT
CFD = $(CF) -g -fsanitize=address
LF = -lSDL2 -lSDL2_image -lncurses
LFD = $(LF) -fsanitize=address 
# pkg-config --cflags --libs sdl2

SRC = $(wildcard src/*.cpp)
TEMPLATE = src/CMatrix.hpp
OBJ = ${SRC:.cpp=.o}


all: ascii-art

ascii-art: $(OBJ)
	$(CXX) $(OBJ) $(LF) -o ascii-art

src/%.o: src/%.cpp $(TEMPLATE)
	$(CXX) $< $(CF) -c -o $@

clean:
	rm -f $(OBJ) ascii-art
	rm -rf doc/

doc: $(wildcard src/*.hpp)
	doxygen Doxyfile