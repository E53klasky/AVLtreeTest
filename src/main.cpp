#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <regex>
#include <functional>

class Student {
public:
	std::string name;
	int gatorId;

	Student(std::string n , int id) : name(n) , gatorId(id) {}
};

class AVLNode {
public:
	Student* student;
	AVLNode* left;
	AVLNode* right;
	int height;

	AVLNode(Student* s) : student(s) , left(nullptr) , right(nullptr) , height(1) {}
};

class AVLTree {
private:
	AVLNode* root;

	int getHeight(AVLNode* node) {
		if (node == nullptr) return 0;
		return node->height;
	}

	int getBalance(AVLNode* node) {
		if (node == nullptr) return 0;
		return getHeight(node->left) - getHeight(node->right);
	}

	AVLNode* rightRotate(AVLNode* y) {
		AVLNode* x = y->left;
		AVLNode* T2 = x->right;

		x->right = y;
		y->left = T2;

		y->height = std::max(getHeight(y->left) , getHeight(y->right)) + 1;
		x->height = std::max(getHeight(x->left) , getHeight(x->right)) + 1;

		return x;
	}

	AVLNode* leftRotate(AVLNode* x) {
		AVLNode* y = x->right;
		AVLNode* T2 = y->left;

		y->left = x;
		x->right = T2;

		x->height = std::max(getHeight(x->left) , getHeight(x->right)) + 1;
		y->height = std::max(getHeight(y->left) , getHeight(y->right)) + 1;

		return y;
	}

	AVLNode* insertHelper(AVLNode* node , Student* student) {
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

	AVLNode* minValueNode(AVLNode* node) {
		AVLNode* current = node;
		while (current->left != nullptr)
			current = current->left;
		return current;
	}

	AVLNode* removeHelper(AVLNode* root , int gatorId) {
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
				}
				else
					*root = *temp;
				delete temp;
			}
			else {
				AVLNode* temp = minValueNode(root->right);
				root->student = temp->student;
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

	void inorderHelper(AVLNode* root , std::vector<std::string>& result) {
		if (root != nullptr) {
			inorderHelper(root->left , result);
			result.push_back(root->student->name);
			inorderHelper(root->right , result);
		}
	}

	void preorderHelper(AVLNode* root , std::vector<std::string>& result) {
		if (root != nullptr) {
			result.push_back(root->student->name);
			preorderHelper(root->left , result);
			preorderHelper(root->right , result);
		}
	}

	void postorderHelper(AVLNode* root , std::vector<std::string>& result) {
		if (root != nullptr) {
			postorderHelper(root->left , result);
			postorderHelper(root->right , result);
			result.push_back(root->student->name);
		}
	}

	Student* searchIdHelper(AVLNode* root , int gatorId) {
		if (root == nullptr || root->student->gatorId == gatorId)
			return root ? root->student : nullptr;

		if (gatorId < root->student->gatorId)
			return searchIdHelper(root->left , gatorId);

		return searchIdHelper(root->right , gatorId);
	}

	void searchNameHelper(AVLNode* root , const std::string& name , std::vector<int>& result) {
		if (root != nullptr) {
			if (root->student->name == name)
				result.push_back(root->student->gatorId);
			searchNameHelper(root->left , name , result);
			searchNameHelper(root->right , name , result);
		}
	}

public:
	AVLTree() : root(nullptr) {}

	bool insert(std::string name , int gatorId) {
		// Validate input
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

	bool remove(int gatorId) {
		if (searchIdHelper(root , gatorId) == nullptr)
			return false;
		root = removeHelper(root , gatorId);
		return true;
	}

	std::string searchId(int gatorId) {
		Student* student = searchIdHelper(root , gatorId);
		return student ? student->name : "";
	}

	std::vector<int> searchName(const std::string& name) {
		std::vector<int> result;
		searchNameHelper(root , name , result);
		return result;
	}

	std::vector<std::string> printInorder() {
		std::vector<std::string> result;
		inorderHelper(root , result);
		return result;
	}

	std::vector<std::string> printPreorder() {
		std::vector<std::string> result;
		preorderHelper(root , result);
		return result;
	}

	std::vector<std::string> printPostorder() {
		std::vector<std::string> result;
		postorderHelper(root , result);
		return result;
	}

	int printLevelCount() {
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



// bug with this code. FIX
	bool removeInorder(int n) {
		std::vector<int> inorderIds;
		std::function<void(AVLNode*)> getInorderIds = [&](AVLNode* node) {
			if (node != nullptr) {
				getInorderIds(node->left);
				inorderIds.push_back(node->student->gatorId);
				getInorderIds(node->right);
			}
			};

		getInorderIds(root);

		if (n < 0 || static_cast<size_t>(n) >= inorderIds.size())
			return false;

		return remove(inorderIds[n]);
	}
};
int main() {
	AVLTree tree;
	int numCommands;
	std::cin >> numCommands;
	std::cin.ignore(); // Clear the newline

	for (int i = 0; i < numCommands; i++) {
		std::string command;
		std::getline(std::cin , command);

		try {
			if (command.substr(0 , 6) == "insert") {
				size_t nameStart = command.find('"');
				size_t nameEnd = command.find('"' , nameStart + 1);
				std::string name = command.substr(nameStart + 1 , nameEnd - nameStart - 1);
				int id = std::stoi(command.substr(nameEnd + 2));

				std::cout << (tree.insert(name , id) ? "successful" : "unsuccessful") << std::endl;
			}
			else if (command.substr(0 , 6) == "remove") {
				if (command.substr(0 , 12) == "removeInorder") {
					// Trim whitespace from the number part
					std::string numStr = command.substr(13);
					numStr.erase(0 , numStr.find_first_not_of(" \t\r\n"));
					numStr.erase(numStr.find_last_not_of(" \t\r\n") + 1);

					int n = std::stoi(numStr);
					std::cout << (tree.removeInorder(n) ? "successful" : "unsuccessful") << std::endl;
				}
				else {
					// Trim whitespace from the number part
					std::string numStr = command.substr(7);
					numStr.erase(0 , numStr.find_first_not_of(" \t\r\n"));
					numStr.erase(numStr.find_last_not_of(" \t\r\n") + 1);

					int id = std::stoi(numStr);
					std::cout << (tree.remove(id) ? "successful" : "unsuccessful") << std::endl;
				}
			}
			else if (command.substr(0 , 6) == "search") {
				if (command[7] == '"') {
					size_t nameStart = command.find('"');
					size_t nameEnd = command.find('"' , nameStart + 1);
					std::string name = command.substr(nameStart + 1 , nameEnd - nameStart - 1);

					auto ids = tree.searchName(name);
					if (ids.empty())
						std::cout << "unsuccessful" << std::endl;
					else
						for (int id : ids)
							std::cout << id << std::endl;
				}
				else {
					// Trim whitespace from the number part
					std::string numStr = command.substr(7);
					numStr.erase(0 , numStr.find_first_not_of(" \t\r\n"));
					numStr.erase(numStr.find_last_not_of(" \t\r\n") + 1);

					int id = std::stoi(numStr);
					std::string name = tree.searchId(id);
					std::cout << (name.empty() ? "unsuccessful" : name) << std::endl;
				}
			}
			else if (command == "printInorder") {
				auto names = tree.printInorder();
				if (!names.empty()) {
					for (size_t i = 0; i < names.size(); i++) {
						std::cout << names[i];
						if (i < names.size() - 1) std::cout << ", ";
					}
					std::cout << std::endl;
				}
			}
			else if (command == "printPreorder") {
				auto names = tree.printPreorder();
				if (!names.empty()) {
					for (size_t i = 0; i < names.size(); i++) {
						std::cout << names[i];
						if (i < names.size() - 1) std::cout << ", ";
					}
					std::cout << std::endl;
				}
			}
			else if (command == "printPostorder") {
				auto names = tree.printPostorder();
				if (!names.empty()) {
					for (size_t i = 0; i < names.size(); i++) {
						std::cout << names[i];
						if (i < names.size() - 1) std::cout << ", ";
					}
					std::cout << std::endl;
				}
			}
			else if (command == "printLevelCount") {
				std::cout << tree.printLevelCount() << std::endl;
			}
			else {
				std::cout << "unsuccessful" << std::endl;
			}
		}
		catch (const std::exception& e) {
		 // Handle any parsing errors
			std::cout << "unsuccessful" << std::endl;
		}
	}

	return 0;
}