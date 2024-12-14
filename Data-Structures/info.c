/*

A data structure is a method to realize a set of operations on some data.

In general, a data structure is a kind of higher-level instruction in a virtual machine: when an
algorithm needs to execute some operations many times, it is reasonable to identify
what exactly the needed operations are and how they can be realized in the most efficient way.
This is the basic question of data structures: given a set of operations whose intended behavior is known,
how should we realize that behavior?

*/

/*

A data structure models some abstract object. It implements a number of operations
on this object, which usually can be classified into:

1. creation and deletion operations,
2. update operations, and
3. query operations

Once it has been created, the object is changed by the update operations.

The query operations do not change the abstract object, although they might change the
representation of the object in the data structure: this is called an adaptive data structure -
it adapts to the query to answer future similar queries faster.

Data structures that allow updates and queries are called dynamic data structures. There are
also simpler data structures that are created just once for some given object and allow
queries but no updates; these are called static data structures.

We want to ﬁnd data structures that realize a given abstract object and are
fast. The size of structures is another quality measure, but it is usually of less
importance. To express speed, we need a measure of comparison; this is the
size of the underlying object, not our representation of that object. Notice that
a long sequence of update operations can still result in a small object.

Our usual complexity measure is the worst-case complexity; so an operation in a
speciﬁc data structure has a complexity O(f (n)) if, for any state of the data
structure reached by a sequence of update operations that produced an object of
size n, this operation takes at most time Cf (n) for some C.

An alternative but weaker measure is the amortized complexity; an update operation has amortized
complexity O(f (n)) if there is some function g(n) such that any sequence of
m of these operations, during which the size of the underlying object is never
larger than n, takes at most time g(n) + mCf (n), so in the average over a long
sequence of operations the complexity is bounded by Cf (n).

Some structures are randomized, so the data structure makes some random
choices, and the same object and sequence of operations do not always lead
to the same steps of the data structure. In that case we analyze the expected
complexity of an operation. This expectation is over the random choices of the
data structure; the complexity is still the worst case of that expectation over all
objects of that size and possible operations.

In some situations, we cannot expect a nontrivial complexity bound of type
O(f (n)) because the operation might give a large answer. The size of the answer
is the output complexity of the operation, and, for operations that sometimes
have a large output complexity, we are interested in output-sensitive methods,
which are fast when the output is small. An operation has output-sensitive
complexity O(f (n) + k) if, on an object of size n that requires an output of
size k, the operation takes at most time C(f (n) + k).

For dynamic data structures, the time to create the structure for an empty
object is usually constant, so we are mainly interested in the update and query
times. The time to delete a structure of size n is almost always O(n). For static
data structures we already create an object of size n, so there we are interested
in the creation time, known as preprocessing time, and the query time.

*/

/*

An abstract object is a conceptual or logical entity that we want to represent and
manipulate using a data structure. It is defined by the operations it supports and the
expected behavior of those operations, but it does not specify how those operations
are implemented. Examples are:

1. Stack: An object that supports operations like push, pop, and peek.
2. Queue: An object that supports enqueue and dequeue.

The abstract object is the idea or model — it defines what operations should exist and their behavior
but doesn't define how to implement them.


A data structure is a concrete implementation of an abstract object.
It provides a way to organize and store data in memory to efficiently
realize the operations defined by the abstract object. For instance:

A stack can be implemented using:
    An array with a pointer to the top.
    A linked list where each node points to the next.
A queue can be implemented using:
    A circular array.
    A doubly linked list.

The data structure determines how the operations of the abstract object are executed,
impacting their efficiency in terms of time and space.

*/
