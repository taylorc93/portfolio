#include <iostream>
#include "structs.h"
#include "queue.h"
#include "warehouse.h"
#include "packaging.h"
#include "manager.h"

using namespace std;

//constructor, sets both private ints to 0
Manager::Manager(){
	numOrders	  = 0;
	numProcessed  = 0;
}

/* Name: readInFile
 * Purpose: Reads in a file of orders and stores all of them in a queue in the
 * manager class
 * Calls: orders.pushBack
 * Returns: nothing
 * Notes: Don't forget to remove input->timeDone after part 2, total time will
 * be used in its place.
 */
void Manager::readInFile(){
	string junk;
	string custType;
	Order* input;

	while (cin >> junk){
		input = new Order;
		if (!cin.eof()){
		   	cin >> input->orderNum >> junk >> custType >> junk
				>> junk>> junk >> junk >> input->arrivalT >> junk
				>> junk >> input->fetchT >> junk >> junk >> junk 
				>> input->packT >> junk;
		}

   		input->timeLeft   = -1;
   	    input->processed  = false;
   		input->sim1TotalT = 0;
		input->sim2TotalT = 0;

   		if (custType == "regular"){
   			input->isSupreme = false;
   		}
   		else{
   			input->isSupreme = true;
   		}
   		orders.pushBack(input);
		numOrders++;
	}
}

//tests to see if the order at the front of the queue that should be sent to 
//the warehouse.  Done by popping the item, checking the arrivalTime, pushing
//it back to the front, and returning true if it should be sent out.
bool Manager::arrival(int t){
	if (!orders.isEmpty()){
		Order* temp = orders.pop();
		if (temp->arrivalT == t){
			orders.pushFront(temp);
			return true;
		}
		else{
			orders.pushFront(temp);
			return false;
		}
	}
	else{
		return false;
	}
}

//pops an order from the Queue and returns a pointer to it
Order* Manager::release(){
	Order* temp    = orders.pop();
	temp->timeLeft = temp->fetchT;
	return (temp);
}

//receives an order popped from shipping and updates numProcessed before
// pushing it back onto the queue
void Manager::receiveProcessed(Order* item){
	item->processed = true;
	numProcessed++;
	orders.pushBack(item);
}

//checks to see if all orders have been processed and the loop should be ended
bool Manager::allDone(){
	return (numProcessed == numOrders);
}

//sorts all the orders by arrival time by popping every item off and
//reorganizing them.
void Manager::sortOrders(){
	if (!orders.isEmpty()){
		int 	size = orders.numOrders();
		Order** temp = new Order* [size];

		for (int i = 0; i < size; i++){
			temp[i] = orders.pop();
		}
		for (int i = 0; i < size; i++){
			orders.orderedInsert(temp[i]);
		}
		delete [] temp;
	}
}

//outputs all the  data concerning the orders.
void Manager::print(){
	orders.print();
}
