
// License: see LICENSE.txt

#ifndef NetworkTesterHeader
#define NetworkTesterHeader

#include <BioActors/Actor.h>

class NetworkTester : public Actor{

	Message dummyMessage;

	enum {
		BOOT, // needed to offset values by 1
		SAY_HELLO,
		DIE
	};
public:

	NetworkTester() {

	}

	void Receive(Message & message) {
		int tag = message.GetTag();

		switch(tag) {

			case Actor::BOOT: {
				Boot();
			}
			break;

			case SAY_HELLO: {
				SayHello();
			}
			break;

			case DIE: {

				cout << "[" << GetAddress() << "] ";
				cout << " Now I die." << endl;
				Die();
			}
			break;
		}
	}

	void SayHello() {
		cout << "[" << GetAddress() << "] ";
		cout << "Hello ! I am " << GetAddress() << endl;

		dummyMessage.SetTag(DIE);

		int address = GetAddress();

		// kill the actor
		Send(address, dummyMessage);

	}

	void Boot() {
		cout << "[" << GetAddress() << "] ";
		cout << "I am booting now ( " << GetAddress() << ") ";
		cout << " actors " << GetSize() << endl;

		int address = GetAddress();

		cout << "[" << GetAddress() << "] ";
		cout << "Sending message to " << address << endl;

		dummyMessage.SetTag(SAY_HELLO);

		Send(address, dummyMessage);

	}
};

#endif
