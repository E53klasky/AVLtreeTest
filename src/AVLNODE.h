#ifndef AVLNODE_H
#define AVLNODE_H

#include "Student.h"

class AVLNode {
public:
    Student* student;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(Student* student);
};

#endif