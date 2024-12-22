// Rawisara Chairat, rawisara@bu.edu
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "BST.h"

using namespace std;

BSTNode* BST::CreateNode(int data){
    BSTNode* New = new BSTNode(); // Create new memory for new node.
    New->data = data; // Set data
    New->left = NULL; // Initialize left side of the node
    New->right = NULL; // Initialize right side of the node
    // if(this->root == nullptr){
    //     this->root = New;
    // }
    return New;
}

BSTNode* BST::Insert(BSTNode* node, int data){
    if(node == NULL){
        return CreateNode(data);  
    }
    if (node->data > data){
        node->left = Insert(node->left, data); //Insert in the left subtree
    } else if (node->data < data){
        node->right = Insert(node->right, data); //Insert in the right subtree
    }
    return node;
}

bool BST::Search(BSTNode* node, int data){
    if(node == NULL){
        return false; // Return false if the data is not found in the BST
    }
    if(node->data == data){
        return true; // Return true if the data is found in the BST
    }
    else if (node->data > data){
        return Search(node->left, data); // Keep searching in the left subtree
    } else {
        return Search(node->right, data); // Keep searching in the right subtree
    }
}

int BST::CountNodes(BSTNode* root){
    if (root == NULL){
        return 0; // Base case
    }
    return 1 + CountNodes(root->left) + CountNodes(root->right); // Count node by calling the CountNodes function recursively.
}

void BST::CreateTree(string file){
    ifstream input;
    input.open(file);
    int data;

    if (!input) {
        return;
    }

    while (input >> data) {
        root = Insert(root, data); // Insert each data read from the file to the BST.
    }
    input.close();
}

int BST::height(BSTNode* node){
    if(node == NULL){
        return 0; //Base case
    } 
    
    // Keep counting by calling height function recursively and finding the maximum height between left and right subtrees.
    return 1 + max(height(node->left), height(node->right)); 
}

bool BST::isBalanced(BSTNode* node){
    if(node == NULL){
        return true;
    }
    int left_height = height(node->left);
    int right_height = height(node->right);

    // Check if the tree is balanced or not by comparing the height of the left and right subtree 
    if(abs(left_height-right_height)>1){ 
        return false;
    } else {
        return true;
    }
}






