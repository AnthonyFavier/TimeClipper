CC=g++
SFMLFLAGS=-lsfml-graphics -lsfml-window -lsfml-system
WARN=-Wall
SRCS=src/*.cpp

all: 
	make game_c
	make game_l 
	make clean

game_c:	$(SRCS)
	$(CC) -c $(WARN) $(SRCS) $(SFMLFLAGS)

game_l: *.o
	$(CC) -o timeClipper *.o $(SFMLFLAGS)

clean:
	rm -f *.o
