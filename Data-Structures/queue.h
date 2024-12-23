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
} queue_t;

queue_t *create_queue(size_t size)
{
    queue_t *new_queue = (queue_t *)allocate(sizeof(queue_t));
    if (!new_queue)
    {
        return NULL;
    }

    new_queue->size = size;
    new_queue->front = 0;
    new_queue->rear = 0;

    new_queue->base = (item_t *)allocate(sizeof(item_t) * new_queue->size);
    if (!new_queue->base)
    {
        deallocate(new_queue);
        return NULL;
    }

    return new_queue;
}

bool queue_empty(queue_t *queue)
{
    return (queue->front == queue->rear);
}

bool queue_full(queue_t *queue)
{
    return (queue->front == (queue->rear + 1) & (queue->size - 1)); // for this to work, size of queue must be a non-negative integral power of 2
}

bool enqueue(item_t item, queue_t *queue)
{
    size_t new_rear = (queue->rear + 1) & (queue->size - 1); // for this to work, size of queue must be a non-negative integral power of 2
    if (queue->front == new_rear)                            // checks if the queue is full
    {
        return false;
    }
    queue->base[queue->rear] = item;
    queue->rear = new_rear;

    return true;
}

item_t dequeue(queue_t *queue)
{
    size_t new_front = (queue->front + 1) & (queue->size - 1); // for this to work, size of queue must be a non-negative integral power of 2
    item_t result = queue->base[queue->front];
    queue->front = new_front;

    return result;
}

item_t peek_queue(queue_t *queue)
{
    return queue->base[queue->front];
}

void delete_queue(queue_t *queue)
{
    deallocate(queue->base);
    deallocate(queue);
}

#endif

/*

Again this has the fundamental disadvantage of any array-based structure - that it is of fixed size.
So it possibly generates overflow errors and does not implement the structure correctly as it limits
it this way. In addition, it always reserves this expected maximum size for the array, even if never
needs it. The preferred alternative is a dynamically allocated structure, with a linked list. The obvious
solution is the following:

*/

#ifdef QUEUE_LINKED_LIST

typedef void *item_t;
typedef struct node_ node_t;

typedef struct
{
    node_t *front;
    node_t *rear;
} queue_t;

struct node_
{
    item_t item;
    node_t *next;
};

queue_t *create_queue()
{
    queue_t *new_queue = (queue_t *)allocate(sizeof(queue_t));
    if (!new_queue)
    {
        return NULL;
    }

    new_queue->rear = NULL;
    new_queue->front = NULL;

    return new_queue;
}

bool queue_empty(queue_t *queue)
{
    return (!queue->front);
}

bool enqueue(item_t item, queue_t *queue)
{
    node_t *new_node = (node_t *)allocate(sizeof(node_t));
    if (!new_node)
    {
        return false;
    }

    new_node->item = item;
    if (!queue->rear) // i.e, if the queue is empty
    {
        queue->rear = queue->front = new_node;
        return true;
    }

    queue->rear->next = new_node;
    new_node->next = NULL;
    queue->rear = new_node;

    return true;
}

item_t dequeue(queue_t *queue)
{
    node_t *temp = queue->front;
    item_t temp_item = temp->item;

    queue->front = temp->next;
    if (queue_empty(queue))
    {
        queue->rear = NULL;
    }

    deallocate(temp);
    return temp_item;
}

item_t peek_queue(queue_t *queue)
{
    return (queue->front->item);
}

void delete_queue(queue_t *queue)
{
    node_t *current_node = queue->front;
    node_t *temp_node;

    while (current_node)
    {
        temp_node = current_node->next;
        deallocate(current_node);
        current_node = temp_node;
    }

    deallocate(queue);
}

#endif

/*

Because we want to remove items from the front of the queue, the pointers in the linked list are
oriented from front to the end, where we insert items.

There are two aesthetical disadvantages of this obvious implementation:

1. we need a special entry point structure, which is different from the list nodes, and
2. we always need to treat the operations involving an empty queue differently.

For insertions into an empty queue and removal of the last element of the queue, we need to change
both insertion and removal pointers; for all other operations we change only one of them.

The first disadvantage can be avoided by joining the list together to make it a cyclic list, with the
last pointer from the end of the queue pointing again to the beginning. We can then do without a
front pointer, because the rear point's next component points to the front point. By this, the entry point
to the queue needs only one pointer, so it is of the same type as the queue nodes.

The second disadvantage can be overcome by inserting a placeholder node in that cyclic list, between
the rear and front end of the cyclic list. The entry point still points to the rear end or, in the
case of an empty list, to that placeholder node. Then, at least for the insert, the empty list is
no longer a special case. So a cyclic list version is the following:

*/

#ifdef CYCLIC_LIST_QUEUE

typedef struct node_ node_t;
typedef node_t queue_t;

typedef void *item_t;

struct node_
{
    node_t *next;
    item_t item;
};

queue_t *create_queue()
{
    queue_t *queue = (queue_t *)allocate(sizeof(queue_t));
    if (!queue)
    {
        return NULL;
    }

    node_t *placeholder = (node_t *)allocate(sizeof(node_t));
    if (!placeholder)
    {
        deallocate(queue);
        return NULL;
    }

    queue->next = placeholder;
    placeholder->next = placeholder;

    return queue;
}

bool enqueue(item_t item, queue_t *queue)
{
    node_t *new_node = (node_t *)allocate(sizeof(node_t));
    if (!new_node)
    {
        return false;
    }

    new_node->item = item;

    node_t *rear_end = queue->next;
    node_t *placeholder = rear_end->next;

    /* If the queue is empty before insertion, both rear_end and placeholder point to the same node, guaranteeing correct behaviour */

    new_node->next = placeholder;
    rear_end->next = new_node;
    queue->next = new_node;

    return true;
}

bool queue_empty(queue_t *queue)
{
    return (queue->next->next == queue->next);
}

item_t dequeue(queue_t *queue)
{
    node_t *rear_end = queue->next;
    node_t *placeholder = rear_end->next;
    node_t *front = placeholder->next;

    item_t item = front->item;

    placeholder->next = front->next;
    deallocate(front);
    if (queue_empty(queue))
    {
        queue->next = placeholder;
    }

    return item;
}

item_t front_element(queue_t *queue)
{
    return (queue->next->next->next->item);
}

void remove_queue(queue_t *queue)
{
    node_t *placeholder = queue->next->next;
    node_t *current_node = queue->next->next->next;
    node_t *temp_node;

    while (current_node != placeholder)
    {
        temp_node = current_node->next;
        deallocate(current_node);
        current_node = temp_node;
    }

    deallocate(placeholder);
    deallocate(queue);
}

#endif

#endif /* C8B5EE2B_C413_4E2F_9318_EE10875680B4 */
