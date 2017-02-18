CC = g++
CC_FLAGS = -Wall -g3
LDFLAGS=-lncurses

EXEC = ./manuscrito.out
SOURCES = $(wildcard *.cpp)

# main target
$(EXEC) :
		$(CC) $(CC_FLAGS) $(SOURCES) -o $(EXEC) $(LDFLAGS)

clean:
		rm -rf $(EXEC) $(OBJECTS)

run: restart
		$(EXEC) test.txt

restart: clean $(EXEC)