
// License: see LICENSE.txt

#ifndef PoolHeader
#define PoolHeader

#include "Actor.h"
#include "Message.h"
#include "Channel.h"

#include <vector>
#include <iostream>
using namespace std;

class Pool {

private:
	vector<Actor*> actors;
	int nextActorIdentifier;

	int rank;
	int size;

	int aliveActors;

	Channel channel;

public:

	Pool();
	~Pool();

	int GetRank();
	int GetSize();

	int Spawn(Actor * actor);
	void Initialize(int * numberOfArguments, char *** argumentValues);
	void Start();
	void Finalize();
	void Dispatch(Message & message);

};

#endif
