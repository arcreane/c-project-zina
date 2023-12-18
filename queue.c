
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


// functions made with the help of our courses from last year in data compression 


queue CreateQueue() // Function to create a new empty queue
{
    queue q; // Declare a queue variable
    q.front = NULL; // Initialize the front pointer to NULL, indicating the queue is empty
    return q;  
}

// Function to check if the queue is empty
int isEmptyQueue(queue q)
{
    return q.front == NULL; 
}

// Function to add an element with priority into the queue
int EnQueue(queue *q, struct TreeNode* e, int priority) {
    struct cell *temp, *cur, *prev;

    temp = (struct cell *) malloc(sizeof(struct cell)); // Dynamically allocate memory for a new cell
    if (!temp) { // Check if malloc was successful
        return -1; // Return -1 on failure
    }

    temp->data = e; // Set the data of the new cell
    temp->priority = priority; // Set the priority of the new cell
    temp->next = NULL; // Set the next pointer of the new cell to NULL

    // Insert the new cell into the queue
    cur = q->front;
    if (cur == NULL || priority < cur->priority) {  // Check if the queue is empty or the new cell has higher priority than the front cell
        temp->next = q->front; // Point new cell to the current front
        q->front = temp; // Make the new cell the new front
    } else {
        // Traverse the queue to find the right position for the new cell
        while (cur->next != NULL && cur->next->priority <= priority) {
            cur = cur->next;
        }
         // Insert the new cell in the found position
        temp->next = cur->next;
        cur->next = temp;
    }

    return 0; // Return 0 on success
}


// Function to remove the element with the highest priority from the queue
int DeQueue(queue *q)
{
    struct cell *temp;
    if (isEmptyQueue(*q)) return -1; // Return -1 if queue is empty
    temp = q->front; // Store the front cell in temp
    q->front = q->front->next; // Update the front pointer to the next cell
    free(temp);  // Free the memory of the dequeued cell
    return 0; // Return 0 on success
}


// Function to retrieve the data of the element at the front of the queue without removing it
int Front(queue q, element* e) {
    if (!isEmptyQueue(q) && e) {
        *e = q.front->data; // Set the passed element pointer to point to the front cell's data
        return 0; // Return O if success
    }
    return -1; // Return -1 if failure
}



