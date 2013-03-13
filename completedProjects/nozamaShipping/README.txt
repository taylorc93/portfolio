Connor Taylor
Comp 15 Project 1
3/7/13

Link to the assignment description: http://www.cs.tufts.edu/comp/15/files/proj1/comp15s13proj1.pdf

File names:

structs.h
queue.h
packaging.h
warehouse.h
manager.h
queue.cpp
packaging.cpp
warehouse.cpp
manager.cpp
main.cpp

Class Interactions:

There are 4 classes that are used in my project: Warehouse, Packaging, Queue,
and Manager. There are 2 structs used, Node andOrder. Node defines nodes for a
doubly-linked list and Order holds all the information about an order. 

Queue defines a queue with a couple extra functions thrown in to make it more
usable for this project.  There is a queue in every other class in the project.

Packaging and Warehouse both define the areas where orders get processed.  Both
have very similar functions that take in an order and make sure that it is 
popped only when it is done being processed.

Manager is where all processed orders are stored.  This class helps control the
flow of the orders throughout the factory.  It also analyzes the data after all
orders have been processed.  It sends items to the warehouse when they arrive 
and receives them from the shipper when they are processed.



Flow:

The flow of the program will start with the Manager object taking in all the
orders. Main will then run simulations 1 and 2.  Each simulation will be
controlled by a different function.  In each sim, the orders will be sent out
by the Manager to the Warehouse, where they will stay until they are fully
processed.  After it is processed, the order is sent to Packaging. The
flow will differ per sim, but each queue will send the order to Packaging to
be processed.  Once it is finished, the packager will send the processed order
back to the Manager, which will mark the order are processed and store it back
in its queue. Once all orders have been processed, the Manager will output the
min, max, and mean total times.
