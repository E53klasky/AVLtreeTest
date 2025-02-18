#include <iostream>
#include <string>
#include <regex>
#include "AVLTree.h"
// test this and other things 
// test insert 00000001 again and searching for it 
int main() {
	// I am making the object and counting the number of commands here
	// I take the commands in with cin
	AVLTree tree;
	int numCommands;
	std::cin >> numCommands;
	std::cin.ignore();

	// loping thorugh all commands  
	for (int i = 0; i < numCommands; i++) {
		// Reads the command entered by the user
		std::string command;
		std::getline(std::cin, command);
		
		try {
			// Checks if the command is to remove an element in inorder 
			if (command.substr(0 , 13) == "removeInorder") {
				// Extracts the number from the command removing white space and trailing whitespace. removing it by name
				std::string numStr = command.substr(14);
				numStr.erase(0 , numStr.find_first_not_of(" \t\r\n")); 
				numStr.erase(numStr.find_last_not_of(" \t\r\n") + 1); 
	
				try {
					// Converts the number to an integer
					// see if the node can be removed 
					int num = std::stoi(numStr); 
					bool result = tree.removeInorder(num); 
					std::cout << (result ? "successful" : "unsuccessful") << std::endl; 
				}
				catch (const std::exception& e) {
					std::cout << "unsuccessful" << std::endl; 
				}
			}
			// Checks if the command is to remove an element by ID
			else if (command.substr(0 , 6) == "remove") {
				// Extracts the number from the command
				std::string numStr = command.substr(7);
				numStr.erase(0 , numStr.find_first_not_of(" \t\r\n"));
				numStr.erase(numStr.find_last_not_of(" \t\r\n") + 1);
				// sees if i can remove it by ID
				try {
					int ufid = std::stoi(numStr); 
					bool result = tree.remove(ufid); 
					std::cout << (result ? "successful" : "unsuccessful") << std::endl; 
				}
				catch (const std::exception& e) {
					std::cout << "unsuccessful" << std::endl; 
				}
			}
			// sees if i can insert a node
			else if (command.substr(0 , 6) == "insert") {
				// gets the name from the getting rid of the quotes 
				size_t nameStart = command.find('"');
				size_t nameEnd = command.find('"' , nameStart + 1);
				std::string name = command.substr(nameStart + 1 , nameEnd - nameStart - 1);
				std::string ufidStr = command.substr(nameEnd + 2);
				
				// calls insert and sees if it can printing out wether it was successful or not 
				std::cout << (tree.insert(name , ufidStr) ? "successful" : "unsuccessful") << std::endl;
			}
			// Checks if the command is to search
			else if (command.substr(0 , 6) == "search") {
				if (command[7] == '"') {
				
					// Getting the name from the command searching by name
					size_t nameStart = command.find('"');
					size_t nameEnd = command.find('"' , nameStart + 1);
					std::string name = command.substr(nameStart + 1 , nameEnd - nameStart - 1);
					// Searches for UFIDs associated with the name and outputs results
					auto ufids = tree.searchName(name);
					if (ufids.empty())
						std::cout << "unsuccessful" << std::endl;
					else {
						for (int ufid : ufids)
							std::cout << ufid << std::endl;
					}
				}
				else {
					// Extracts the UFID from the command and searches for the associated name searching by ufid 
					std::string numStr = command.substr(7);
					numStr.erase(0 , numStr.find_first_not_of(" \t\r\n"));
					numStr.erase(numStr.find_last_not_of(" \t\r\n") + 1);
					int ufid = std::stoi(numStr);
					std::string name = tree.searchId(ufid);
					std::cout << (name.empty() ? "unsuccessful" : name) << std::endl;
				}
			}
			// prints elements in inorder traversal
			else if (command == "printInorder") {
				auto names = tree.printInorder();
				if (!names.empty()) {
					for (size_t i = 0; i < names.size(); i++) {
						std::cout << names[i];
						if (i < names.size() - 1)
							std::cout << ", ";
					}
					std::cout << std::endl;
				}
			}
			// print elements in preorder traversal
			else if (command == "printPreorder") {
				auto names = tree.printPreorder();
				if (!names.empty()) {
					for (size_t i = 0; i < names.size(); i++) {
						std::cout << names[i];
						if (i < names.size() - 1)
							std::cout << ", ";
					}
					std::cout << std::endl;
				}
			}
			// print elements in postorder traversal
			else if (command == "printPostorder") {
				auto names = tree.printPostorder();
				if (!names.empty()) {
					for (size_t i = 0; i < names.size(); i++) {
						std::cout << names[i];
						if (i < names.size() - 1)
							std::cout << ", ";
					}
					std::cout << std::endl;
				}
			}
			// print the level of nodes at each level
			else if (command == "printLevelCount") {
				std::cout << tree.printLevelCount() << std::endl;
			}
		
			else {
				std::cout << "unsuccessful" << std::endl;
			}
		}
		catch (const std::exception& e) {
			// Catches any exceptions and outputs unsuccessful if an error occurs
			std::cout << "unsuccessful" << std::endl;
		}
	}
	return 0;
}
	