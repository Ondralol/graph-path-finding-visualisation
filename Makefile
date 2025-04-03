CC=g++
LD=$(CC)
CFLAGS =-std=c++20 -Wall -pedantic -g
SOURCE=src

SFML_INCLUDE = /usr/include/SFML #Change file path accordingly
SFML_LIB = /usr/lib/x86_64-linux-gnu #Change file path accordingly
SFML_LIBS = -lsfml-window -lsfml-graphics -lsfml-system

all: main doxygen

main: $(SOURCE)/main.o $(SOURCE)/graph.o $(SOURCE)/conversion.o $(SOURCE)/graphVisualisation.o
	$(LD) $(CFLAGS) -o $@ $^ -L$(SFML_LIB) $(SFML_LIBS) 

%.o: $(SOURCE)/%.cpp
	$(CC) $(CFLAGS) -I$(SFML_INCLUDE) -c -o $@ $<

doxygen:
	doxygen docs/Doxyfile


run:
	@./main $(word 2, $(MAKECMDGOALS)) $(word 3, $(MAKECMDGOALS) $(word 4, $MAKECMDGOALS))
 
clean:
	rm -rf src/*.o main docs/html docs/latex 
