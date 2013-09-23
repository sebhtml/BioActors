
// License: see LICENSE.txt

#ifndef RingHeader
#define RingHeader

#include <mpi.h>
#include <vector>
using namespace std;

class Ring {

private:
	// TODO: these vectors should probably be list or be reserved
	// because when a vector doubles its size, the starting address
	// may change
	vector<char*> buffers;
	vector<MPI_Request> requests;
	vector<bool> busy;

	// TODO: build a list of ready buffers

	int last;
	int maximumToTest;

	char * CreateNewBuffer(MPI_Request * & request);

public:

	Ring();
	~Ring();

	void Initialize();
	char * GetBuffer(MPI_Request * & request);
	void TestBuffers();
	void Print();
};

#endif
