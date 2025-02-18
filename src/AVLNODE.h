#ifndef AVLNODE_H
#define AVLNODE_H

#include "Student.h"

// The AVLNode class is a AVL tree
// Each node contains a pointer to a Student object, pointers to the left and right child nodes
//and the height 1 based hight 

class AVLNode {
public:
    Student* student;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(Student* student);
};

#endif