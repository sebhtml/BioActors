

// License: see LICENSE.txt

#include "Starter.h"
#include "NetworkTester.h"

Starter::Starter() {
}

Starter::~Starter() {

}

void Starter::Receive(Message & message) {
	int tag = message.GetTag();

	if(tag == Actor::BOOT) {

		Boot(message);

	} else if(tag == DIE) {

		PrintAddress();
		cout << "Now I must die..." << endl;

		Die(message);
	} else if(tag == ADVISE) {
		Advise(message);
	}
}

void Starter::Boot(Message & message) {

	PrintAddress();
	cout << "booting..." << endl;

	srand(time(NULL) * GetAddress());
	finished = 0;

	NetworkTester::ACTORS_PER_RANK = 4;

	int count = NetworkTester::ACTORS_PER_RANK;

	for(int i = 0 ; i < count; ++i) {

		Actor * actor = new NetworkTester();

		Spawn(actor);

		PrintAddress();
		cout << "spawns actor " << actor->GetAddress() << endl;

		int address = actor->GetAddress();

		Message startMessage;
		startMessage.SetTag(NetworkTester::START);
		Send(address, startMessage);

	}

}

void Starter::Advise(Message & message) {

	finished ++;

	int total = GetSize() * NetworkTester::ACTORS_PER_RANK;

	PrintAddress();
	cout << "finished: " << finished << endl;

	if(finished == total) {

		Message death;

		PrintAddress();
		cout << "killing Starter actors" << endl;

		// also kill Starter actors too.
		for(int i = 0 ; i < GetSize() ; ++i) {
			int address = i;
			death.SetTag(Starter::DIE);
			Send(address, death);
		}

		PrintAddress();
		cout << "killing NetworkTester actors" << endl;

		// kill other actors
		for(int i = 0; i < total; ++i) {

			int address = i + GetSize();
			death.SetTag(NetworkTester::DIE);
			Send(address, death);
		}
	}
}




