CXX=g++
INC=-I/usr/include/eigen3
CXXFLAGS=-g -ggdb

all : estOLS

% : %.cpp Makefile
	${CXX} ${CXXFLAGS} -o $@ ${INC} $<
