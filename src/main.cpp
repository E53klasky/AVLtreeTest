#include <iostream>
#include "AVLTree.h"

int main() {
	AVLTree tree;
	int numCommands;
	std::cin >> numCommands;
	std::cin.ignore();

	for (int i = 0; i < numCommands; i++) {
		std::string command = "";
		std::getline(std::cin , command);

		try {
			std::cout << "Debug: Processing command: '" << command << "'" << std::endl;
			if (command.substr(0 , 13) == "removeInorder") {
				std::cout << "Debug: Detected removeInorder command" << std::endl;
				// Extract the parameter starting at index 14 (skipping the space)
				std::string numStr = command.substr(14);
				std::cout << "Debug: numStr before trim: '" << numStr << "'" << std::endl;

				numStr.erase(0 , numStr.find_first_not_of(" \t\r\n"));
				numStr.erase(numStr.find_last_not_of(" \t\r\n") + 1);
				std::cout << "Debug: numStr after trim: '" << numStr << "'" << std::endl;

				try {
					int n = std::stoi(numStr);
					std::cout << "Debug: Parsed n = " << n << std::endl;
					bool result = tree.removeInorder(n);
					std::cout << "Debug: removeInorder result = " << (result ? "true" : "false") << std::endl;
					std::cout << (result ? "successful" : "unsuccessful") << std::endl;
				}
				catch (const std::exception& e) {
					std::cout << "Debug: Exception in number parsing: " << e.what() << std::endl;
					std::cout << "unsuccessful" << std::endl;
				}
			}
			else if (command.substr(0 , 6) == "remove") {
				std::cout << "Debug: Detected remove command" << std::endl;
				std::string numStr = command.substr(7);
				std::cout << "Debug: numStr before trim: '" << numStr << "'" << std::endl;

				numStr.erase(0 , numStr.find_first_not_of(" \t\r\n"));
				numStr.erase(numStr.find_last_not_of(" \t\r\n") + 1);
				std::cout << "Debug: numStr after trim: '" << numStr << "'" << std::endl;

				try {
					int id = std::stoi(numStr);
					std::cout << "Debug: Parsed id = " << id << std::endl;
					bool result = tree.remove(id);
					std::cout << "Debug: remove result = " << (result ? "true" : "false") << std::endl;
					std::cout << (result ? "successful" : "unsuccessful") << std::endl;
				}
				catch (const std::exception& e) {
					std::cout << "Debug: Exception in number parsing: " << e.what() << std::endl;
					std::cout << "unsuccessful" << std::endl;
				}
			}
			else if (command.substr(0 , 6) == "insert") {
				std::cout << "Debug: Detected insert command" << std::endl;
				size_t nameStart = command.find('"');
				size_t nameEnd = command.find('"' , nameStart + 1);
				std::string name = command.substr(nameStart + 1 , nameEnd - nameStart - 1);
				int id = std::stoi(command.substr(nameEnd + 2));

				std::cout << (tree.insert(name , id) ? "successful" : "unsuccessful") << std::endl;
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
			std::cout << "Debug: Main exception caught: " << e.what() << std::endl;
			std::cout << "unsuccessful" << std::endl;
		}
	}

	return 0;
}