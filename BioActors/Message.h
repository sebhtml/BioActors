
// License: see LICENSE.txt

#ifndef MessageHeader
#define MessageHeader

#define bufferSize (8 * 1024)

#include <string.h>

class Message{

	char content[bufferSize];

	int source;
	int destination;
	int tag;
	int size;


public:
	static int MAXIMUM_SIZE;

	Message();
	~Message();

	int GetSource();
	int GetDestination();
	int GetTag();
	char * GetContent();
	int GetContentSize();

	void SetSource(int newSource);
	void SetDestination(int newDestination);
	void SetTag(int newTag);
	void SetContentSize(int newSize);
};

#endif
