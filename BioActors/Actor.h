// License: see LICENSE.txt

#ifndef ActorHeader
#define ActorHeader

//typedef int Address;

#include "Channel.h"
#include "Message.h"

class Pool;

/**
 *
 * \see Gul Agha (1986)
 * Actors: a model of concurrent computation in distributed systems
 * http://dl.acm.org/citation.cfm?id=7929
 *
 * When receiving a message, an actor can:
 *
 * 1. Send messages to other actors (or itself)
 *
 * 2. Create new actors
 *
 * 3. Change its behavior (here this is implemented with the internal
 * state of the objects of a specialized class.
 **/
class Actor {

private:
	//Address address;
	int address;
	int size;
	Channel * channel;
	Pool * pool;
	bool dead;

protected:

	/*
	 * Send a message
	 **/
	void Send(int & destination, Message & message);

	/**
	 * Spawn a new actor
	 **/
	int Spawn(Actor * actor);

	/**
	 * Die of a beautiful death.
	 */
	void Die(Message & message);

public:

	enum {
		BOOT = 99999999
	};

	Actor();
	virtual ~Actor();

	virtual void Receive(Message & message) = 0;

	void SetAddress(int newAddress, int newSize, Channel * newChannel,
			Pool * pool);

	int GetSize();

	bool IsDead();
	int GetAddress();

	void PrintAddress();
};

#endif
