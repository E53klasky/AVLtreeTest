#include <iostream>
#include "AVLTree.h"

int main() {
	AVLTree tree;
	int numCommands;
	std::cin >> numCommands;
	std::cin.ignore();

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