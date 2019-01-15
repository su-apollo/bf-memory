CXX = g++
CXXFLAGS = -std=c++11
AR = ar
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGET = bf-math.a
INC = -I./include

all : $(TARGET)
	ar rcv $(TARGET) $(OBJS)

$(TARGET) :
	$(CXX) -c $(CXXFLAGS) $(SRCS) $(INC)

clean :
	rm -f $(TARGET)
	rm -f *.o