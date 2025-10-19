CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

TARGET = orderbook
SOURCES = main.cpp order_book.cpp
OBJECTS = $(SOURCES:.cpp=.o)


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp order_book.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

