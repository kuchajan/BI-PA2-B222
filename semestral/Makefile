CXX = g++
CF = -Wall -pedantic -Wextra -I/usr/include/SDL2 -D_REENTRANT
CFD = $(CF) -g -fsanitize=address
LF = -fsanitize=address -lSDL2 -lSDL2_image

# pkg-config --cflags --libs sdl2
SRC = $(wildcard src/*.cpp)
SRCHDR = $(wildcard src/*.hpp)
OBJ = ${SRC:.cpp=.o}


all: ascii-art

ascii-art: $(OBJ)
	$(CXX) $(OBJ) $(LF) -o ascii-art

src/%.o: src/%.cpp $(SRCHDR)
	$(CXX) $< $(CFD) -c -o $@

clean:
	rm -f $(OBJ) ascii-art