#ifndef warehouse
#define warehouse

#include <iostream>
#include "structs.h"
#include "queue.h"

using namespace std;

//
// Made By	: Connor Taylor
// Date		: 2/28/12
// 
// This file declares the Warehouse class in the Nozama Shipping Project. The
// class has public functions to access data in the private queue.  These are
// noOrders (checks isEmpty), receiveOrder (push), toPackaging (pop), and 
// incrementTime (updateTime).  These functions also do additional things 
// mentioned above their declarations. isDone checks if the Warehouse should
// pop an item by checking timeLeft. 
//

class Warehouse{

public:

	Warehouse();

	//checks to see if the queue is done processing an order
	bool isDone();
	
	//tests if the queue has no orders in it
	bool noOrders();

	//receives an item sent by the Manager and updates the order's timeLeft 
	//data and pushes it onto the queue
	void receiveOrder(Order* item);

	//pops an order to send it to the Packaging class, updates timeLeft, and
	//returns the item popped
	Order* toPackaging();

	//updates the total time for orders in the queue and timeLeft
	void incrementTime(int t, bool isSim1);

private:

	Queue  orders;	
	int    timeLeft;
};

#endif


