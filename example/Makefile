CXXFLAGS=-O3 -Wall -std=c++98 -g -march=native
#STRIP=true
STRIP=strip

all:
	mpicxx main.cpp $(CXXFLAGS) -o NetworkTest -I..
	$(STRIP) NetworkTest
