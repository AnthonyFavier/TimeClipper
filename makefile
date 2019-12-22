CC=g++
SFMLFLAGS=-lsfml-graphics -lsfml-window -lsfml-system
WARN=-Wall
SRCS=src/*.cpp

all: game_c game_l

game_c:	$(SRCS)
	$(CC) -c $(SRCS) $(SFMLFLAGS)

game_l: *.o
	$(CC) -o timeClipper *.o $(SFMLFLAGS)

clean:
	rm -f *.o
