
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

	Pool() {
	}

	//template <class ActorType>
	void AddActor(Actor * actor) {

		//int localIdentifier = nextActorIdentifier;

		actors[nextActorIdentifier] = actor;

		//cout << "Adding actor on rank " << rank << endl;
	
		int address = rank + nextActorIdentifier * size;

		//cout << "AddActor address= " << address << endl;

		actor->SetAddress(address, size, &channel);

		aliveActors ++;
		nextActorIdentifier++;
	}

	void Initialize(int * numberOfArguments, char *** argumentValues) {

		channel.Initialize(numberOfArguments, argumentValues);
		
		rank = channel.GetRank();
		size = channel.GetSize();

		//cout << "Pool -> rank " << rank << " size " << size << endl;

		nextActorIdentifier = 0;

		int maximumNumberOfActors = 131072;
		actors.reserve(maximumNumberOfActors);

		aliveActors = 0;
	}

	void Start() {

		Message message;

		//cout << "Pool: booting actors" << endl;

		// first, boot every actor by sending the message BOOT
		int bootTag = Actor::BOOT;

		for(int i = 0 ; i < nextActorIdentifier ; ++i) {

			Actor * actor = actors[i];

			if(actor == NULL)
				continue;

			message.SetTag(bootTag);
			message.SetSource(actor->GetAddress());
			message.SetDestination(actor->GetAddress());

			// we don't include BOOT messages in metrics
			actor->Receive(message);
		}

		int tick = 0;

		//cout << "Pool: entering loop" << endl;

		// do the main loop now.
		while(aliveActors > 0) {

			//cout << "Tick ! " << ticks << endl;

			tick++;

			/*
			if(ticks == 0)
				break;
				*/

			if(channel.Receive(message)) {

				Dispatch(message);
			}

		}

	}

	void Finalize() {

		channel.Finalize();
	}

	void Dispatch(Message & message) {

		int destinationActor = message.GetDestination();

		/*
		cout << "Received message: " ;
		cout << " tag: " << message.GetTag();
		cout << " destinationActor: " << destinationActor;
		cout << " rank " << rank << endl;
		*/

		int localIdentifier = destinationActor / size;

		/*
		int localIdentifier = destinationActor;
	       
		if(rank > 0)
			localIdentifier = destinationActor % rank;
		*/

		/*
		cout << "Dispatching to address " << destinationActor;
		cout << " on rank " << rank;
		cout << " trying local actor # " << localIdentifier;
		cout << endl;
		*/

		if(localIdentifier < nextActorIdentifier) {
			Actor * actor = actors[localIdentifier];

			if(actor == NULL)
				return;

			actor->Receive(message);

			if(actor->IsDead()) {

				// TODO How can I delete this actor since I don't know
				// its specialized type ?
				delete actor;

				actors[localIdentifier] = NULL;

				aliveActors--;
			}
		}

	}

};

#endif
