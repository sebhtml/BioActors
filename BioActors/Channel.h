
// License: see LICENSE.txt

#ifndef ChannelHeader
#define ChannelHeader

#include "Message.h"
#include "Ring.h"

#include <mpi.h>
#include <iostream>
using namespace std;

class Channel {

private:
	MPI_Comm routingCommunicator;
	MPI_Comm messagingCommunicator;

	Ring ring;

	int rank;
	int size;

	int sourceActorOffset;
	int destinationActorOffset;
	int sourceRankOffset;
	int destinationRankOffset;
	int contentOffset;

	char * receivingBuffer;

	int probeOperations;
	int messagesSent;
	int messagesReceived;

	int routingMessagesSent;
	int routingMessagesReceived;

public:
	Channel();
	~Channel();
	bool Receive(Message & message);
	void Send(int & destination, Message & message);
	void Initialize(int * numberOfArguments, char *** argumentValues);
	void Finalize();
	void Print();
	int GetRank();
	int GetSize();
	int GetRankFromAddress(int & actorAddress);

};

#endif
