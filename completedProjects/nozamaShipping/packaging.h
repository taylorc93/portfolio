#ifndef packaging
#define packaging

#include <iostream>
#include "structs.h"

using namespace std;

//
// Made By: Connor Taylor
// Date   : 3/1/13
//
// This file declares the Packaging Class of the Nozama Shipping Project. The
// class has functions that access the queue.  None of these functions have
// been tested except for the constructor, as it was necessary for part 2.
//

const int NUM_PACKERS = 3;

class Packaging{

public:

    Packaging();

	//checks to see if the order is done being processed in the specified queue
    bool isDone(int queueNum);

	//receives an order (item is received from warehouse function toPackaging)
	void receiveOrder(Order* item, bool isSim1);

	//pops the order off the selected queue
    Order* ship(int queueNum);

	//updates the total time for orders in the queues and timeLefts 	
	void incrementTime(int t, bool isSim1);

private:

	//returns the queue number with the shortest regular wait time.
	int shortestWaitTimeReg(Order* item, int start, int end);

	//returns the queue number with the shortest wait time for a supreme order
	int shortestWaitTimeSup(Order* item);

	//adds an item to Queue[pos] and updates timeLeft[pos]
	void addOrder(Order* item, int pos);

	Queue* orders; //pointer to store multiple queues of orders
    int*   timeLeft; //pointer to times left in queue. Same order as queues
};

#endif

