CXX = g++
CXXFLAGS = -Wall -g

OBJS = main.o library.o string_tools.o
TARGET = music_library

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS)

main.o: main.cpp library.h string_tools.h
	$(CXX) $(CXXFLAGS) -c main.cpp

library.o: library.cpp library.h
	$(CXX) $(CXXFLAGS) -c library.cpp

string_tools.o: string_tools.cpp string_tools.h
	$(CXX) $(CXXFLAGS) -c string_tools.cpp

clean:
	rm -f $(OBJS) $(TARGET)