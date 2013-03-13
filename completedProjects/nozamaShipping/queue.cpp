#include <iostream>
#include "queue.h"
#include <cstdlib>

using namespace std;

//constructor, sets head and tail equal to NULL
Queue::Queue(){
	head = NULL;
	tail = NULL;
}

bool Queue::isEmpty(){
	return (head == NULL);
}

//pushes an item to the back of the queue
void Queue::pushBack(Order* item){
   	Node* node = new Node;
	node->item = item;

    if (head == NULL){
        head       = node;
        tail       = node;
        node->next = NULL;
    }
    else{
        tail->next = node;
        node->next = NULL;
        tail       = node;
    }
}

//pushes an item to the front of the queue
void Queue::pushFront(Order* item){
	Node* newNode = new Node;
	newNode->item = item;

	if (head == NULL){
		head 		  = newNode;
		tail		  = newNode;
		newNode->next = NULL;
	}
	else{
		newNode->next = head;
		head 		  = newNode;
	}
}

/* Name: orderedInsert
 * Purpose: places an order into the queue based on its arrival time
 * Calls: none
 * Returns: nothing
 * Notes: called by sortOrders to reorganize the list of orders in the manager
 * class
 */
void Queue::orderedInsert(Order* item){
	Node* newNode = new Node;
	newNode->item = item;

	if (head == NULL){
		head = newNode;
		tail = newNode;
		newNode->next = NULL;
	}
	else{
		Node* iter  = head;
		Node* iter2 = NULL;
		while (iter != NULL){
			if (iter->item->arrivalT > item->arrivalT){
				if (iter2 == NULL){
					newNode->next = iter;
					head		  = newNode;
			   		return;
				}
				else{
					iter2->next   = newNode;
					newNode->next = iter;
					return;
				}
			}
			iter2 = iter;
			iter  = iter->next;
		}
		newNode->next = NULL;
		iter2->next   = newNode;
		tail		  = newNode;
	}
}

//pops an item off the front of the queue
Order* Queue::pop(){
	if (isEmpty()){
		cout << "popped an empty queue" << endl;
		exit(1);
	}
	Node* temp = head;
	if (head == tail){
		head = NULL;
		tail = NULL;
	}
	else{
		head = head->next;
	}
	Order* info = temp->item;
	return info;
}

//updates the timeLeft for the first item in the queue (since it is being 
//processed) and then increments total time and updates timeDone for all items.
void Queue::updateTime(int t, bool isSim1){
	Node* iter = head;
   	if (iter != NULL){
		iter->item->timeLeft--;
	}
	while (iter != NULL){
		if (isSim1){
			iter->item->sim1TotalT++;
		}
		else{
			iter->item->sim2TotalT++;
		}
		iter = iter->next;
	}
}

//returns the wait time for a regular order that will be added to the queue
int Queue::waitTimeRegular(){
	int   time = 0;
	Node* iter = head;
	while (iter != NULL){
		time += iter->item->timeLeft;
		iter  = iter->next;
	}
	return time;
}

//returns the wait time for a supreme order that will be added to the queue
int Queue::waitTimeSupreme(){
	int   time = 0;
	Node* iter = head;

	while ((iter != NULL) && (iter->item->isSupreme)){
		time += iter->item->timeLeft;
		iter  = iter->next;
	}
	return time; 
}

//returns the number of supreme orders in a queue
int Queue::numSupremes(){
	Node* iter  = head;
	int   count = 0;

	while ((iter != NULL) && (iter->item->isSupreme)){
		count++;
		iter = iter->next;
	}
	return count;
}

//returns the total number of orders in a queue.
int Queue::numOrders(){
	Node* iter  = head;
	int   count = 0;

	while (iter != NULL){
		count++;
		iter = iter->next;
	}
	return count;
}

//prints out all the data about each order in a queue.  In addition, also 
//outputs the min, max, and mean times from each simulation. Should only be
//called at the end of the program when total times are acquired.
void Queue::print(){
	Node* iter = head;

	while (iter != NULL){ 
        cout << "Order " << iter->item->orderNum << " for ";
        if (iter->item->isSupreme == true){
            cout << "supreme ";
        }
        else{
			cout << "regular ";
        }
        cout << "customer arrived at time " << iter->item->arrivalT << endl;
        cout << "fetch time " << iter->item->fetchT << " minutes, ";
        cout << "pack time " << iter->item->packT << " minutes" << endl;
        cout << "elapsed time sim1 ";
        cout << iter->item->sim1TotalT << " minutes, ";
        cout << "elapsed time sim2 ";
        cout << iter->item->sim2TotalT << endl << endl;

		iter = iter->next;
	}

	cout << "..." << endl << endl;

	printMin(1);
	printMax(1);
    printMean(1);
	cout << numOrders() << " orders processed" << endl << endl;

	printMin(2);
	printMax(2);
	printMean(2);
    cout << numOrders() << " orders processed" << endl << endl;

}

//finds and prints the max total time for the sim passed in
void Queue::printMax(int sim){
	Node* iter = head;
	int   max  = 0;

	while (iter != NULL){
		if (sim == 1){
			if (iter->item->sim1TotalT > max){
				max = iter->item->sim1TotalT;
			}
		}
		else if (sim == 2){
			if (iter->item->sim2TotalT > max){
				max = iter->item->sim2TotalT;
			}		
		}	
		iter = iter->next;		
	}

	cout << "sim" << sim << " max elapsed time " << max << " minutes" << endl;

}

//finds and prints the min total time for the sim passed in
void Queue::printMin(int sim){
    Node* iter = head;
    int   min  = 10000;

    while (iter != NULL){
        if (sim == 1){
            if (iter->item->sim1TotalT < min){
                min = iter->item->sim1TotalT;
            }
        }
        else if (sim == 2){
            if (iter->item->sim2TotalT < min){
                min = iter->item->sim2TotalT;
            }
        }
        iter = iter->next;
    }

    cout << "sim" << sim << " min elapsed time " << min	<< " minutes" << endl;

}

//finds and prints the mean total time for the sim passed in.  If sim2 is 
//passed, it also outputs the mean total times for the supreme orders and the
//regular orders.
void Queue::printMean(int sim){
	int   sum       = 0;
	int   sumReg    = 0;
	int   sumSup	= 0;
	int   numSups   = 0;
	int   numRegs   = 0;
	int   numTotal  = 0;
	int   mean;
	Node* iter      = head;

	while (iter != NULL){
		if (sim == 1){
			sum += iter->item->sim1TotalT;
		}
		else if (sim == 2){
			sum += iter->item->sim2TotalT;
			if (iter->item->isSupreme){
				sumSup += iter->item->sim2TotalT;
				numSups++;
			}
			else{
				sumReg += iter->item->sim2TotalT;
				numRegs++;
			}
		}
		numTotal++;
		iter = iter->next;
	}

	if (sim == 1){
		cout << "sim1 mean elapsed time " << sum/numTotal << " minutes"
			 << endl;
	}
	else if (sim == 2){
		cout << "sim2 mean total time for all customers " << sum/numTotal 
			 << " minutes" << endl;
		cout << "sim2 mean total time for supreme customers "
			 << sumSup/numSups << " minutes" << endl;
		cout << "sim2 mean total time for regular customers " 
			 << sumReg/numRegs << " minutes" << endl;
	}
}
