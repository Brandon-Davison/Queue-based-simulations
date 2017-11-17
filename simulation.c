#include "simulation.h"

// returns a pointer to dynamically allocated node.
Node *makeNode(const int customerNumber, const int serviceTime)
{
	Node *node = (Node*)malloc(sizeof(Node));
	if (node != NULL)
	{
		node->pNext = NULL;
		Data *data = (Data*)malloc(sizeof(Data));
		if (data != NULL)
		{
			data->customerNumber = customerNumber;
			data->serviceTime = data->totalTime = serviceTime;
			node->pData = data;
			return node;
		}
		else
		{
			free(node);
		}
	}
	return NULL;
}

// returns a pointer to a dynamically allocated queue.
Queue* makeQueue()
{
	Queue *queue = (Queue*)malloc(sizeof(Queue));
	if (queue != NULL)
	{
		queue->pHead = queue->pTail = NULL;
	}
	return queue;
}

// Adds newNode to the queue.
void enqueue(Queue *queue, Node *newNode)
{
	if (newNode != NULL)
	{
		if (queue->pHead == NULL)
		{
			queue->pHead = queue->pTail = newNode;
		}
		else
		{
			newNode->pNext = queue->pHead;
			queue->pHead = newNode;
		}
	}
}

/* Removes the last node in the queue and returns it as an output parameter.
 Precondtion: queue isn't empty. */
void dequeue(Queue *queue)
{
	Node *prev = NULL, *curr = queue->pHead;

	if (curr->pNext == NULL)
	{
		queue->pHead = queue->pTail = NULL;
	}
	else
	{
		while (curr->pNext != NULL)
		{
			prev = curr;
			curr = curr->pNext;
		}
		prev->pNext = NULL;
		queue->pTail = prev;
		free(curr);	
	}
}

// returns true if the queue is empty.
bool isEmpty(const Queue *queue)
{
	return queue->pHead == NULL;
}

// Traverses through the queue and calls printCustomerInfo() for each node
void printQueue(Node *node)
{
	if (node == NULL)
	{
		puts("Line is empty\n");
	}
	else
	{
		while (node != NULL)
		{
			printCustomerInfo(node, NA);
			node = node->pNext;
		}
	}
}

// prints which lane the customer is in, and their custoner number.
void printCustomerInfo(Node *node, const CustomerType customer)
{
	if (node != NULL)
	{
		printf("%s lane.\n", customer == EXPRESS ? "express" : "normal");
		printf("Customer Number: %d\n", node->pData->customerNumber);
		puts("");
	}
}

// Returns a random number between min and max. Precondition: srand() was called 
int getRand(const int min, const int max)
{
	double scale = (double)(max - min) / RAND_MAX;
	return min + floor(rand() * scale);
}

// runs one unit of time in the simulation. 
void runIteration(Queue *expressLane, Queue *normalLane)
{
	static int customerNumber = 1;
	static int expressArrival = 0, normalArrival = 0;
	Node *expressCustomer = NULL, *normalCustomer = NULL;
	 
	/* Update arrival times */
	expressArrival == 0 ? expressArrival = getRand(MIN_EXPRESS_ARRIVAL, MAX_EXPRESS_ARRIVAL) : expressArrival--;
	normalArrival == 0 ? normalArrival = getRand(MIN_NORMAL_ARRIVAL, MAX_NORMAL_ARRIVAL) : normalArrival--;

	/* Create new customers */
	if (expressArrival == 0) 
	{
		expressCustomer = makeNode(customerNumber, getRand(MIN_EXPRESS_SERVICE, MAX_EXPRESS_SERVICE));
		customerNumber++;
	}
	if (normalArrival == 0) 
	{
		normalCustomer = makeNode(customerNumber, getRand(MIN_NORMAL_SERVICE, MAX_NORMAL_SERVICE));
		customerNumber++;
	}
	
	/* place new customers into the queue and print their info. */
	if (expressCustomer != NULL)
	{
		enqueue(expressLane, expressCustomer);
		printCustomerInfo(expressCustomer, EXPRESS);
	}
	if (normalCustomer != NULL)
	{
		enqueue(normalLane, normalCustomer);
		printCustomerInfo(normalCustomer, NORMAL);
	}

	/* Process customers in queue */
	processLine(expressLane);
	processLine(normalLane);
}

/*
 * Called by runIteration, once for the expressLane and once for the normalLane.
 * Updates units of time and determines if the current customer in the line if finished,
 * and if they are, it removes them from the line. 
 */
void processLine(Queue *line)
{
	if (!isEmpty(line))
	{
		incrementWaitTimes(line);
		Data *currentCustomer = line->pTail->pData;
		currentCustomer->serviceTime--;
		
		if (currentCustomer->serviceTime == 0)
		{
			printf("Customer %d is finished, they waited %d minutes.\n", currentCustomer->customerNumber,
				currentCustomer->totalTime);
			dequeue(line);
		}
	}
}

/*
* Called by processLine()
* Adds a unit of time for each customer currently in line.
*/
void incrementWaitTimes(Queue *queue)
{
	Node *curr = queue->pHead;

	while (curr != queue->pTail)
	{
		curr->pData->totalTime++;
		curr = curr->pNext;
	}
}

bool runApp()
{
	Queue *expressLane = makeQueue(), *normalLane = makeQueue();
	if (expressLane == NULL || normalLane == NULL) return false;

	srand(time(NULL));

	int minutes;
	puts("How many minutes do you want to simulate?");
	scanf(" %d", &minutes);

	for (int i = 1; i < minutes + 1; ++i)
	{
		if (i % 10 == 0)
		{
			puts("--- 10 minutes have passed --- ");
			puts("Line for express Lane:");
			printQueue(expressLane->pHead);

			puts("Line for Noraml Lane:");
			printQueue(normalLane->pHead);
		}
		runIteration(expressLane, normalLane);
	}
	return true;
}
