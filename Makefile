CXX = g++
CXXFLAGS = -std=c++11
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGET = test
LIBS = -lbf-math
LIB_DIR = -L./bf-math
INC = -I./bf-math/include

all : $(TARGET)
	$(CXX) -o $(CXXFLAGS) $(OBJS) $(INC) $(LIB_DIR) $(LIBS)

$(TARGET) :
	$(CXX) -c $(CXXFLAGS) $(SRCS) $(INC) $(LIB_DIR) $(LIBS)

clean :
	rm -f $(TARGET)
	rm -f *.o