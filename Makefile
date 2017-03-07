CF = -Wall --std=c++14
LF = -Wall --std=c++14  -lGL -lGLEW -lSDL2 -lSDL2_image -lassimp
CXX = g++
SRC = $(wildcard *.cpp)
OBJ = $(SRC: %.cpp=obj/%.o)
TRG = hello


all: $(TRG)


obj/%.o: %.cpp Makefile
	@mkdir -p obj/
	$(CXX) $(CF) $< -c -o $@


$(TRG): $(OBJ) Makefile
	$(CXX) $(OBJ) $(LF) -o $@


clean:
	rm -rf $(TRG) obj/
