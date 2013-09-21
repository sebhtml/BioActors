
// License: see LICENSE.txt

#ifndef NetworkTestHeader
#define NetworkTestHeader

#include "NetworkTester.h"
#include <BioActors/Pool.h>

class NetworkTest : public Pool {

public:

	NetworkTest() {

	}

	void Run(int * argc, char *** argv) {

		Initialize(argc, argv);

		for(int i = 0 ; i < 32; ++i) {
			NetworkTester * actor = new NetworkTester();
			AddActor(actor);
		}

		Start();

		Finalize();
	}
};

#endif
