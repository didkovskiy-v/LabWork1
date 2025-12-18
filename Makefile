CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

SRC = main.cpp RasterImage.cpp BmpMeta.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = image_processor

TEST_SRC = test.cpp RasterImage.cpp BmpMeta.cpp
TEST_BIN = image_processor_tests

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

run: all
	./$(TARGET)

test: $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -o $(TEST_BIN) $(TEST_SRC) -lgtest -lgtest_main -pthread
	./$(TEST_BIN)

clean:
	rm -f $(OBJ) $(TARGET) $(TEST_BIN)

.PHONY: all run clean test
