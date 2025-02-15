#include "AVLTree.h"
#include <regex>
#include <queue>
#include <iostream>
#include <iomanip>
// TODO how will I deal with 00000001 
// I may keep it as a string and a int
// when searching I will look for the string (for display and stops the 00000001 from being converted to 1)
// when inserting I will convert the string to an int
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
// funcion bug with 00000001 strores it as 1 first it is stored correctly but then it does not stay correct 
bool AVLTree::insert(const std::string& name , const std::string& ufid) {
    // Validate name: must not be empty and must contain only alphabetic characters and spaces.
    if (name.empty())
        return false;
    for (char c : name) {
        if (!std::isalpha(c) && c != ' ')
            return false;
    }

    // Validate UFID: must be exactly 8 digits.
    if (ufid.size() != 8)
        return false;
    for (char c : ufid) {
        if (!std::isdigit(c))
            return false;
    }

    int numericUfid = 0;
    for (char c : ufid) {
        numericUfid = numericUfid * 10 + (c - '0');
    }
 //   std::cout << "Debug: Inserting student " << name << " with ID " << numericUfid << std::endl;

    if (searchId(numericUfid) != "unsuccessful")
        return false;

    Student* student = new Student(name , numericUfid);
    root = insertHelper(root , student);


    return true;
}

AVLNode* AVLTree::insertHelper(AVLNode* node , Student* student) {
   // std::cout << "Debug: Inserting student " << student->name << " with ID " << student->gatorId << std::endl;
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
    return student ? student->name : "unsuccessful";
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

    // Get total number of nodes first
    std::vector<std::string> inorderList = printInorder();
    if (static_cast<size_t>(n) >= inorderList.size())
        return false;

    int currentIndex = 0;
    int targetId = -1;

    std::function<void(AVLNode*)> findNthNode = [&](AVLNode* node) {
        if (node == nullptr || targetId != -1)
            return;

        findNthNode(node->left);

        if (currentIndex == n) {
            targetId = node->student->gatorId;
            return;
        }
        currentIndex++;

        findNthNode(node->right);
        };

    findNthNode(root);

    if (targetId == -1)
        return false;

    return remove(targetId);
}