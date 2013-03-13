#ifndef structs
#define structs

#include <iostream>
using namespace std;

// Made By	: Connor Taylor
// Date		: 2/20/13
//
// This header file defines all the structs that are going to be used in the
// Nozama shipping project.  The two structs that are going to be used are
// the struct to hold the order information and the struct to hold nodes for 
// the linked lists.

// holds all the order info
struct Order{
	int  arrivalT;
	int  fetchT;
	int  packT;
	int  timeLeft; // the time left this item needs to be in the queue
	int  orderNum;
	bool isSupreme;
	bool processed; // set to true once order goes through packaging
	int  sim1TotalT;
	int  sim2TotalT;
};

// doubly linked list that holds all the info in the queues
struct Node{
	Node* 	next;
	Order* 	item;
};

#endif

