cachesim: cachesim.h cachesim.cpp
	g++ -std=c++11 cachesim.cpp -o cachesim -g

all: cachesim

clean:
	rm cachesim