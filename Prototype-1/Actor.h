#ifndef ActorHeader
#define ActorHeader

//typedef int Address;

#include "Channel.h"
#include "Message.h"

class Actor {

private:
	//Address address;
	int address;
	int size;
	Channel * channel;
	bool dead;

protected:

	void Send(int & destination, Message & message) {

		message.SetSource(address);
		message.SetDestination(destination);

		channel->Send(destination, message);
	}

public:

	enum {
		BOOT
	};

	virtual void Receive(Message & message) = 0;

	// TODO: use a constructor
	void SetAddress(int newAddress, int newSize, Channel * newChannel) {

		//cout << "SetAddress .... " << newAddress << endl;

		address = newAddress;
		size = newSize;
		channel = newChannel;

		dead = false;
	}

	int GetAddress() {
		return address;
	}

	int GetSize() {
		return size;
	}

	void Die() {
		dead = true;
	}

	bool IsDead() {
		return dead;

	}
};

#endif
