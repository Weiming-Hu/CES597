# helloWorld.cpp
# 
# Weiming Hu
# weiming@psu.edu
# 
# Written for PSU CSE 597-002, Fall 2018
# August 2018

CC=gcc
CXX=g++
CFLAGS=-c -g
CXXFLAGS=-c -g -std=c++11


SOURCES=helloWorld.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=helloWorld.out

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(OBJECTS) -o $@

.c.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm *o helloWorld.out
