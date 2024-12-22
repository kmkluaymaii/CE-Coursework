//
//  LinkedList.h
//  Linked_List
//
//  Copyright Tali Moreshet. All rights reserved.
//
//  This is an implementation of an unordered Doubly Linked List class, which is made up of Nodes.

#ifndef LinkedList_h
#define LinkedList_h

#include <iostream>

using namespace std;

class Node {
public:
    Node(int data);                                 // constructor
    int data;                                       // node data, doubles as key
    Node *next;                                     // node next pointer
    Node *prev;                                     // node previous pointer
    void print();                                   // prints data value
};

class LinkedList {
public:
    LinkedList();                                   // constructor
    ~LinkedList();                                  // destructor
    void insertToTail(int data);                    // insert node with data to tail of list
    int removeFromTail();                           // remove node from tail of list, return its data
    void insertToHead(int data);                    // insert node with data to head of list
    int removeFromHead();                           // remove node from head of list, return its data
    void insertToMedian(int data);                  // insert node with data to median position of list
    int removeFromMedian();                         // remove node from meidan position of list, return its data
    int returnMedian();                             // prints the data of the median position
    void print();                                   // prints linked list data
    Node* head;                                     // head of list
    Node* tail;                                     // tail of list
    Node* median;                                   // median of list
    int size;                                       // number of elements in the list
};

#endif /* LinkedList_h */
