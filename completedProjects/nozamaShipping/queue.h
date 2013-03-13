#ifndef queue
#define queue

#include <iostream>
#include "structs.h"
using namespace std;

// Made By: Connor Taylor
// Date: 3/1/13
//
// This file declares the Queue class for the Nozama Shipping project.  This 
// class has lots of functions.  It has the normal pushBack, pop, isEmpty, and 
// print functions, but also has a pushFront function which pushes an item to
// the top of the queue, like a stack.  This class also has an updateTime 
// function which updates the time variables in the nodes of the linked list
// and a waitTime function, which returns the length that an order will need to
// wait before it can be handled.
//

class Queue{

public:

	Queue();

	//adds a new item to the back, like a normal queue
    void pushBack(Order* item);

	//adds an item to the front of the queue
	void pushFront(Order* item);

	//adds an order to the queue.  Added to a varying location based on the 
	//arrival time of the order
	void orderedInsert(Order* item);

    Order* pop();

    bool isEmpty();

	//increments the total time of all orders in the queue
	void updateTime(int t, bool isSim1);

	//returns the wait time for a regular order (all the way at the back)
	int waitTimeRegular();

	//returns the wait time for a supreme order (behind the last supreme item)
	int waitTimeSupreme();

	//returns the number of supreme orders in the queue
	int numSupremeOrders();

	//returns the number of supreme orders in the queue
	int numSupremes();

    //returns the total number of orders in the queue.
	int numOrders();

	//prints out all the information concerning the total times of orders
	void print();

	void printMax(int sim);

	void printMin(int sim);

	void printMean(int sim);

private:

    Node* head;
	Node* tail;
};

#endif

