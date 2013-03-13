#include <iostream>
#include "structs.h"
#include "queue.h"
#include "warehouse.h"
#include "packaging.h"
#include "manager.h"
using namespace std;

//
// Made by: Connor Taylor
// Date	  : 3/1/13
//
// This is the mainframe for the Nozama shipping project. The system will run
// two simulations of retrieving and packaging items.  The first does not
// consider supreme membership in any way, while the second allows supreme
// orders to jump to the front of the queue. At the end of the simulations,
// the min, max, and mean total times for each sim are calculated and compared.
//

void sim1(Manager unit);
void sim2(Manager unit);

//Creates the manager object, reads in the data, then runs the simulations
int main(){

	Manager command;
	command.readInFile();

	sim1(command);
	sim2(command);

}

/* Name: sim1
 * Purpose: Runs the first simulation of the retrieval/packaging system
 * Arguments: The manager object 
 * Calls: Everything (more or less)
 * Returns: nothing	   
 * Notes: This simulation will test retrieving and packaging orders without 
 * giving any preference to supreme orders
 */
void sim1(Manager command){
	for (int t = 0; true; t++){
		if (command.retriever.isDone()){
            command.shipping.receiveOrder(command.retriever.toPackaging(), 
										  true);
        }
		for (int i = 0; i < NUM_PACKERS; i++){
			if (command.shipping.isDone(i)){
				command.receiveProcessed(command.shipping.ship(i));
			}
		}
        if (command.allDone()){
            break;
        }

		while (command.arrival(t)){
			command.retriever.receiveOrder(command.release());
		}
        command.retriever.incrementTime(t, true);
		command.shipping.incrementTime(t, true);
	}
	command.sortOrders();
}

/* Name: sim2
 * Purpose: Runs the first simulation of the retrieval/packaging system        
 * Arguments: The manager object                                               
 * Calls: Everything (more or less)                                            
 * Returns: nothing                                                            
 * Notes: This simulation will test retrieving and packaging orders and give 
 * preference to supreme orders when they are being packaged.
 */
void sim2(Manager command){
    for (int t = 0; true; t++){
        if (command.retriever.isDone()){
            command.shipping.receiveOrder(command.retriever.toPackaging(),
                                          false);
        }
        for (int i = 0; i < NUM_PACKERS; i++){
            if (command.shipping.isDone(i)){
                command.receiveProcessed(command.shipping.ship(i));
            }
        }
        if (command.allDone()){
            break;
        }

        while (command.arrival(t)){
            command.retriever.receiveOrder(command.release());
        }
        command.retriever.incrementTime(t, false);
    	command.shipping.incrementTime(t, false);
	}
	command.sortOrders();
	command.print();
}
