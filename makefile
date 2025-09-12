CXX = g++
CC = gcc
CXXFLAGS = -Wall -std=c++23 -DUNICODE -c -m64 -g
CCFLAGS = -Wall -std=c23 -DUNICODE -c -m64 -g
INCLUDE = -I ./include
LIBRARY = -L ./library -lOpenGL32 -lOpenAL32 -lglfw3dll
TARGET = ForK.exe

all: $(TARGET)

$(TARGET): ForK.o glad.o
	$(CXX) -o ./build/$(TARGET) ForK.o glad.o $(LIBRARY)

glad.o: glad.c
	$(CC) $(CCFLAGS) $(INCLUDE) -o glad.o glad.c

ForK.o: ForK.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o ForK.o ForK.cpp

test.o: __FK.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o test.o __FK.cpp

test: test.o glad.o
	$(CXX) -o ./build/test.exe test.o glad.o $(LIBRARY)

clean:
	del /f /q *.o *.exe

clear:
	strip ./build/$(TARGET)
	del /f /q *.o

.PHONY: all clean