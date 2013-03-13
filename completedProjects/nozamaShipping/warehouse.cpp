#include <iostream>
#include "structs.h"
#include "queue.h"
#include "warehouse.h"
using namespace std;

//constructor, sets timeLeft to sentinel val of -1
Warehouse::Warehouse(){
	timeLeft = -1;
}

//checks to see if an order should be popped
bool Warehouse::isDone(){
	return (timeLeft == 0);
}

// Takes the order, changes its timeLeft to equal its fetch time, and pushes it
// to the queue
void Warehouse::receiveOrder(Order* item){
	if (item->timeLeft == -1){
   		item->timeLeft = item->fetchT;
	}
	if (orders.isEmpty()){
		timeLeft = item->fetchT;
	}
	orders.pushBack(item);
}

//Pops the item that should be sent and the next item (if there is one). If
//a second item exists, timeLeft is updated to that item's fetch time.  It is
//then pushed to the front and the first popped item is returned.
Order* Warehouse::toPackaging(){
	Order* temp    = orders.pop();
	temp->timeLeft = -1;
	Order* temp2;
	if (!orders.isEmpty()){
		temp2	 = orders.pop();
		timeLeft = temp2->timeLeft;
		orders.pushFront(temp2);
	}
	return temp;
}

//calls updateTime and updates timeLeft
void Warehouse::incrementTime(int t, bool isSim1){
	orders.updateTime(t, isSim1);
	timeLeft--;
}
