
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

		int actorsPerRank = 1;

		if(*argc == 2)
			actorsPerRank = atoi((*argv)[1]);

		NetworkTester::ACTORS_PER_RANK = actorsPerRank;

		NetworkTester * actor = new NetworkTester();
		Spawn(actor);

		//cout << "NetworkTest create actor " << address << endl;

		int rank = GetRank();
		srand(time(NULL) * rank);

		Start();

		Finalize();
	}
};

#endif
