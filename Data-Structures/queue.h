#ifndef C8B5EE2B_C413_4E2F_9318_EE10875680B4
#define C8B5EE2B_C413_4E2F_9318_EE10875680B4

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "./Allocator/allocator.h"

/*

The queue is a structure almost as simple as the stack; it also stores items, but it
differs from the stack in that it returns those items first that have been entered first, so it
is FIFO storage (first in, first out).

Queues are useful if there are tasks that have to be processed cyclically. Also, they are a central
structure in breadth-first search; breadth-first search and depth-first search really differ
only in that BFS uses a queue and DFS uses a stack to store the node that will be explored next.

The queue should support at least the following operations:

1. enqueue (obj): Insert obj at the end of the queue, making it the last item
2. dequeue (obj): Return the first object from the queue and remove it from the queue
3. queue_empty (obj): Test whether the queue is empty

The difference between queue and stack that makes the queue slightly
more difficult is that the changes occur at both ends: at one end, there are
inserts; at the other, deletes.

If we choose an array-based implementation for
the queue, then the part of the array that is in use moves through the array. If
we had an infinite array, this would present no problem. We could write it as
follows:

*/

#ifdef IDEAL_QUEUE

#define INFINITY

size_t lower = 0, size_t = 0;
typedef void *item_t;

item_t queue[INFINITY];

bool queue_empty(void)
{
    return (lower == upper);
}

void enqueue(item_t item)
{
    queue[upper++] = x;
}

item_t dequeue(void)
{
    return (queue[lower++]);
}

#endif

/*

A real implementation with a finite array has to wrap this around, using index calculation modulo
the length of the array. It could look as follows:

*/

#ifdef ARRAY_QUEUE

typedef void *item_t;

typedef struct
{
    item_t *base;
    size_t front;
    size_t rear;
    size_t size;
}

#endif

#endif /* C8B5EE2B_C413_4E2F_9318_EE10875680B4 */
