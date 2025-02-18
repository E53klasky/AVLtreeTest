#include "AVLTree.h"
#include <regex>
#include <queue>
#include <iostream>
#include <iomanip>
// TODO: 
// 2. analzes time copmptetites 
// 3. check if there is a bug with 00000001 bc this goes to 1 can't insert it again
// check search method for stoi and test it and insertion 

// construtor 
AVLTree::AVLTree() : root(nullptr) {}

// get the height by recursively calling the node's height 
int AVLTree::getHeight(AVLNode* node) {
    if (node == nullptr) return 0;
    return node->height;
}

// claculates the balance factor by subtracting the difference between the hieghts of its left and right subtree
int AVLTree::getBalance(AVLNode* node) {
    if (node == nullptr) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// does a right rotation on the unbalanced node 
AVLNode* AVLTree::rightRotate(AVLNode* unbalancedNode) {
    AVLNode* leftChild = unbalancedNode->left;
    AVLNode* leftRightSubtree = leftChild->right;

    leftChild->right = unbalancedNode;
    unbalancedNode->left = leftRightSubtree;
    // updates the heights again 
    unbalancedNode->height = std::max(getHeight(unbalancedNode->left) , getHeight(unbalancedNode->right)) + 1;
    leftChild->height = std::max(getHeight(leftChild->left) , getHeight(leftChild->right)) + 1;

    return leftChild;
}

// left roation on unbalacned node 
AVLNode* AVLTree::leftRotate(AVLNode* unbalancedNode) {
    AVLNode* rightChild = unbalancedNode->right;
    AVLNode* rightLeftSubtree = rightChild->left;

    rightChild->left = unbalancedNode;
    unbalancedNode->right = rightLeftSubtree;
    // updaing the heights after  rotation 
    unbalancedNode->height = std::max(getHeight(unbalancedNode->left) , getHeight(unbalancedNode->right)) + 1;
    rightChild->height = std::max(getHeight(rightChild->left) , getHeight(rightChild->right)) + 1;

    return rightChild;
}

// finds the smallest value in a subtree
AVLNode* AVLTree::minValueNode(AVrotaionLNode* node) {
    AVLNode* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

// inserts a node into the tree but befroe that it checks to see if the value is vaild 
bool AVLTree::insert(const std::string& name , const std::string& ufId) {
    // Validate name: must not be empty and must contain only alphabetic characters and spaces.
    if (name.empty())
        return false;
    for (char c : name) {
        if (!std::isalpha(c) && c != ' ')
            return false;
    }

    // Validate UFID: must be exactly 8 digits.
    if (ufId.size() != 8)
        return false;
    for (char c : ufId) {
        if (!std::isdigit(c))
            return false;
    }

    int numericUfid = 0;
    for (char c : ufId) {
        numericUfid = numericUfid * 10 + (c - '0');
    }
 //   std::cout << "Debug: Inserting student " << name << " with ID " << numericUfid << std::endl;

    if (searchId(numericUfid) != "unsuccessful")
        return false;

    Student* student = new Student(name , numericUfid);
    root = insertHelper(root , student);

    return true;
}

// helper fucntion for insert which recursively inserts a studnet into the tree while maintaining balance
AVLNode* AVLTree::insertHelper(AVLNode* node , Student* student) {
   // std::cout << "Debug: Inserting student " << student->name << " with ID " << student->ufId << std::endl;
    if (node == nullptr)
        return new AVLNode(student);

    if (student->ufId < node->student->ufId)
        node->left = insertHelper(node->left , student);
    else if (student->ufId > node->student->ufId)
        node->right = insertHelper(node->right , student);
    else
        return node;
    // updates tehe hieght
    node->height = 1 + std::max(getHeight(node->left) , getHeight(node->right));
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && student->ufId < node->left->student->ufId)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && student->ufId > node->right->student->ufId)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && student->ufId > node->left->student->ufId) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && student->ufId < node->right->student->ufId) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// removes a student from the tree based on the ufid numner
bool AVLTree::remove(int ufId) {
    if (searchIdHelper(root , ufId) == nullptr)
        return false;
    root = removeHelper(root , ufId);
    return true;
}

// helper fucntion for the removing which recursivly call removes a node while making sure the tree is balanced 
AVLNode* AVLTree::removeHelper(AVLNode* root , int ufId) {
    if (root == nullptr)
        return root;
// located the node to delte
    if (ufId < root->student->ufId)
        root->left = removeHelper(root->left , ufId);
    else if (ufId > root->student->ufId)
        root->right = removeHelper(root->right , ufId);
    else {
        // if the node has one or no children case 
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
        // if the node has two children case 
        else {
            AVLNode* temp = minValueNode(root->right);
            Student* oldStudent = root->student;
            root->student = new Student(temp->student->name , temp->student->ufId);
            delete oldStudent;
            root->right = removeHelper(root->right , temp->student->ufId);
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

// search the tree for the id number
std::string AVLTree::searchId(int ufId) {
    Student* student = searchIdHelper(root , ufId);
    return student ? student->name : "unsuccessful";
}

// recursively searches for a stundet who has that ufID
Student* AVLTree::searchIdHelper(AVLNode* root , int ufId) {
    if (root == nullptr || root->student->ufId == ufId)
        return root ? root->student : nullptr;

    if (ufId < root->student->ufId)
        return searchIdHelper(root->left , ufId);

    return searchIdHelper(root->right , ufId);
}

// searches the tree for the name 
std::vector<int> AVLTree::searchName(const std::string& name) {
    std::vector<int> result;
    searchNameHelper(root , name , result);
    return result;
}

// recursively calls the function to see if the name exsits 
void AVLTree::searchNameHelper(AVLNode* root , const std::string& name , std::vector<int>& result) {
    if (root != nullptr) {
        if (root->student->name == name)
            result.push_back(root->student->ufId);
        searchNameHelper(root->left , name , result);
        searchNameHelper(root->right , name , result);
    }
}

// this does an inorder traversal of the tree and return a string vector of the sutdnets names in sorted order 
std::vector<std::string> AVLTree::printInorder() {
    std::vector<std::string> result;
    inorderHelper(root , result);
    return result;
}

// // this funciton recurviely does the inorder rtaversal (left, root, right)
void AVLTree::inorderHelper(AVLNode* root , std::vector<std::string>& result) {
    if (root != nullptr) {
        inorderHelper(root->left , result);
        result.push_back(root->student->name);
        inorderHelper(root->right , result);
    }
}

// this does a preorder traversal of the tree and returns a vector of student names 
std::vector<std::string> AVLTree::printPreorder() {
    std::vector<std::string> result;
    preorderHelper(root , result);
    return result;
}

// recursvely does the preorder traversal (root, left, right)
void AVLTree::preorderHelper(AVLNode* root , std::vector<std::string>& result) {
    if (root != nullptr) {
        result.push_back(root->student->name);
        preorderHelper(root->left , result);
        preorderHelper(root->right , result);
    }
}

// does is the postorder one traversals the tree and returns a vector of student names 
std::vector<std::string> AVLTree::printPostorder() {
    std::vector<std::string> result;
    postorderHelper(root , result);
    return result;
}

// this recursively traversal in postorder (left, right, root) 
void AVLTree::postorderHelper(AVLNode* root , std::vector<std::string>& result) {
    if (root != nullptr) {
        postorderHelper(root->left , result);
        postorderHelper(root->right , result);
        result.push_back(root->student->name);
    }
}

// get teh number of levels in the tree
int AVLTree::printLevelCount() {
    if (root == nullptr) return 0;

    int levels = 0;
    std::queue<AVLNode*> queue;
    queue.push(root);

    // does a (BFS) level -order traversal
    while (!queue.empty()) {
        // number of number at the level
        int size = queue.size();
        levels++;
        // traverses and process each node at the level 
        while (size--) {
            AVLNode* node = queue.front();
            queue.pop();

            if (node->left) queue.push(node->left);
            if (node->right) queue.push(node->right);
        }
    }

    return levels;
}

// removes the nth node in inorder traversal 
bool AVLTree::removeInorder(int n) {
    if (root == nullptr || n < 0)
        return false;

    // Get total number of nodes first
    std::vector<std::string> inorderList = printInorder();
    // if n is out of bounds then return false 
    if (static_cast<size_t>(n) >= inorderList.size())
        return false;

    int currentIndex = 0;
    int targetId = -1;

    // lambda function to perform an inorder traversal and fin the nth node's ufid
    std::function<void(AVLNode*)> findNthNode = [&](AVLNode* node) {
        // stops searching if the targer is already found 
        if (node == nullptr || targetId != -1)
            return;

        findNthNode(node->left);
        // if it matches the index n 
        // then i am storing the ufid 
        if (currentIndex == n) {
            targetId = node->student->ufId;
            return;
        }
        currentIndex++;

        findNthNode(node->right);
        };
    //starts to search for the nth node
    findNthNode(root);

    if (targetId == -1)
        return false;

    return remove(targetId);
}