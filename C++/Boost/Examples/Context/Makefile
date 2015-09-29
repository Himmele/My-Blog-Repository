CC        := gcc
CXX       := g++
LD        := g++
CFLAGS    := -O2
LDFLAGS   := 
LIBS	  := -L../boost_1_52_0/stage/lib -lboost_context

SRCS      := $(wildcard *.cpp)
OBJS      := $(patsubst %.cpp,%.o,$(SRCS))
INCLUDES  := -I../boost_1_52_0
BUILD_DIR := .

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: all clean

all: HelloWorld

HelloWorld: HelloWorld.o
	$(LD) $(LDFLAGS) $^ -o $@ $(LIBS)

clean:
	@rm -rf *.o HelloWorld
