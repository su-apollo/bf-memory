CXX = g++
CXXFLAGS = -std=c++11
AR = ar

MAIN = main.cpp
SRCS = $(wildcard ../bf-memory/source/*.cpp)
OBJS = $(patsubst ../bf-memory/source/%.cpp, %.o, $(SRCS))
INC = -I../bf-memory/header

PROJECT = bf-memory
TARGET = test.out
LIB = bf-memory.a

all : $(LIB)
	$(CXX) $(CXXFLAGS) -pthread -o $(TARGET) $(MAIN) $(LIB) $(INC)

$(LIB) : $(PROJECT)
	$(AR) rcv $(LIB) $(OBJS)

$(PROJECT) :
	$(CXX) -c $(CXXFLAGS) $(SRCS) $(INC)

clean :
	rm -f $(TARGET)
	rm -f $(LIB)
	rm -f *.o