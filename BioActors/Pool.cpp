
// License: see LICENSE.txt

#include "Pool.h"

Pool::Pool() {
}

Pool::~Pool() {

}

int Pool::GetRank() {
	return rank;
}

int Pool::GetSize() {
	return size;
}

//template <class ActorType>
int Pool::Spawn(Actor * actor) {

	actors[nextActorIdentifier] = actor;

	int address = rank + nextActorIdentifier * size;

	actor->SetAddress(address, size, &channel, this);

	aliveActors ++;
	nextActorIdentifier++;

	return address;
}

void Pool::Initialize(int * numberOfArguments, char *** argumentValues) {

	channel.Initialize(numberOfArguments, argumentValues);


	rank = channel.GetRank();
	size = channel.GetSize();

	nextActorIdentifier = 0;

	int maximumNumberOfActors = 131072;
	actors.reserve(maximumNumberOfActors);

	aliveActors = 0;
}

void Pool::Start() {

	Message message;

	//cout << "Pool: booting actors" << endl;

	// first, boot every actor by sending the message BOOT
	int bootTag = Actor::BOOT;

	int actorsToBoot = nextActorIdentifier;

	for(int i = 0 ; i < actorsToBoot ; ++i) {

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

void Pool::Finalize() {

	channel.Finalize();
}

void Pool::Dispatch(Message & message) {

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




