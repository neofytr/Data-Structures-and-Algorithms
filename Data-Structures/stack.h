#ifndef A4AA5F91_C34B_4B17_863D_CAC9481DC891
#define A4AA5F91_C34B_4B17_863D_CAC9481DC891

#include <stdbool.h>
#include <stdlib.h>
#include "./Allocator/allocator.h"

/*

Stack and Queue are elementary data structures that have a
common generalization: the double-ended queue (this has
far fewer applications though).

*/

/*

The stack is the simplest of all structures, with an obvious interpretation:
putting objects on the stack and taking them off again, with access possible only to the
top item. For this reason they are sometimes also described as LIFO storage: last in, first out.

Stacks occur in programming wherever we have nested blocks,
local variables, recursive definitions, or backtracking. Typical programming
exercises that involve a stack are the evaluation of arithmetic expressions with
parentheses and operator priorities, or search in a labyrinth with backtracking.

The stack should support at least the following operations:

1. push (obj): Put obj on the stack, making it the top item.
2. pop() : Return the top object from the stack and remove it from the stack.
3. stack_empty() : Test whether the stack is empty.

Also, the realization of the stack has, of course, to give the right values, so we need
to specify the correct behavior of the stack. One method would be an algebraic specification
of what correct sequences of operations and return values are. This has been done
for simple structures like the stack, but even then the specification is not very helpful
in understanding the structure. Instead, we can describe a canonical implementation on
an idealized machine, which gives the correct answer for all correct sequences (no pop
on an empty stack, no memory problems caused by bounded arrays). Assuming that the elements
we want to store on the stack are of type item_t, this could look as follows:

*/

#ifdef CANONICAL_STACK

size_t i = 0;
item_t stack[#INFINITY];

bool stack_empty(void)
{
    return (!i);
}

void push(item_t x)
{
    stack[i++] = x;
}

item_t pop(void)
{
    return stack[--i];
}

#endif

/*

This describes the correct working of the stack, but we have the problem of assuming
both an infinite array and that any sequence of operations will be correct.

A more realistic version might be the following:

*/

#ifdef STACK_ONE

#define MAX_SIZE

size_t i = 0;
item_t stack[MAX_SIZE];

bool stack_empty(void)
{
    return (!i);
}

bool push(item_t x)
{
    if (i < MAX_SIZE)
    {
        stack[i++] = x;
        return true;
    }

    return false;
}

item_t pop(void)
{
    return (stack[i--]);
}

#endif

/*

This now limits the correct behavior of the stack by limiting the maximum
number of items on the stack at one time so it is not really the correct stack we want,
but at least it does specify an error message in the return value if the stack overflow
is reached by one push too many.

This is a fundamental defect of array-based realizations of data structures: they are of
fixed-size, the size needs to be decided in advance, and the structure needs the full size
no matter how many items are really in the structure.

There is a systematic way to overcome these problems for array-based structures, which we will
see, but usually a solution with dynamically allocated memory is preferable.

We specified an error value only for the stack overflow condition, but not
for the stack underflow, because the stack overflow is an error generated by
the structure, which would not be present in an ideal implementation, whereas
a stack underflow is an error in the use of the structure and so a result in the
program that uses the stack as a black box.

Also, this allows us to keep the
return value of pop as the top object from the stack; if we wanted to catch
stack underflow errors in the stack implementation, we would need to return
the object and the error status.

A final consideration in our first stack version
is that we might need multiple stacks in the same program, so we want to
create the stacks dynamically. For this we need additional operations to create
and remove a stack, and each stack operation needs to specify which stack it
operates on. One possible implementation could be the following:

*/

#ifdef STACK_TWO

typedef struct
{
    item_t *base;
    item_t *top;
    size_t size;
} stack_t;

stack_t *create_stack(size_t size)
{
    stack_t *st = (stack_t *)allocate(sizeof(stack_t));
    st->base = (item_t *)allocate(size * sizeof(item_t));

    st->size = size;
    st->top = st->base;

    return (st);
}

bool stack_empty(stack_t *st)
{
    return ((uintptr_t)st->base == (uintptr_t)st->top);
}

bool push(item_t x, stack_t *st)
{
    if ((uintptr_t)st->top < (uintptr_t)st->base + st->size)
    {
        *(st->top) = x;
        st->top += 1;
        return true;
    }

    return false;
}

item_t pop(stack_t *st)
{
    st->top -= 1;
    return *(st->top);
}

item_t top_element(stack_t *st)
{
    return *(st->top - 1);
}

void remove_stack(stack_t *st)
{
    deallocate(st->base);
    deallocate(st);
}

#endif

/*

Again, we include some security checks and leave out others. Our policy
in general is to include those security checks that test for errors introduced
by the limitations of this implementation as opposed to an ideal stack, but
to assume both that the use of the stack is correct and that the underlying
operating system never runs out of memory. We included another operation
that is frequently useful, which just returns the value of the top element without
taking it from the stack.


Frequently, the preferable implementation of the stack is a dynamically allocated structure
using a linked list, where we insert and delete in front of the list. This has the advantage
that the structure is not fixed size; therefore, we need not be prepared for stack overflow
errors if we can assume that the memory of the computer is unbounded, and so we can
always get a new node.

It is as simple as the array-based structure if we already have the get_node and
return_node functions, whose correct implementations we discuss later:

*/

#ifdef STACK_THREE

typedef struct stack_ stack_t;

stack_t *get_node(void);
void return_node(stack_t *st);

struct stack_
{
    item_t item;
    stack_t *next;
};

stack_t *create_stack(void)
{
    stack_t *st;
    st = get_node();
    st->next = NULL;
    return st;
}

bool stack_empty(stack_t *st)
{
    return (!st->next);
}

void push(item_t x, stack_t *st)
{
    stack_t *tmp;
    tmp = get_node();

    tmp->item = x;

    tmp->next = st->next;
    st->next = tmp;
}

item_t pop(stack_t *st)
{
    stack_t *tmp;
    item_t tmp_item;

    tmp = st->next;
    st->next = tmp->next;
    tmp_item = tmp->item;

    return_node(tmp);
    return tmp_item;
}

item_t top_element(stack_t *st)
{
    return (st->next->item);
}

void remove_stack(stack_t *st)
{
    while (st)
    {
        stack_t *tmp = st;
        st = tmp->next;
        return_node(tmp);
    }
}

#endif

/*

Notice that we have a placeholder node in front of the linked list; even an empty stack
is represented by a list with one node, and the top of the stack is only the second node
in the list. This is necessary as the stack identifier returned by create_stack and used
in all stack operations should not be changed by stack operations (because if it's changed,
how will we return the new pointer to the user?). So we cannot just
use a pointer to the start of the linked list as a stack identifier.

Because the components of a node will be invalid after it is returned, we need temporary
copies of necessary values in pop and remove_stack. The operation remove_stack should return
all the remaining nodes; there is no reason to assume that only empty stacks will be removed,
and we will suffer a memory leak if we fail to return the remaining nodes.

The implementation as a dynamically allocated structure always has the advantage
of greater elegance; it avoids stack overflow conditions and needs just the memory
proportional to the actually used items, not a big array of a size estimated by the
programmer as upper bound to the maximum use expected to occur.

One disadvantage is a possible decrease in speed: dereferencing a pointer does not take
longer than incrementing an index, but the memory location accessed by the pointer might be
anywhere in the memory, whereas the next component of the array will be near the
previous component. Thus, array-based structures work well with the cache, whereas dynamicall
allocated structures might generate cache misses. So if we are quite certain about the
maximum possible size of the stack, for examples, because it's size is only logarithmic
in the size of the input, we will prefer the array-based version.

If one wants to combine these advantages, one could use a linked-list of blocks, each
block containing an array, but when the array becomes full, we just link it to a new node
with a new array. Such an implementation could look as follows:

*/

#ifdef STACK_FOUR

typedef struct stack_ stack_t;

struct stack_
{
    item_t *base;
    item_t *top;
    size_t size;
    stack_t *previous;
};

stack_t *create_stack(size_t size)
{
    stack_t *st;
    st = (stack_t *)allocate(sizeof(stack_t));

    st->base = (item_t *)allocate(size * sizeof(item_t));
    st->size = size;
    st->top = st->base;

    st->previous = NULL;
    return st;
}

bool stack_empty(stack_t *st)
{
    return (st->base == st->top && !st->previous);
}

void push(item_t x, stack_t *st)
{
    if (st->top < st->base + st->size)
    {
        *(st->top++) = x;
    }
    else
    {
        stack_t *new;
        new = (stack_t *)allocate(sizeof(stack_t));
        new->base = st->base;
        new->top = st->top;
        new->size = st->size;
        new->previous = st->previous;

        st->previous = new;
        st->base = (item_t *)allocate(sizeof(item_t) * st->size);
        st->top = st->base + 1;
        *(st->base) = x;
    }
}

item_t pop(stack_t *st)
{
    if (st->top == st->base)
    {
        stack_t *old;
        old = st->previous;

        deallocate(st->base);

        st->base = old->base;
        st->top = old->top;
        st->size = old->size;
        st->previous = old->previous;

        deallocate(old);
    }

    st->top -= 1;
    return *(st->top);
}

item_t top_element(stack_t *st)
{
    if (st->top == st->base)
    {
        return *(st->previous->top - 1);
    }
    else
    {
        return *(st->top - 1);
    }
}

void remove_stack(stack_t *st)
{
    while (st)
    {
        stack_t *tmp = st->previous;
        deallocate(st->base);
        deallocate(st);
        st = tmp;
    }
}

#endif

/*

In our classification, push and pop are update operations and stack_empty and top_element
are query operations.

For the linked-list implementation, the operations involve the external get node and
return node functions, which occur in both push and pop once, so the
implementation works only in constant time if we can assume these functions
to be constant-time operations. We will discuss the implementation of this
dynamic node allocation in Section 1.4, but we can assume here (and in all later
structures) that this works in constant time.

For the block list we allocate large
parts of memory for which we used here the standard memory management
operations malloc and free instead of building an intermediate layer, as
described in Section 1.4. It is traditional to assume that memory allocation and
deallocation are constant-time operations, but especially with the free there
are nontrivial problems with a constant-time implementation, so one should
avoid using it frequently. This could happen in the block list variant if there
are many push/pop pairs that just go over a block boundary. So the small
advantage of the block list is probably not worth the additional problems.

The create stack operation involves only one such memory alloca-
tion, and so that should be constant time in each implementation; but the
remove stack operation is clearly not constant time, because it has to de-
stroy a potentially large structure. If the stack still contains n elements, the
remove stack operation will take time O(n).

*/

#ifdef STACK_ARR_BASIC

#define ITEM_TYPE void *
typedef struct stack_ stack_t;
typedef ITEM_TYPE item_t;

struct stack_
{
    item_t *arr;
    size_t top;
    size_t max_size;
};

stack_t *create_stack(size_t max_size)
{
    stack_t *stack = (stack_t *)allocate(sizeof(stack_t));
    if (!stack)
    {
        return NULL;
    }

    item_t *arr = (item_t *)allocate(max_size * sizeof(item_t));
    if (!stack)
    {
        return NULL;
    }

    stack->arr = arr;
    stack->top = 0;
    stack->max_size = max_size;
    return stack;
}

bool push(item_t item, stack_t *stack)
{
    if (!stack || stack->top >= stack->max_size)
    {
        return false;
    }

    stack->arr[stack->top++] = item;
    return true;
}

item_t pop(stack_t *stack)
{
    if (!stack)
    {
        return (item_t)0;
    }
    return stack->arr[--stack->top];
}

item_t peek(stack_t *stack)
{
    if (!stack)
    {
        return (item_t)0;
    }
    return stack->arr[stack->top - 1];
}

bool is_empty(stack_t *stack)
{
    if (!stack)
    {
        return true;
    }
    return (stack->top == 0);
}

bool delete_stack(stack_t *stack)
{
    if (!stack)
    {
        return false;
    }

    deallocate(stack->arr);
    deallocate(stack);

    return true;
}

#endif

#ifdef STACK_ARR_ADV

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define ITEM_TYPE void *
typedef ITEM_TYPE item_t;

typedef struct stack_
{
    item_t *arr;
    size_t top;
    size_t max_size;
} stack_t;

typedef enum
{
    STACK_OK = 0,        // Operation successful
    STACK_ERR_NULL = 1,  // Stack is NULL
    STACK_ERR_FULL = 2,  // Stack is full
    STACK_ERR_EMPTY = 3, // Stack is empty
    STACK_ERR_ALLOC = 4  // Memory allocation failed
} stack_error_t;

typedef struct
{
    stack_error_t error;
} result_t;

typedef struct
{
    stack_error_t error;
    item_t value;
} value_result_t;

stack_t *create_stack(size_t max_size);
result_t push(item_t item, stack_t *stack);
value_result_t pop(stack_t *stack);
value_result_t peek(stack_t *stack);
bool is_empty(stack_t *stack);
result_t delete_stack(stack_t *stack);

stack_t *create_stack(size_t max_size)
{
    if (max_size == 0)
    {
        return NULL;
    }

    stack_t *stack = (stack_t *)allocate(sizeof(stack_t));
    if (!stack)
    {
        return NULL;
    }

    stack->arr = (item_t *)allocate(max_size * sizeof(item_t));
    if (!stack->arr)
    {
        deallocate(stack);
        return NULL;
    }

    stack->top = 0;
    stack->max_size = max_size;
    return stack;
}

result_t push(item_t item, stack_t *stack)
{
    if (!stack)
    {
        return (result_t){.error = STACK_ERR_NULL};
    }
    if (stack->top >= stack->max_size)
    {
        return (result_t){.error = STACK_ERR_FULL};
    }

    stack->arr[stack->top++] = item;
    return (result_t){.error = STACK_OK};
}

value_result_t pop(stack_t *stack)
{
    if (!stack)
    {
        return (value_result_t){.error = STACK_ERR_NULL, .value = NULL}; // Stack is NULL
    }
    if (stack->top == 0)
    {
        return (value_result_t){.error = STACK_ERR_EMPTY, .value = NULL}; // Stack is empty
    }

    return (value_result_t){.error = STACK_OK, .value = stack->arr[--stack->top]}; // Success
}

value_result_t peek(stack_t *stack)
{
    if (!stack)
    {
        return (value_result_t){.error = STACK_ERR_NULL, .value = NULL}; // Stack is NULL
    }
    if (stack->top == 0)
    {
        return (value_result_t){.error = STACK_ERR_EMPTY, .value = NULL}; // Stack is empty
    }

    return (value_result_t){.error = STACK_OK, .value = stack->arr[stack->top - 1]}; // Success
}

bool is_empty(stack_t *stack)
{
    return (!stack || stack->top == 0);
}

result_t delete_stack(stack_t *stack)
{
    if (!stack)
    {
        return (result_t){.error = STACK_ERR_NULL}; // Stack is NULL
    }

    deallocate(stack->arr);
    deallocate(stack);
    return (result_t){.error = STACK_OK}; // Success
}

#endif

#endif /* A4AA5F91_C34B_4B17_863D_CAC9481DC891 */
