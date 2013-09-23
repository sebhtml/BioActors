

// License: see LICENSE.txt

#include "Message.h"


int Message::MAXIMUM_SIZE = bufferSize;

Message::Message() {

	source = -1;
	destination = -1;
	tag = -1;
	size = 0;
}

Message::~Message() {

}

char * Message::GetContent() {
	return content;
}

int Message::GetTag() {
	return tag;
}

int Message::GetContentSize() {
	return size;
}

void Message::SetContentSize(int newSize) {
	size = newSize;

}

int Message::GetSource() {

	return source;
}

int Message::GetDestination() {
	return destination;
}

void Message::SetSource(int newSource) {

	source = newSource;
}

void Message::SetDestination(int newDestination) {
	destination = newDestination;
}

void Message::SetTag(int newTag) {
	tag = newTag;
}

