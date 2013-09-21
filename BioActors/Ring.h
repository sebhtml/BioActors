
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

	char * CreateNewBuffer(MPI_Request * & request) {

		//cout << "Creating new buffer" << endl;

		char * buffer = new char[4 * sizeof(int) + Message::MAXIMUM_SIZE];
		bool state = false;
		MPI_Request newRequest;

		buffers.push_back(buffer);
		busy.push_back(state);
		requests.push_back(newRequest);

		int iterator = requests.size() - 1;

		busy[iterator] = true;
		request = &(requests[iterator]);

		//cout << "Returned request is " << request << endl;

		return buffers[iterator];
	}


public:

	Ring() {
		maximumToTest = 16;
	}

	void Initialize() {
		int toReserve = 128;

		buffers.reserve(toReserve);
		requests.reserve(toReserve);
		busy.reserve(toReserve);
	}

	char * GetBuffer(MPI_Request * & request) {

		if(requests.size() == 0) {
			return CreateNewBuffer(request);
		}

		int tested = 0;
		int iterator = (last - maximumToTest + requests.size()) % requests.size();

		while(tested < (int) requests.size()) {

			if(!busy[iterator]) {

				busy[iterator] = true;
				request = &requests[iterator];

				return buffers[iterator];
			}

			tested++;

			iterator = (iterator + 1) % requests.size();
		}

		return CreateNewBuffer(request);
	}

	void TestBuffers() {

		int iterator = last;
		int tested = 0;

		while(iterator < (int) requests.size()
				&& tested < maximumToTest) {

			int flag = 0;
			MPI_Status status;

			if(busy[iterator]) {

				MPI_Test(&requests[iterator], &flag,
					&status);
			}

			if(flag > 0) {

				busy[iterator] = false;
			}

			iterator++;
			tested++;
		}

		last = iterator % requests.size();

	}

	void Print() {

		cout << "Buffers in ring: " << buffers.size() << endl;
	}
};

#endif
