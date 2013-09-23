
// License: see LICENSE.txt

#ifndef NetworkTestHeader
#define NetworkTestHeader

#include "NetworkTester.h"
#include <BioActors/Pool.h>

#include <stdlib.h>
#include <time.h>

class NetworkTest : public Pool {

public:

	NetworkTest() {

	}

	void Run(int * argc, char *** argv) {

		Initialize(argc, argv);

		NetworkTester::ACTORS_PER_RANK = 256;

		int rank = GetRank();

		srand(time(NULL) * rank);

		for(int i = 0 ; i < NetworkTester::ACTORS_PER_RANK; ++i) {
			NetworkTester * actor = new NetworkTester();
			AddActor(actor);
		}

		Start();

		Finalize();
	}
};

#endif
