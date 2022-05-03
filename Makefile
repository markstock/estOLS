CXX=g++
INC=-I/usr/include/eigen3
#CXXFLAGS=-g -ggdb
CXXFLAGS=-Ofast -march=native

all : estOLS

% : %.cpp Makefile
	${CXX} ${CXXFLAGS} -o $@ ${INC} $<
