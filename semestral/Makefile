CXX = g++
CF = -Wall -pedantic -Wextra -I/usr/include/SDL2 -D_REENTRANT
CFD = $(CF) -g -fsanitize=address
LF = -lSDL2 -lSDL2_image
LFD = $(LF) -fsanitize=address 

# pkg-config --cflags --libs sdl2
SRC = $(wildcard src/*.cpp)
SRCHDR = $(wildcard src/*.hpp)
OBJ = ${SRC:.cpp=.o}


all: ascii-art

ascii-art: $(OBJ)
	$(CXX) $(OBJ) $(LFD) -o ascii-art

src/%.o: src/%.cpp $(SRCHDR)
	$(CXX) $< $(CFD) -c -o $@

clean:
	rm -f $(OBJ) ascii-art