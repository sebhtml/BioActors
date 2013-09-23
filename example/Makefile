CXXFLAGS=-O3 -Wall -std=c++98 -g -march=native
STRIP=true
#STRIP=strip
BIOACTOR_OBJECTS=../BioActors/Actor.cpp ../BioActors/Message.cpp

all:
	mpicxx main.cpp  $(CXXFLAGS) $(BIOACTOR_OBJECTS) -o NetworkTest -I..
	$(STRIP) NetworkTest
