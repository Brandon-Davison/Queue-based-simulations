#ifndef SIMULATION_H
#define SUMULATION_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Arrival times
#define MIN_EXPRESS_ARRIVAL 1
#define MAX_EXPRESS_ARRIVAL 5

#define MIN_NORMAL_ARRIVAL 3 
#define MAX_NORMAL_ARRIVAL 8

// Service times
#define MIN_EXPRESS_SERVICE 1
#define MAX_EXPRESS_SERVICE 5

#define MIN_NORMAL_SERVICE 3
#define MAX_NORMAL_SERVICE 8

typedef struct data 
{
	int customerNumber; // Unique identifier; starts at 1; after 24 hours, should be reset to 1
	int serviceTime;    // Random time; varies between express and normal lanes; units in minutes
	int totalTime;      // totalTime = serviceTime + sum of serviceTimes of customers in line before this customer; units in minutes
} Data; 

typedef struct node 
{
	Data *pData;
	struct queueNode *pNext;
} Node;

typedef struct queue
{
	Node *pHead, *pTail;
} Queue;

typedef enum {
	EXPRESS = 1, NORMAL = 2, NA = 3
} CustomerType;
typedef CustomerType LaneType;

// returns a pointer to dynamically allocated node.
Node *makeNode(const int customerNumber, const int serviceTime);

// returns a pointer to a dynamically allocated queue.
Queue* makeQueue();

// adds newNode to the queue
void enqueue(Queue *queue, Node *newNode);

/* removes the last node in the queue and returns it as an output parameter.
 precondition: queue isn't empty. */
void dequeue(Queue *queue); 

// returns true if the queue is empty.
bool isEmpty(const Queue *queue);

// Traverses through the queue and calls printCustomerInfo() for each node
void printQueue(Node *node);

// prints which lane the customer is in, and their custoner number.
void printCustomerInfo(Node *node, const CustomerType customer);

// Returns a random number between min and max. Precondition: srand() was called 
int getRand(const int min, const int max); 

// runs one unit of time in the simulation. 
void runIteration(Queue *expressLane, Queue *normalLane); 

/* 
 * Called by runIteration, once for the expressLane and once for the normalLane.
 * Updates units of time and determines if the current customer in the line if finished, 
 * and if they are, it removes them from the line. 
 */
void processLine(Queue *line);

/*
 * Called by processLine()
 * Adds a unit of time for each customer currently in line.
 */
void incrementWaitTimes(Queue *queue);

bool runApp();

#endif
