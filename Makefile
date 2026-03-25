CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

SRCS = main.cpp Item.cpp Compare.cpp ItemGenerator.cpp
TARGET = project2

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
