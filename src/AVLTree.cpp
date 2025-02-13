#include "AVLTree.h"
#include <regex>
#include <queue>
#include <iostream>

AVLTree::AVLTree() : root(nullptr) {}

int AVLTree::getHeight(AVLNode* node) {
    if (node == nullptr) return 0;
    return node->height;
}

int AVLTree::getBalance(AVLNode* node) {
    if (node == nullptr) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

AVLNode* AVLTree::rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = std::max(getHeight(y->left) , getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left) , getHeight(x->right)) + 1;

    return x;
}

AVLNode* AVLTree::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = std::max(getHeight(x->left) , getHeight(x->right)) + 1;
    y->height = std::max(getHeight(y->left) , getHeight(y->right)) + 1;

    return y;
}

AVLNode* AVLTree::minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

/*
example from stepik
Node * insert(Node* root, int key)
{
    if(root==NULL)
    {
        Node* temp=new Node();
        temp->name=key;
        return temp;
    }
    if (key < root->name)
        root->left  = insert(root->left, key);
    else if (key > root->name)
        root->right = insert(root->right, key);

    return root;
}


*/

bool AVLTree::insert(std::string name , int gatorId) {
    if (gatorId < 10000000 || gatorId > 99999999)
        return false;

    std::regex namePattern("^[a-zA-Z ]+$");
    if (!std::regex_match(name , namePattern))
        return false;

    if (searchIdHelper(root , gatorId) != nullptr)
        return false;

    Student* student = new Student(name , gatorId);
    root = insertHelper(root , student);
    return true;
}

AVLNode* AVLTree::insertHelper(AVLNode* node , Student* student) {
    if (node == nullptr)
        return new AVLNode(student);

    if (student->gatorId < node->student->gatorId)
        node->left = insertHelper(node->left , student);
    else if (student->gatorId > node->student->gatorId)
        node->right = insertHelper(node->right , student);
    else
        return node;

    node->height = 1 + std::max(getHeight(node->left) , getHeight(node->right));
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && student->gatorId < node->left->student->gatorId)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && student->gatorId > node->right->student->gatorId)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && student->gatorId > node->left->student->gatorId) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && student->gatorId < node->right->student->gatorId) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

bool AVLTree::remove(int gatorId) {
    if (searchIdHelper(root , gatorId) == nullptr)
        return false;
    root = removeHelper(root , gatorId);
    return true;
}

AVLNode* AVLTree::removeHelper(AVLNode* root , int gatorId) {
    if (root == nullptr)
        return root;

    if (gatorId < root->student->gatorId)
        root->left = removeHelper(root->left , gatorId);
    else if (gatorId > root->student->gatorId)
        root->right = removeHelper(root->right , gatorId);
    else {
        if (root->left == nullptr || root->right == nullptr) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
                delete temp->student;
                delete temp;
            }
            else {
                Student* oldStudent = root->student;
                *root = *temp;
                delete oldStudent;
                delete temp;
            }
        }
        else {
            AVLNode* temp = minValueNode(root->right);
            Student* oldStudent = root->student;
            root->student = new Student(temp->student->name , temp->student->gatorId);
            delete oldStudent;
            root->right = removeHelper(root->right , temp->student->gatorId);
        }
    }

    if (root == nullptr)
        return root;

    root->height = 1 + std::max(getHeight(root->left) , getHeight(root->right));
    int balance = getBalance(root);

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

std::string AVLTree::searchId(int gatorId) {
    Student* student = searchIdHelper(root , gatorId);
    return student ? student->name : "";
}

Student* AVLTree::searchIdHelper(AVLNode* root , int gatorId) {
    if (root == nullptr || root->student->gatorId == gatorId)
        return root ? root->student : nullptr;

    if (gatorId < root->student->gatorId)
        return searchIdHelper(root->left , gatorId);

    return searchIdHelper(root->right , gatorId);
}

std::vector<int> AVLTree::searchName(const std::string& name) {
    std::vector<int> result;
    searchNameHelper(root , name , result);
    return result;
}

void AVLTree::searchNameHelper(AVLNode* root , const std::string& name , std::vector<int>& result) {
    if (root != nullptr) {
        if (root->student->name == name)
            result.push_back(root->student->gatorId);
        searchNameHelper(root->left , name , result);
        searchNameHelper(root->right , name , result);
    }
}

std::vector<std::string> AVLTree::printInorder() {
    std::vector<std::string> result;
    inorderHelper(root , result);
    return result;
}

void AVLTree::inorderHelper(AVLNode* root , std::vector<std::string>& result) {
    if (root != nullptr) {
        inorderHelper(root->left , result);
        result.push_back(root->student->name);
        inorderHelper(root->right , result);
    }
}

std::vector<std::string> AVLTree::printPreorder() {
    std::vector<std::string> result;
    preorderHelper(root , result);
    return result;
}

void AVLTree::preorderHelper(AVLNode* root , std::vector<std::string>& result) {
    if (root != nullptr) {
        result.push_back(root->student->name);
        preorderHelper(root->left , result);
        preorderHelper(root->right , result);
    }
}

std::vector<std::string> AVLTree::printPostorder() {
    std::vector<std::string> result;
    postorderHelper(root , result);
    return result;
}

void AVLTree::postorderHelper(AVLNode* root , std::vector<std::string>& result) {
    if (root != nullptr) {
        postorderHelper(root->left , result);
        postorderHelper(root->right , result);
        result.push_back(root->student->name);
    }
}

int AVLTree::printLevelCount() {
    if (root == nullptr) return 0;

    int levels = 0;
    std::queue<AVLNode*> q;
    q.push(root);

    while (!q.empty()) {
        int size = q.size();
        levels++;

        while (size--) {
            AVLNode* node = q.front();
            q.pop();

            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }

    return levels;
}

bool AVLTree::removeInorder(int n) {
    if (root == nullptr || n < 0)
        return false;

    std::cout << "Debug: Attempting to remove index " << n << std::endl;

    int currentIndex = 0;
    AVLNode* nodeToRemove = nullptr;

    std::function<void(AVLNode*)> findNthNode = [&](AVLNode* node) {
        if (node == nullptr || nodeToRemove != nullptr)
            return;

        findNthNode(node->left);

        std::cout << "Debug: Visiting node " << node->student->name
            << " at index " << currentIndex << std::endl;

        if (currentIndex++ == n) {
            nodeToRemove = node;
            std::cout << "Debug: Found node to remove: " << node->student->name << std::endl;
            return;
        }

        findNthNode(node->right);
        };

    findNthNode(root);

    if (nodeToRemove == nullptr) {
        std::cout << "Debug: No node found to remove" << std::endl;
        return false;
    }

    return remove(nodeToRemove->student->gatorId);
}