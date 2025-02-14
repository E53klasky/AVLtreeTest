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

    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* insertHelper(AVLNode* node , Student* student);
    AVLNode* minValueNode(AVLNode* node);
    AVLNode* removeHelper(AVLNode* root , int gatorId);

    void inorderHelper(AVLNode* root , std::vector<std::string>& result);
    void preorderHelper(AVLNode* root , std::vector<std::string>& result);
    void postorderHelper(AVLNode* root , std::vector<std::string>& result);

    Student* searchIdHelper(AVLNode* root , int gatorId);

    void searchNameHelper(AVLNode* root , const std::string& name , std::vector<int>& result);
    void getInorderIds(AVLNode* node , std::vector<int>& inorderIds);

public:
    AVLTree();

    bool insert(const std::string& name , const std::string& gatorId);
    bool remove(int gatorId);
    bool removeInorder(int num);

    std::string searchId(int gatorId);

    std::vector<int> searchName(const std::string& name);
    std::vector<std::string> printInorder();
    std::vector<std::string> printPreorder();
    std::vector<std::string> printPostorder();

    int printLevelCount();
};

#endif