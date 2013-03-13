#ifndef manager
#define manager

#include <iostream>
#include "structs.h"
#include "queue.h"
#include "warehouse.h"
#include "packaging.h"

using namespace std;

//
// Made By:	Connor Taylor
// Date	  : 2/28/13
// 
// This file defines the Manager class that is used in the Nozama Shipping
// project. This class will be able to read in and store a list of orders,
// send orders to the warehouse, receive orders back from shipping that have
// been processed, check if all orders have been processed, and do data 
// analysis on the total times spent in each simulation.
//

class Manager{

public:

	Manager();

    Warehouse retriever;

    Packaging shipping;

	//reads in the input file and stores all data in the orders queue.
	void readInFile();

	//checks if an order should be popped and sent to the retriever
	bool arrival(int time);	

	//pops an order to send to the warehouse
	Order* release();

	//receives an order that has been processed by shipping
	void receiveProcessed(Order* item);

	//tests if all the orders have been processed
	bool allDone();

	//sorts the orders by arrival time after a sim is run.
	void sortOrders();

	//outputs the data about the orders after they have been processed
	void print();

private:
	
	Queue orders; //holds all the orders that were read in from the file 
	int   numOrders;
	int   numProcessed; //the number of orders with processed set to true	
};

#endif

