// Rawisara Chairat, rawisara@bu.edu

/* To remove median, 
1. Check if there is anything in the list, if not then return -1 and error message.
2. Find the median position, then set a pointer to that position
3. If there is only one value in the list, then make everything point to null
4. If there are more than one value in the list,
    4.1. If there is a node before median: set the node before median to point to the node after median
    4.2. If there is a node after median: set the mode after median to point to the node before median
    4.2. If size is odd then set median to one position before the old median
    4.3. If size is even, then set median to the next position
5. Decrease the size of the list by one and delete the median, then return its data
*/

#include <iostream>
#include "LinkedList.h"

using namespace std;

//Node Class
Node::Node(int data){
    this -> data = data;
    this -> next = nullptr;
    this -> prev = nullptr;
}
void Node::print(){
    cout<< data;
}

//Linked List Class
LinkedList::LinkedList(){
    size = 0;
    head = nullptr;
    tail = nullptr;
    median = nullptr;
}

LinkedList::~LinkedList(){
    Node* temp = head;
    while (temp != nullptr){
        Node* next = temp -> next;
        delete temp;
        temp = next;
    } 
}

void LinkedList::insertToTail(int data){
    Node* list = new Node(data);
    if(size == 0){
        head = tail = median = list;
    } else{
        tail->next = list;
        list->prev = tail;
        tail = list;
        if (size % 2 == 0){
            median = median -> next;
        }
    }
    size++;
}

int LinkedList::removeFromTail(){
    if(size == 0){
        cout<< "Could not remove from tail."<<endl;
        return -1;
    }
    int data = tail -> data;
    Node* remove = tail;

    if(size == 1){
        head = tail = median = nullptr;
    } else {
        tail = tail -> prev;
        tail -> next = nullptr;
        if(size % 2 == 1){
            median = median -> prev;
        }
    }
    size --;
    delete remove;
    return data;
}
void LinkedList::insertToHead(int data){
    Node* list = new Node(data);
    if(size == 0){
        head = tail = median = list;
    } else {
        list -> next = head;
        head -> prev = list;
        head = list;
        if (size % 2 == 1){
            median = median -> prev;
        }
    }
    size++;
} 
int LinkedList::removeFromHead(){
    if(size == 0)  {
        cout<< "Could not remove from head."<<endl;
        return -1;
    }

    int data = head -> data;
    Node* remove = head;

    if(size == 1){
        head = tail = median = nullptr;
    } else {
        head = head -> next;
        head -> prev = nullptr;
        if(size % 2 == 0){
            median = median -> next;
        }
    }
    size --;
    delete remove;
    return data;
}
void LinkedList::insertToMedian(int data){
     Node* list = new Node(data);
    if(size == 0){
        head = tail = median = list;
    } else if (size == 1){
        list -> next = head;
        head -> prev = list;
        head = list;
        median = head -> next;
    } else {
        list -> next = median;
        list -> prev = median -> prev;
        if (median -> prev){
            median -> prev -> next = list;
        }
        median -> prev = list;
        if(median == head){
            head = list;
        } 
        if (size % 2 == 0){
            median = list;
        }
    }
    size++;
}
int LinkedList::removeFromMedian(){
    if(size == 0){
        cout<< "Could not remove from median."<<endl;
        return -1;
    }

    int data = median -> data;
    Node* remove = median;

    if(size == 1){
        head = tail = median = nullptr;
    } else {
        if (median -> prev){
            median -> prev -> next = median -> next;
        }
        if (median -> next){
            median -> next -> prev = median -> prev;
        }

        if(size % 2 == 1){
            median = median -> prev;
        } else {
            median = median -> next;
        }

        if (median == nullptr){
            head = tail = nullptr;
        }
    }
    size --;
    delete remove;
    return data;
}

int LinkedList::returnMedian(){
    if(size == 0) return -1;
    return median -> data;
}

void LinkedList::print(){
    Node* list = head;
    while(list != nullptr){
        list -> print();
        list = list->next;
        if(list != nullptr){
            cout << " ";
        }
    }
    cout << endl;
}