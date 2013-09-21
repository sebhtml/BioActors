#ifndef ChannelHeader
#define ChannelHeader

#include "Message.h"

#include <mpi.h>
#include <iostream>
using namespace std;

class Channel {

private:
	MPI_Comm routingCommunicator;
	MPI_Comm messagingCommunicator;

	int rank;
	int size;
	int maximumNumberOfActors;

public:
	Channel() {
		maximumNumberOfActors = 131072;
	}

	int GetMaximumNumberOfActors() {
		return maximumNumberOfActors;
	}

	bool Receive(Message & message) {

		int source = MPI_ANY_SOURCE;
		int tag = MPI_ANY_TAG;

		MPI_Status status;
		int flag = 0;

		MPI_Iprobe(source, tag, messagingCommunicator, &flag, 
				               &status);

		if(!flag)
			return false;

		source = status.MPI_SOURCE;
		tag = status.MPI_TAG;

		message.SetTag(tag);

		MPI_Datatype datatype = MPI_BYTE;

		int count = 0;
		MPI_Get_count(&status, datatype, &count);

		char * buffer = message.GetContent();
		buffer -= Message::METADATA_SIZE;

		MPI_Recv(buffer, count, datatype, source, tag,
				messagingCommunicator, &status);

		message.SetContentSize(count);

		/*
		cout << "Channel got a message with tag ";
		cout << tag << ", ";
		cout << message.GetSource() << " -> " << source;
		cout << " " << message.GetDestination();
		cout << " -> " << rank << endl;
		*/

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
		int sourceRank = GetRankFromAddress(address);
		int destinationRank = GetRankFromAddress(destination);

		/*
		cout << "Send: mapping= " << address << " -> " << sourceRank;
		cout << " " << destination << " -> " << destinationRank;
		cout << endl;
		*/

		char * buffer = message.GetContent();
		buffer -= Message::METADATA_SIZE;

		int count = message.GetContentSize();

		MPI_Request request;
		MPI_Datatype datatype = MPI_BYTE;

		MPI_Isend(buffer, count, datatype, destinationRank,
				tag, messagingCommunicator, &request);

		MPI_Request_free(&request);
	}

	void Initialize(int * numberOfArguments, char *** argumentValues) {
		MPI_Init(numberOfArguments, argumentValues);
		
		MPI_Comm_dup(MPI_COMM_WORLD, &routingCommunicator);
		MPI_Comm_dup(MPI_COMM_WORLD, &messagingCommunicator);

		MPI_Comm_rank(messagingCommunicator, &rank);
		MPI_Comm_size(messagingCommunicator, &size);

		//cout << "Channel rank= " << rank << " size= " << size << endl;
	}

	void Finalize() {

		MPI_Finalize();
	}

	int GetRank() {
		return rank;
	}

	int GetSize() {
		return size;

	}

	int GetRankFromAddress(int & actorAddress) {

		//return actorAddress / maximumNumberOfActors;

		return actorAddress % size;
	}



};

#endif
