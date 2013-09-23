
// License: see LICENSE.txt

#include "NetworkTester.h"

#include "Starter.h"

NetworkTester::NetworkTester() {

}

NetworkTester::~NetworkTester() {

}

uint64_t NetworkTester::GetMicroSeconds() {

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

void NetworkTester::Receive(Message & message) {
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


		case START: {
			Start(message);
		}
		break;

		case SAY_HELLO: {
			SayHello(message);
		}
		break;

		case DIE: {

			PrintAddress();
			cout << "Now I die." << endl;

			Die(message);
		}
		break;
	}
}

void NetworkTester::SayHello(Message & message) {

	int total = GetSize() * ACTORS_PER_RANK;

	PrintAddress();
	cout << "Hello ! I am " << GetAddress();
	cout << " out of " << total << " NetworkTester actors" << endl;

	Message dummyMessage;
	dummyMessage.SetTag(TEST_MESSAGE_RESPONSE);

	int address = GetAddress();

	timeBefore = GetMicroSeconds();
	Send(address, dummyMessage);
}

void NetworkTester::TestMessage(Message & message) {
	int address = message.GetSource();
	Message dummyMessage;
	dummyMessage.SetTag(TEST_MESSAGE_RESPONSE);
	timeBefore = GetMicroSeconds();
	Send(address, dummyMessage);
}

void NetworkTester::TestMessageResponse(Message & message) {

	uint64_t timeAfter = GetMicroSeconds();

	int latency = (timeAfter - timeBefore) / 2;

	sum += latency;

	messages ++;

	int messagesToSend = 10000;

	if(messages == messagesToSend) {

		PrintAddress();
		cout << "communicated " << messages << " ";
		cout << " TEST_MESSAGE messages";

		int average = sum / messages;

		cout << " point-to-point latency: ";
		cout << average << " us" << endl;

		// kill the actor
		Message dummyMessage;
		dummyMessage.SetTag(Starter::ADVISE);
		int address = controller;
		Send(address, dummyMessage);

	} else {
		// send a message to a random stranger
		int total = GetSize() * ACTORS_PER_RANK;
		int address = rand() % total;

		// skip Starter actors
		address += GetSize() * 1;

		Message dummyMessage;
		dummyMessage.SetTag(TEST_MESSAGE);
		Send(address, dummyMessage);
	}

}

void NetworkTester::Start(Message & message) {

	sum = 0;
	messages = 0;
	controller = 0;

	PrintAddress();
	cout << "starting !" << endl;

	int address = GetAddress();

	PrintAddress();
	cout << "Sending SAY_HELLO message to " << address << endl;

	Message dummyMessage;
	dummyMessage.SetTag(SAY_HELLO);

	Send(address, dummyMessage);

}

int NetworkTester::ACTORS_PER_RANK = 320;

