#Makefile

MPICXX=mpicxx
STRIP=true
ECHO=echo
#STRIP=strip
Q=@

CXXFLAGS=-O3 -Wall -std=c++98 -g -march=native


BIOACTOR_OBJECTS=../BioActors/Actor.o ../BioActors/Message.o ../BioActors/Ring.o ../BioActors/Pool.o ../BioActors/Channel.o

EXAMPLE_OBJECTS=main.o NetworkTester.o Starter.o

TARGET=NetworkTest

# inference rule
%.o: %.cpp
	$(Q)$(ECHO) "  BUILD $@"
	$(Q)$(MPICXX) $(CXXFLAGS) $(CONFIG_FLAGS) -I.. -c $< -o $@

all: $(TARGET)
       
NetworkTest:	$(BIOACTOR_OBJECTS) $(EXAMPLE_OBJECTS)
	$(Q)$(ECHO) "  LINK $@"
	$(Q)$(MPICXX) $(CXXFLAGS) $(BIOACTOR_OBJECTS) $(EXAMPLE_OBJECTS) -o $@
	$(Q)$(STRIP) $@
clean:
	$(Q)$(RM) $(BIOACTOR_OBJECTS) $(EXAMPLE_OBJECTS) $(TARGET)
