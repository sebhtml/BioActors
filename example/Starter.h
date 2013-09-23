
// License: see LICENSE.txt

#ifndef StarterHeader
#define StarterHeader

#include <BioActors/Actor.h>

#include "NetworkTester.h"

#include <stdlib.h>
#include <sys/time.h>
#include <stdint.h>

/**
 * This actor start other actors for the network test.
 */
class Starter : public Actor {

private:

	int finished;
	void Advise(Message & message);
	void Boot(Message & message);

public:

	enum {
		BOOT, // to skip 0
		DIE,
		ADVISE
	};

	Starter();
	~Starter();

	void Receive(Message & message);
};

#endif
