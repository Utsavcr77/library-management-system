CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude
SRC = src/main.cpp src/Book.cpp src/Member.cpp src/Library.cpp
TARGET = library_system

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
