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

.PHONY: all checkdirs clean

all: Echo Fibonacci

Fibonacci: Fibonacci.o
	$(LD) $(LDFLAGS) $^ -o $@ $(LIBS)

Echo: Echo.o
	$(LD) $(LDFLAGS) $^ -o $@ $(LIBS)

clean:
	@rm -rf *.o Fibonacci Echo
