CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++11
LDFLAGS=

TARGET=aes
SRCS=main.cpp aes.cpp
OBJS=$(SRCS:.cpp=.o)

TEST_TARGET=test_aes
TEST_SRCS=test.cpp aes.cpp
TEST_OBJS=$(TEST_SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(LDFLAGS) -o $(TEST_TARGET) $(TEST_OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) $(TEST_TARGET) $(TEST_OBJS)
