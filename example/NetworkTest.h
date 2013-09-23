
// License: see LICENSE.txt

#ifndef NetworkTestHeader
#define NetworkTestHeader

#include <BioActors/Pool.h>

#include "Starter.h"

#include <stdlib.h>
#include <time.h>

class NetworkTest : public Pool {

public:

	NetworkTest() {

	}

	void Run(int * argc, char *** argv) {

		Initialize(argc, argv);

		Actor * actor = new Starter();
		Spawn(actor);

		Start();

		Finalize();
	}
};

#endif
