
SRC = main.cpp Autoroute.cpp Ecosystem.cpp glad.cpp GraphicEnvironement.cpp Entitie.cpp Shader.cpp Camera.cpp
OBJ = $(SRC:.cpp=.o)

CFLAGS 		 = -O3 -Wall -W -pedantic -std=c++17
LIB = -framework openGL -lglfw

all: $(OBJ)
	g++ $(OBJ) -o a.out $(LIB)

%.o: %.cpp %.hpp
	g++ -o $@ -c $< $(CFLAGS)

%.o: %.cpp
	g++ -o $@ -c $< $(CFLAGS)

clean:
	rm $(OBJ)

cleanall:
	rm a.out $(OBJ)