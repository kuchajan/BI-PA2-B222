CXX = g++
CF = -Wall -pedantic -Wextra -D_REENTRANT -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600 -I/usr/include/SDL2
CFD = $(CF) -g -fsanitize=address
LF = -lSDL2 -lSDL2_image -lncurses -ltinfo
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
	rm -rf examples/test/output/
	rm -f examples/art_*.txt

doc: $(wildcard src/*.hpp)
	doxygen Doxyfile

TESTSCALE=assets/testscale.bmp
LENA=assets/Lena.tiff
SATURN=assets/edu_ring_a-round_the_saturn.jpg
BIRD=assets/Redwingblackbird1.jpg
DRAWING=assets/drawing.png

test: ascii-art
	./ascii-art -i $(TESTSCALE) -i $(TESTSCALE) -i $(TESTSCALE) -i $(TESTSCALE) -i $(TESTSCALE) -i $(TESTSCALE) -i $(TESTSCALE) -i $(TESTSCALE) -i $(TESTSCALE) -i $(TESTSCALE) -i $(TESTSCALE) -i $(TESTSCALE) -i $(TESTSCALE) -i $(TESTSCALE) -i $(TESTSCALE) -i $(TESTSCALE) -i $(TESTSCALE) -i $(TESTSCALE) -cs 0123456789ABCDEF 0,3,5,6,7,8,9,10,11,12,13,14,15,16,17 -cs 0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF 1 -fs 32 32 3 -fcr 3 3 8 8 4 -fb -255 5 -fb 255 6 -fb 0 7 -fct -255 8 -fct 255 9 -fct 0 10 -frr 11,16 -frl 12,14 -fmh 13 -fmv 14 -ffh 15 -ffv 16 -fi 17 -os 2 examples/test/output/
	cd 
	./examples/test/test.sh

example1: ascii-art
	./ascii-art -i $(LENA) -i $(SATURN) -i $(BIRD) -i $(DRAWING) -fs 100 50 0,1,2 -fs 50 50 3 -fb -25 0,2 -fct 50 0 -fb 110 3 -fi 1,3 -cs "@B%8&WM*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|1?-_+~<>i!lI;:,\"^'. " 1 -os 1

example2: ascii-art
	./ascii-art -i $(LENA) -i $(SATURN) -i $(BIRD) -i $(DRAWING) -fs 100 50 0,1,2 -fs 50 50 3 -fb -25 2 -fct 50 0,1 -fb 110 3 -fi 3 -os 2 examples/art_

example3: ascii-art
	./ascii-art -i $(LENA) -i $(SATURN) -i $(BIRD) -i $(DRAWING) -fs 100 50 0,1,2 -fs 50 50 3 -fb -25 2 -fct 50 0,1 -fb 110 3 -fi 3 -os 3
