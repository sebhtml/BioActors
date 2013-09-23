// License: see LICENSE.txt

#include "Actor.h"
#include "Pool.h"

#include <iostream>
using namespace std;

void Actor::Send(int & destination, Message & message) {

	message.SetSource(address);
	message.SetDestination(destination);

	channel->Send(destination, message);
}


Actor::Actor() {

}

Actor::~Actor() {

}

void Actor::SetAddress(int newAddress, int newSize, Channel * newChannel,
		Pool * newPool) {

	//cout << "SetAddress .... " << newAddress << endl;

	address = newAddress;
	size = newSize;
	channel = newChannel;
	pool = newPool;

	dead = false;
}

int Actor::GetAddress() {
	return address;
}

int Actor::GetSize() {
	return size;
}

void Actor::Die(Message & message) {
	dead = true;
}

bool Actor::IsDead() {
	return dead;

}

int Actor::Spawn(Actor * actor) {

	return pool->Spawn(actor);
}

void Actor::PrintAddress() {
	cout << "[actor#" << GetAddress() << "] ";
}
