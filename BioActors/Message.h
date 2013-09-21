
// License: see LICENSE.txt

#ifndef MessageHeader
#define MessageHeader

#define bufferSize (8 * 1024)

#include <string.h>

class Message{

	int tag;
	char content[bufferSize];
	int size;

	int source;
	int destination;

public:
	static int MAXIMUM_SIZE;

	Message() {

	}

	char * GetContent() {
		return content;
	}

	int GetTag() {
		return tag;
	}

	int GetContentSize() {
		return size;
	}

	void SetContentSize(int newSize) {
		size = newSize;

		// get the source and destination from the message
		/*
		memcpy(&source, content + sourceOffset, 1 * sizeof(int));
		memcpy(&destination, content + destinationOffset, 1 * sizeof(int));
		*/
	}

	int GetSource() {

		return source;
	}

	int GetDestination() {
		return destination;
	}

	void SetSource(int newSource) {

		source = newSource;

		//memcpy(content + sourceOffset, &source, 1 * sizeof(int));
	}

	void SetDestination(int newDestination) {
		destination = newDestination;

		//memcpy(content + destinationOffset, &destination, 1 * sizeof(int));
	}

	void SetTag(int newTag) {
		tag = newTag;
	}
};

int Message::MAXIMUM_SIZE = bufferSize;

#endif
