//
//  BST.h
//  BST
//
//  Copyright Â© Tali Moreshet. All rights reserved.
//
//  This is an implementation of a binary search tree.

#ifndef BST_h
#define BST_h

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

/* A Binary Search Tree node */
struct BSTNode {
    int data;         // the key stored by this node
    BSTNode* left;    // the left child of this node
    BSTNode* right;   // the right child of this node
};


/* Implements a Binary Search Tree keyed by Integers  */
class BST {
public: 

    BSTNode* root = nullptr;          // the root of the binary search tree

    /* Creates and returns a new BSTNode with the provided data value */
    BSTNode* CreateNode(int data);

    /* Receives as input the root of a binary search tree or subtree, creates and inserts a new node 
       with the given value 'data'. Returns pointer to the root of the tree/subtree after the new node 
       was inserted. */
    BSTNode* Insert(BSTNode* node, int data);

    /* Searches for the provided 'data' in the subtrees of 'node'. If 'node' is the root of a tree, it 
       will search the entire tree and return true if the key is found, false if not. */
    bool Search(BSTNode* node, int data);

    /* Returns the number of nodes in the tree/subtree with the provided root. */
    int CountNodes(BSTNode* root);

    /* Receives a text file containing one integer per line, and inserts them into a binary search tree 
       one at a time. */
    void CreateTree(string file);

    /* Return the height of each side of the tree*/
    int height(BSTNode* node);
    
    /* Returns true if the binary tree/subtree with the provided root is balanced. That is, that for 
       any given node in the tree, the difference in heights of its left and right subtree differ by 
       one at most. */
    bool isBalanced(BSTNode* node);

};

#endif /* BST_h */



