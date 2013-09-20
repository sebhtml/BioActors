
license: BSD
provides:

use MPI_Comm_dup to have a namespace to the library.

	allocateMessageHandle

	while(1){
	
		receiveMessage()
		
		if(haveMessage())
			processMessage()
	}


source is int
destination is int
tag is int
message content has bytes (char)

also, 2 * 32 bits for routing at the end of content


Actors are numbered using:

actorIdentifier = actorRank + iterator * numberOfRanks
