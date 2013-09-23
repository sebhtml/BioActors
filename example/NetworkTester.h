
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
	int finished;
	int controller;

	uint64_t timeBefore;

	uint64_t sum;

	enum {
		BOOT, // needed to offset values by 1
		SAY_HELLO,
		TEST_MESSAGE,
		TEST_MESSAGE_RESPONSE,
		DIE,
		ADVISE
	};
public:

	static int ACTORS_PER_RANK;

	NetworkTester() {

	}

	uint64_t GetMicroSeconds() {

		struct timeval tv;
		struct timezone tz;
		struct tm *tm;
		gettimeofday(&tv, &tz);
		tm=localtime(&tv.tv_sec);
		//printf(" %d:%02d:%02d %ld \n", tm->tm_hour, tm->tm_min,tm->tm_sec, tv.tv_usec);

		uint64_t value = 0;
		uint64_t hours = tm->tm_hour;
		value += (60 * 1000 * 1000 * hours);
		value += 60 * 1000000 * (uint64_t)tm->tm_min;
		value += 1000000 * (uint64_t)tm->tm_sec;
		value += tv.tv_usec;

		return value;
	}

	void Receive(Message & message) {
		int tag = message.GetTag();

		switch(tag) {

			case TEST_MESSAGE: {
				TestMessage(message);
			}
			break;

			case TEST_MESSAGE_RESPONSE: {
				TestMessageResponse(message);
			}
			break;

			case ADVISE: {
				Advise(message);
			}
			break;

			case Actor::BOOT: {
				Boot(message);
			}
			break;

			case SAY_HELLO: {
				SayHello(message);
			}
			break;

			case DIE: {

				cout << "[" << GetAddress() << "] ";
				cout << " Now I die." << endl;

				Die(message);
			}
			break;
		}
	}

	void Advise(Message & message) {

		finished ++;

		int total = GetSize() * ACTORS_PER_RANK;

		if(finished == total) {

			Message death;
			death.SetTag(DIE);

			cout << "[" << GetAddress() << "] killing everyone now." << endl;

			for(int i = 0; i < total; ++i) {

				Send(i, death);
			}
		}
	}

	void SayHello(Message & message) {

		int total = GetSize() * ACTORS_PER_RANK;

		cout << "[" << GetAddress() << "] ";
		cout << "Hello ! I am " << GetAddress();
		cout << " out of " << total << " actors" << endl;

		Message dummyMessage;
		dummyMessage.SetTag(TEST_MESSAGE_RESPONSE);

		int address = GetAddress();

		timeBefore = GetMicroSeconds();
		Send(address, dummyMessage);
	}

	void TestMessage(Message & message) {
		int address = message.GetSource();
		Message dummyMessage;
		dummyMessage.SetTag(TEST_MESSAGE_RESPONSE);
		timeBefore = GetMicroSeconds();
		Send(address, dummyMessage);
	}

	void TestMessageResponse(Message & message) {

		uint64_t timeAfter = GetMicroSeconds();

		int latency = (timeAfter - timeBefore) / 2;

		sum += latency;

		messages ++;

		int messagesToSend = 10000;

		if(messages == messagesToSend) {

			cout << "[" << GetAddress() << "] ";
			cout << " communicated " << messages << " ";
			cout << " TEST_MESSAGE messages";

			int average = sum / messages;

			cout << " point-to-point latency: ";
			cout << average << " us" << endl;

			// kill the actor
			Message dummyMessage;
			dummyMessage.SetTag(ADVISE);
			int address = controller;
			Send(address, dummyMessage);

		} else {
			// send a message to a random stranger
			int total = GetSize() * ACTORS_PER_RANK;
			int address = rand() % total;

			Message dummyMessage;
			dummyMessage.SetTag(TEST_MESSAGE);
			Send(address, dummyMessage);
		}

	}

	void Boot(Message & message) {

		sum = 0;
		messages = 0;
		finished = 0;
		controller = 0;

		cout << "[" << GetAddress() << "] ";
		cout << "I am booting now ( " << GetAddress() << ") ";
		cout << " ranks " << GetSize() << endl;

		int address = GetAddress();

		cout << "[" << GetAddress() << "] ";
		cout << "Sending message to " << address << endl;

		Message dummyMessage;
		dummyMessage.SetTag(SAY_HELLO);

		Send(address, dummyMessage);

	}
};

int NetworkTester::ACTORS_PER_RANK = 320;

#endif
