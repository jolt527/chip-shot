LIBS = -lSDL2
CFLAGS = -Wall -DDEBUG

SRC = $(wildcard *.cpp)
EXECUTABLE = chip-shot

all: chip-shot

$(EXECUTABLE): $(SRC)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -rf $(EXECUTABLE)

