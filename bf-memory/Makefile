CXX = g++
CXXFLAGS = -std=c++11
AR = ar
SRCS = $(wildcard source/*.cpp)
OBJS = $(patsubst source/%.cpp, %.o, $(SRCS))
TARGET = bf-memory.a
INC = -I./header

all : $(TARGET)
	$(AR) rcv $(TARGET) $(OBJS)

$(TARGET) :
	$(CXX) -c $(CXXFLAGS) $(SRCS) $(INC)

clean :
	rm -f $(TARGET)
	rm -f *.o