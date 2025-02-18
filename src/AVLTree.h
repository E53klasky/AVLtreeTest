#ifndef AVLTREE_H
#define AVLTREE_H

#include <string>
#include <vector>
#include "AVLNODE.h"

class AVLTree {
private:
    AVLNode* root;

    int getHeight(AVLNode* node);
    int getBalance(AVLNode* node);
    // rotation functions 
    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);

    // helper to insert 
    AVLNode* insertHelper(AVLNode* node , Student* student);
    // find the min value in a subtree 
    AVLNode* minValueNode(AVLNode* node);
    // removes a node with the ufid
    AVLNode* removeHelper(AVLNode* root , int gatorId);

    // helpers for searchs
    void inorderHelper(AVLNode* root , std::vector<std::string>& result);
    void preorderHelper(AVLNode* root , std::vector<std::string>& result);
    void postorderHelper(AVLNode* root , std::vector<std::string>& result);

    Student* searchIdHelper(AVLNode* root , int gatorId);

    void searchNameHelper(AVLNode* root , const std::string& name , std::vector<int>& result);

    void getInorderIds(AVLNode* node , std::vector<int>& inorderIds);

public:
    AVLTree();
    // inserting and removing
    bool insert(const std::string& name , const std::string& gatorId);
    bool remove(int gatorId);
    bool removeInorder(int num);

    // searching
    std::string searchId(int gatorId);
    std::vector<int> searchName(const std::string& name);
    std::vector<std::string> printInorder();
    std::vector<std::string> printPreorder();
    std::vector<std::string> printPostorder();

    // returns the number of levels in the tree
    int printLevelCount();
};

#endif