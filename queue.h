
#include <stdio.h>
#include <stdlib.h>



typedef struct TreeNode* element;
// define a new type 'element' as a pointer to a structure treenode


/*this structure represents a cell in the priority queue. each cell contains the data (of type 'element'),
an integer representing its priority and a pointer to the next cell in the queue*/ 
struct cell {
    element data; 
    int priority;
    struct cell *next; 
};



// this define the queue type which has a single field 'front', a pointer ot the first 'cell' in the priority queue
typedef  struct { 
    struct cell *front; 
} queue;


queue CreateQueue(); // to create an empty PQ
int isEmptyQueue(queue q); // to check whether a PQ is empty
int EnQueue(queue *q, struct TreeNode* e, int priority);// to enqueue e of priority pr
int DeQueue(queue *q);// to dequeue the front element
int Front(queue q, element*e); // to get the front element
