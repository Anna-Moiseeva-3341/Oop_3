CXX = g++
CXXFLAGS = -std=c++17 -Wall -I.
TARGET = game

SRC_DIRS = . abilities exceptions mainElements
SRCS = $(wildcard *.cpp) \
       $(wildcard abilities/*.cpp) \
       $(wildcard exceptions/*.cpp) \
       $(wildcard mainElements/*.cpp)

OBJS = $(SRCS:.cpp=.o)
DEPS = $(wildcard *.h) \
       $(wildcard abilities/*.h) \
       $(wildcard exceptions/*.h) \
       $(wildcard mainElements/*.h)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)
%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	rm -f $(TARGET) $(wildcard *.o) \
		  $(wildcard abilities/*.o) \
		  $(wildcard exceptions/*.o) \
		  $(wildcard mainElements/*.o)

.PHONY: clean

list:
	@echo "Source files:" $(SRCS)
	@echo "Object files:" $(OBJS)
	@echo "Header files:" $(DEPS)

.PHONY: list