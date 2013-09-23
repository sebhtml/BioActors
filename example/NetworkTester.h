
// License: see LICENSE.txt

#ifndef NetworkTesterHeader
#define NetworkTesterHeader

#include <BioActors/Actor.h>

#include <stdlib.h>
#include <sys/time.h>
#include <stdint.h>

class NetworkTester : public Actor{

private:

	int messages;
	int controller;

	uint64_t timeBefore;

	uint64_t sum;

public:

	enum {
		START, // needed to offset values by 1
		SAY_HELLO,
		TEST_MESSAGE,
		TEST_MESSAGE_RESPONSE,
		DIE,
		ADVISE
	};


	static int ACTORS_PER_RANK;

	NetworkTester();
	~NetworkTester();

	uint64_t GetMicroSeconds();
	void Receive(Message & message);
	void SayHello(Message & message);
	void TestMessage(Message & message);
	void TestMessageResponse(Message & message);
	void Start(Message & message);

};

#endif
