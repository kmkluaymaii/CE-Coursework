//
//  main.cpp
//  Linked_List_Median
//
//  Copyright Tali Moreshet. All rights reserved.
//

#include "LinkedList.h"

int main(int argc, const char * argv[]) {
    
    // LinkedList *myLL = new LinkedList();
    
    // myLL->insertToTail(1);
    // myLL->insertToTail(2);
    // myLL->insertToTail(3);

    // cout << "Linked list data: ";
    // myLL->print();
    // cout << "Linked list median element: " << myLL->returnMedian() << endl;
    
    // myLL->insertToMedian(10);
    // myLL->insertToMedian(20);
    
    // cout << "Linked list data: ";
    // myLL->print();
    // cout << "Linked list median element: " << myLL->returnMedian() << endl;
    
    // cout << "Removed tail: " << myLL->removeFromTail() << endl;
    // cout << "Linked list data: ";
    // myLL->print();
    // cout << "Linked list median element: " << myLL->returnMedian() << endl;
    
    // cout << "removed median: " << myLL->removeFromMedian() << endl;
    // cout << "Linked list data: ";
    // myLL->print();
    // cout << "Linked list median element: " << myLL->returnMedian() << endl;
    
    // cout << "Removed head: " << myLL->removeFromHead() << endl;
    // cout << "Linked list data: ";
    // myLL->print();
    // cout << "Linked list median element: " << myLL->returnMedian() << endl;

    LinkedList *myLL = new LinkedList();
    
            myLL->insertToTail(13);
            myLL->insertToHead(15);
            myLL->insertToMedian(14);

            myLL->insertToHead(10);
            myLL->insertToTail(20);
            myLL->insertToMedian(1);
            myLL->insertToMedian(2);
            myLL->insertToMedian(3);
            myLL->insertToMedian(9999);

            myLL->print();

            myLL->removeFromMedian();
            myLL->removeFromHead();
            myLL->removeFromTail();

            myLL->print();

            myLL->removeFromTail();
            myLL->removeFromTail();
            cout << myLL->removeFromTail() << endl;
            myLL->removeFromHead();

            myLL->print();

            myLL->removeFromHead();
            cout << myLL->removeFromMedian() << endl;



            delete myLL;
    
    return 0;
}
