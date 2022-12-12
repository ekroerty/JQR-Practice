# Module 1
## CSD-D JQR Project v. 1
### September 22nd, 2022

<br>

# 1. Introduction
## 1.1 Overview

Module 1 serves as a project to determine a Trainee's ability to understand and implement various requirements of the Developer JQR. You, the Trainee, are tasked with creating, testing, and implementing various Data structures and Documentation.

- Data Structures: Data structures will be written in the C language and adhere to the BARR-C style Guide. The data structures written will be used for the various components in the Server, Clients, and Workers. 

- Testing and Documentation: Testing and documentation will be performed by the Trainee. Testing will fully test all aspects of every module and their interoperability. Documentation will serve as a setup guide and user guide.

<br>

## 1.2 Administrative
### 1.2.1 Time Considerations

This module should be completed with two (2) to three (3) months

<br>

# 2 Detailed Requirements

## 2.1 Data Structures

Data structures are C language modules created to be used as a library for various components during follow-on phases. Each data structure should be data type agnostic, meaning they can handle any data given to it. They should be designed in a finite way; finite inputs and outputs. 

Data Structures may be completed in any order; data structures may be used to build other data structures. Data structures that should be created are as follows:

<br>

### 2.1.1 Arrays

|Req. |Summary |
|-|-|
|2.1.1.1 |Correctly creates an array context |
|2.1.1.2 |Can hold any data type in the array |
|2.1.1.3 |Can return the size of the array |
|2.1.1.4 |Can add an element to the end of the array |
|2.1.1.5 |Can add an element to the beginning of the array |
|2.1.1.6 |Can insert an element anywhere within the array |
|2.1.1.7 |Can remove an element from the end of the array |
|2.1.1.8 |Can remove an element from the beginning of the array |
|2.1.1.9 |Can remove an element anywhere within the array |
|2.1.1.10 |Can get an element from n position within the array |
|2.1.1.11 |Can destroy all elements within an array using the correct deallocation function |
|2.1.1.12 |Can correctly destroy an array context |
|2.1.1.13 |Causes no memory leaks during all operations|

<br>

### 2.1.2 Multi-dimensional Arrays

> This module is conducive to building on the 'Array'

|Req. |Summary |
|-|-|
|2.1.2.1 |Correctly creates a multi-dimensional array context |
|2.1.2.2 |Can hold any data type in the multi-dimensional array |
|2.1.2.3 |Can return the size of the multi-dimensional array |
|2.1.2.4 |Can add an array to the end of the multi-dimensional array |
|2.1.2.5 |Can add an array to the beginning of the multi-dimensional array |
|2.1.2.6 |Can insert an array anywhere into the multi-dimensional array |
|2.1.2.7 |Can remove an array from the end of the multi-dimensional array |
|2.1.2.8 |Can remove an array from the beginning of the multi-dimensional array |
|2.1.2.9 |Can remove an array anywhere in the multi-dimensional array |
|2.1.2.10 |Can get an array at n position within a multi-dimensional array |
|2.1.2.11 |Can get an element at (n,n) position within a multi-dimensional array |
|2.1.2.12 |Can destroy all arrays within a multi-dimensional array using the correct deallocation functions |
|2.1.2.13 |Can correctly destroy a multi-dimensional array context |
|2.1.2.14 |Causes no memory leaks during all operations|

<br>

### 2.1.3 Circularly Linked List

|Req. |Summary |
|-|-|
|2.1.3.1 |Correctly creates a circularly linked list context |
|2.1.3.2 |Can hold any data type in the circularly linked list |
|2.1.3.3 |Can return the size of the circularly linked list |
|2.1.3.4 |Can add an element to the beginning of a circularly linked list |
|2.1.3.5 |Can add an element to the end of a circularly linked list |
|2.1.3.6 |Can insert an element anywhere into a circularly linked list |
|2.1.3.7 |Can remove an element from the beginning of a circularly linked list |
|2.1.3.8 |Can remove an element from the end of a circularly linked list |
|2.1.3.9 |Can remove an element from anywhere within a circularly linked list |
|2.1.3.10 |Can get an element at the n position of a circularly linked list |
|2.1.3.11 |Can destroy all elements within a circularly linked list using the correct deallocation functions |
|2.1.3.12 |Can correctly destroy a circularly linked list context |
|2.1.3.13 |Causes no memory leaks during all operations |
|2.1.3.14 |Can find the first occurrence of an item in a circularly linked list |
|2.1.3.15 |Can sort a circularly linked list alphanumerically given a function pointer (sorting function)|

<br>

### 2.1.4 Queue 
|Req. |Summary |
|-|-|
|2.1.4.1 |Correctly creates a queue context |
|2.1.4.2 |Can hold any data type in the queue |
|2.1.4.3 |Can return the size of the queue |
|2.1.4.4 |Can return a Boolean determining if the queue is empty |
|2.1.4.5 |Can enqueue an item into the queue |
|2.1.4.6 |Can dequeue an item from the queue |
|2.1.4.7 |Can peek at the top item in the queue |
|2.1.4.8 |Can destroy items in a queue using the correct deallocation functions |
|2.1.4.9 |Can correctly destroy a queue context |
|2.1.4.10 |Causes no memory leaks during all operations|

<br>

### 2.1.5 Priority Queue 
|Req. |Summary |
|-|-|
|2.1.5.1 |Correctly creates a priority queue context |
|2.1.5.2 |Can hold any data type in the priority queue |
|2.1.5.3 |Can return the size of the priority queue |
|2.1.5.4 |Can return a Boolean determining if the priority queue is empty |
|2.1.5.5 |Can properly enqueue an item priority queue based on the item's given priority |
|2.1.5.6 |Can dequeue an item from the priority queue |
|2.1.5.7 |Can destroy items in a priority queue using the correct deallocation functions |
|2.1.5.8 |Can correctly destroy a priority queue context |
|2.1.5.9 |Causes no memory leaks during all operations|

<br>

### 2.1.6 Stack 
|Req. |Summary |
|-|-|
|2.1.6.1 |Correctly creates a stack context |
|2.1.6.2 |Can hold any data type in the stack |
|2.1.6.3 |Can return the size of the stack |
|2.1.6.4 |Can return a Boolean determining if the stack is empty |
|2.1.6.5 |Can push an item to the top of the stack |
|2.1.6.6 |Can pop an item from the top of the stack |
|2.1.6.7 |Can peak at the item on the top of the stack |
|2.1.6.8 |Can destroy all items on the stack using the correct deallocation functions |
|2.1.6.9 |Can correctly destroy a stack context |
|2.1.6.10 |Causes no memory leaks during all operations|

<br>

### 2.1.7 Tree 
|Req. |Summary |
|-|-|
|2.1.7.1 |Correctly creates a tree context |
|2.1.7.2 |Can hold any data type in a tree |
|2.1.7.3 |Can find the first occurrence of an item in a tree |
|2.1.7.4 |Can insert an item into a specified location in a tree |
|2.1.7.5 |Can remove an item from a specified location in a tree |
|2.1.7.6 |Can destroy all items on the stack using the correct deallocation functions |
|2.1.7.7 |Can correctly destroy a tree context |
|2.1.7.8 |Causes no memory leaks during all operations|

<br>

### 2.1.8 Binary Search Tree 
|Req. |Summary |
|-|-|
|2.1.8.1 |Correctly creates a binary search tree context |
|2.1.8.2 |Can hold any data type in a binary search tree |
|2.1.8.3 |Can perform depth first traversal in a binary search tree |
|2.1.8.4 |Can perform breadth first traversal in a binary search tree |
|2.1.8.5 |Can locate an item in a binary search tree |
|2.1.8.6 |Can add an item to a binary search tree |
|2.1.8.7 |Can remove an item from a binary search tree |
|2.1.8.8 |Can destroy all items in a binary search tree using the correct deallocation functions |
|2.1.8.9 |Can correctly destroy a binary search tree context |
|2.1.8.10 |Causes no memory leaks during all operations|

<br>

### 2.1.9 Weighted Graph 
|Req. |Summary |
|-|-|
|2.1.9.1 |Correctly creates a weighted graph context |
|2.1.9.2 |Can hold any data type in a weighted graph |
|2.1.9.3 |Can define structures required for a weighted graph |
|2.1.9.4 |Can create a weighted graph with n number of nodes |
|2.1.9.5 |Can add n number of edges to a weighted graph |
|2.1.9.6 |Can find a node within an existing weighted graph |
|2.1.9.7 |Can find an edge within a weighted graph |
|2.1.9.8 |Can remove an edge from a weighted graph |
|2.1.9.9 |Can remove a node and all its edges from a weighted graph |
|2.1.9.10 |Can calculate the weight of a path within a weighted graph |
|2.1.9.11 |Can destroy all edges and nodes within a weighted graph using the correct deallocation functions |
|2.1.9.12 |Can correctly destroy a weighted graph context |
|2.1.9.13 |Causes no memory leaks during all operations|

<br>

### 2.1.10 Hash Table 
|Req. |Summary |
|-|-|
|2.1.10.1 |Correctly creates a hash table context |
|2.1.10.2 |Can hold any data type in a hash table |
|2.1.10.3 |Can navigate through a hash table to find the nth item |
|2.1.10.4 |Can find an item in a hash table |
|2.1.10.5 |Can remove selected items from a hash table |
|2.1.10.6 |Can insert an item into a hash table |
|2.1.10.7 |Can implement functionality to mitigate hash collisions within a hash table |
|2.1.10.8 |Can destroy all items in a hash table using the correct deallocation functions |
|2.1.10.9 |Can correctly destroy a hash table context |
|2.1.10.10 |Causes no memory leaks during all operations|

<br>

### 2.1.11 Thread Pool 
|Req. |Summary |
|-|-|
|2.1.11.1 |Correctly creates a thread pool context |
|2.1.11.2 |Can create a pool of worker threads that accept a job function pointer and arguments |
|2.1.11.3 |Can implement a queue to hold received jobs |
|2.1.11.4 |Can gracefully tear down when stop signal received |
|2.1.11.5 |Can destroy all jobs in a thread pool using the correct deallocation functions |
|2.1.11.6 |Can correctly destroy a thread pool context |
|2.1.11.7 |Causes no memory leaks during all operations|