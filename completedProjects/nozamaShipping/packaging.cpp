#include <iostream>
#include "structs.h"
#include "queue.h"
#include "packaging.h"
using namespace std;

//constructor, sets time left to sentinel val of -1
Packaging::Packaging(){
    orders   = new Queue [NUM_PACKERS];
	timeLeft = new int   [NUM_PACKERS];
	for (int i = 0; i < NUM_PACKERS; i++){
		timeLeft[i] = -1;
	}
}

//checks if an order should be popped.
bool Packaging::isDone(int i){
	return (timeLeft[i] == 0);
}

/* Name: receiveOrder
 * Purpose: takes an order sent to it from the warehouse and determines where
 * to add it before adding it to a queue. 
 * Calls: shortestWaitTimeSup, shortestWaitTimeReg, addOrder
 * Returns: nothing
 * Notes: None
 */
void Packaging::receiveOrder(Order* item, bool isSim1){
	int queueNum;
	if ((item->isSupreme) && (!isSim1)){
		queueNum = shortestWaitTimeSup(item);
	}
	else{
		queueNum = shortestWaitTimeReg(item, 0, NUM_PACKERS);
	}

	if (isSim1){
		if (item->timeLeft == -1){
			item->timeLeft = item->packT;
		}
		if (orders[queueNum].isEmpty()){
			timeLeft[queueNum] = item->packT;
		}
		orders[queueNum].pushBack(item);
	}
	else{
		addOrder(item, queueNum);
	}
}

//pops an item off the queue to ship out.  Also updates timeleft by either 
//making it the timeLeft of the next order or -1 if the queue is now empty
Order* Packaging::ship(int i){
    Order* temp = orders[i].pop();
    Order* temp2;
    if (!orders[i].isEmpty()){
        temp2  		= orders[i].pop();
        timeLeft[i] = temp2->timeLeft;
        orders[i].pushFront(temp2);
    }
	else{
		timeLeft[i] = -1;
	}
    return temp;
}

/* Name: shortestWaitTimeSup
 * Purpose: finds the queue with the shortest wait time for supreme orders
 * Calls: shortestWaitTimeReg (not always)
 * Returns: the index of the best queue to use.
 * Notes: In the case of a tie, the queue with the shortest regular wait time
 * is used. If this is also a tie, the queue with the lower index is chosen
 */
int Packaging::shortestWaitTimeSup(Order* item){
	int* waitTimes = new int [NUM_PACKERS];
	for (int i = 0; i < NUM_PACKERS; i++){
		waitTimes[i] = orders[i].waitTimeSupreme();
		if (waitTimes[i] == 0){
			delete [] waitTimes;
			return i;
		}
	}
	int  bestQueue = 0;
	int  bestTime  = waitTimes[0];
	for (int i = 1; i < NUM_PACKERS; i++){
		if (bestTime > waitTimes[i]){
			bestTime = waitTimes[i];
		}
		else if (bestTime == waitTimes[i]){
			bestQueue = shortestWaitTimeReg(item, bestQueue, i);
			bestTime  = waitTimes[bestQueue];
		} 
	}
	delete [] waitTimes;
	return bestQueue;
}

//loops through the queue and finds the best queue to place an item in.  If 
//there is a tie, it prefers the queue with the lower index.
int Packaging::shortestWaitTimeReg(Order* item, int start, int end){
	int bestTime  = orders[start].waitTimeRegular();
	int bestQueue = start;
	int diff	  = end - start;
	int test;
	for (int i = start + 1; i < end; i++){
		test = orders[i].waitTimeRegular();
		if (test < bestTime){
			bestTime  = test;
			bestQueue = i;
		}
		if ((test == bestTime) && (diff == 1)){
			return bestQueue;
		}
	}
	return bestQueue;
}

/* Name: addOrder
 * Purpose: adds an order to a queue with index i received from the warehouse
 * Calls: pushBack, pushFront, pop
 * Returns: nothing
 * Notes: tests for an empty queue first, then checks if the item is supreme 
 * and should be pushed to the front (or behind the other supreme orders in 
 * front of it)
 */
void Packaging::addOrder(Order* item, int i){

	if (item->timeLeft == -1){	
		item->timeLeft = item->packT;
	}
	if (orders[i].isEmpty()){
		timeLeft[i] = item->timeLeft;
		orders[i].pushBack(item);
		return;
	}

	if (item->isSupreme){
		int size = orders[i].numSupremes();
		if (size == 0){
			timeLeft[i] = item->timeLeft;
			orders[i].pushFront(item);
			return;
		}

		Order** temp = new Order* [size];
		for (int k = 0; k < size; k++){
			temp[k] = orders[i].pop();
		}
		orders[i].pushFront(item);

		for (int k = size - 1; k >= 0; k--){
			orders[i].pushFront(temp[k]);
		}
		delete [] temp;
	}
	else{
		orders[i].pushBack(item);
	}
	return;
}

//updates the time of all orders in all the queues and the timeLefts of the 
//queues.
void Packaging::incrementTime(int t, bool isSim1){
	for (int i = 0; i < NUM_PACKERS; i++){
	    timeLeft[i]--;
		orders[i].updateTime(t, isSim1);
	}
}
