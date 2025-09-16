CXX = g++
CC = gcc
CXXFLAGS = -Wall -std=c++23 -DUNICODE -c -m64 -g -O3
CCFLAGS = -Wall -std=c23 -DUNICODE -c -m64 -g -O3
INCLUDE = -I ./include
LIBRARY = -L ./library -lOpenGL32 -lOpenAL32 -lglfw3dll
TARDIR = ./build/
TARGET = ForK
SRC = glad.c ForK.cpp graphic.cpp audio.cpp program.cpp grade.cpp
OBJ = $(SRC:.cpp=.o)
OBJ := $(OBJ:.c=.o)
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $(TARDIR)$(TARGET) $(OBJ) $(LIBRARY)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@ $(INCLUDE)
clean:
	del /f /q *.o *.exe
clear:
	strip $(TARDIR)$(TARGET).exe
	del /f /q *.o
.PHONY: all clean