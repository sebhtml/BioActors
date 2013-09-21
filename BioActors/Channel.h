
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
	Channel() {

		messagesSent = 0;
		messagesReceived = 0;
		routingMessagesSent = 0;
		routingMessagesReceived = 0;
		probeOperations = 0;

		int items = 0;
		sourceActorOffset = 0 + items++ * sizeof(int);
		destinationActorOffset = 0 + items++ * sizeof(int);
		sourceRankOffset = 0 + items++ * sizeof(int);
		destinationRankOffset = 0 + items++ * sizeof(int);
		contentOffset = 0 + items++ * sizeof(int);
	}

	bool Receive(Message & message) {

		// do a bunch of MPI_Test
		ring.TestBuffers();

		int source = MPI_ANY_SOURCE;
		int tag = MPI_ANY_TAG;

		MPI_Status status;
		int flag = 0;

		MPI_Iprobe(source, tag, messagingCommunicator, &flag, 
				               &status);

		probeOperations ++;

		if(!flag)
			return false;

		source = status.MPI_SOURCE;
		tag = status.MPI_TAG;

		message.SetTag(tag);

		MPI_Datatype datatype = MPI_BYTE;

		int count = 0;
		MPI_Get_count(&status, datatype, &count);

		MPI_Recv(receivingBuffer, count, datatype, source, tag,
				messagingCommunicator, &status);

		int sourceActor = -1;
		int destinationActor = -1;

		memcpy(&sourceActor, receivingBuffer + sourceActorOffset, sizeof(int));
		memcpy(&destinationActor, receivingBuffer + destinationActorOffset, sizeof(int));

		char * content = message.GetContent();
		count -= contentOffset;

		memcpy(content, receivingBuffer + contentOffset, count * sizeof(char));

		message.SetContentSize(count);
		message.SetSource(sourceActor);
		message.SetDestination(destinationActor);

		/*
		cout << "Channel got a message with tag ";
		cout << tag << ", ";
		cout << message.GetSource() << " -> " << source;
		cout << " " << message.GetDestination();
		cout << " -> " << rank << endl;
		*/

		messagesReceived++;
		return true;
	}

	void Send(int & destination, Message & message) {

		int tag = message.GetTag();

		/*
		cout << "Channel is sending message with tag";
		cout << " " << tag << " from ";
		cout << message.GetSource();
		cout << " to " << message.GetDestination() << endl;
		*/

		int address = message.GetSource();
		//int sourceRank = GetRankFromAddress(address);
		int destinationRank = GetRankFromAddress(destination);

		/*
		cout << "Send: mapping= " << address << " -> " << sourceRank;
		cout << " " << destination << " -> " << destinationRank;
		cout << endl;
		*/

		char * content = message.GetContent();
		int count = message.GetContentSize();

		MPI_Request * request = NULL;
		char * buffer = ring.GetBuffer(request);

		memcpy(buffer + sourceActorOffset, &address, sizeof(int));
		memcpy(buffer + destinationActorOffset, &destination, sizeof(int));

		memcpy(buffer + contentOffset, content, count);

		count += contentOffset;

		MPI_Datatype datatype = MPI_BYTE;

		/*
		cout << "Isend with buffer " << buffer << " " << count;
		cout << " from rank " << rank << " to rank ";
		cout << destinationRank;
		cout << " request is " << request << endl;
		*/

		MPI_Isend(buffer, count, datatype, destinationRank,
				tag, messagingCommunicator, request);

		messagesSent ++;
	}

	void Initialize(int * numberOfArguments, char *** argumentValues) {
		MPI_Init(numberOfArguments, argumentValues);

		MPI_Comm_dup(MPI_COMM_WORLD, &routingCommunicator);
		MPI_Comm_dup(MPI_COMM_WORLD, &messagingCommunicator);

		MPI_Comm_rank(messagingCommunicator, &rank);
		MPI_Comm_size(messagingCommunicator, &size);

		//cout << "Channel rank= " << rank << " size= " << size << endl;

		ring.Initialize();

		receivingBuffer = new char[4 * sizeof(int) + Message::MAXIMUM_SIZE];
	}

	void Finalize() {

		Print();

		MPI_Finalize();
	}

	void Print() {

		cout << "Channel metrics for rank " << rank << "/" << size << endl;

		cout << "probeOperations " << probeOperations << endl;
		cout << "messagesSent " << messagesSent << endl;
		cout << "messagesReceived " << messagesReceived << endl;
		cout << "routingMessagesSent " << routingMessagesSent << endl;
		cout << "routingMessagesReceived " << routingMessagesReceived << endl;

		ring.Print();
	}

	int GetRank() {
		return rank;
	}

	int GetSize() {
		return size;

	}

	int GetRankFromAddress(int & actorAddress) {

		return actorAddress % size;
	}

};

#endif
