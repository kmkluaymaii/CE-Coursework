//
//  main.cpp
//  BST
//
//  Copyright Â© Tali Moreshet. All rights reserved.
//

#include "BST.h"

using namespace std;

int main(int argc, const char * argv[]) {
    
    if (argc != 2)
    {
        cout << "Please supply a file name as input" << endl;
        return -1;
    }
   
    BST bst; 
    bst.CreateTree(argv[1]);
    
    cout << "Node 5 " << ((bst.Search(bst.root, 5)) ?  "is " : "is not ") << "in the tree" << endl;         // true
    // cout << "Node 8 " << ((bst.Search(bst.root, 8)) ?  "is " : "is not ") << "in the tree" << endl;         // true
    // cout << "Node 7 " << ((bst.Search(bst.root, 7)) ?  "is " : "is not ") << "in the tree" << endl;         // false
    // cout << "Node 0 " << ((bst.Search(bst.root, 0)) ?  "is " : "is not ") << "in the tree" << endl;         // false

    cout << "The tree contains " << bst.CountNodes(bst.root) << " nodes" << endl;                            // 6
    cout << "The tree " << ((bst.isBalanced(bst.root)) ? "is " : "is not ") << "balanced" << endl;          // true

    return 0;
}



