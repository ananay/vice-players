CC=g++
CFLAGS=-c -Wall -D_SERVER
LDFLAGS=-ldl -lpthread
SOURCES=$(wildcard *.cpp)
SOURCES+=$(wildcard ../squirrel/*.cpp)
SOURCES+=$(wildcard ../RakNet/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=../bin/vcp-svr

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -Rf *.o $(EXECUTABLE) ../RakNet/*.o ../squirrel/*.o

