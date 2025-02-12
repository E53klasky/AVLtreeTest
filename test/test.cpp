#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <string>

#include "../src/AVLTree.h"
#include "../src/Student.h"
#include "../src/AVLNODE.h"

using namespace std;

// TEST 1: Test the insert function
TEST_CASE("Test InvalufIDs Commands" , "[invalufIDs]") {
	AVLTree tree;
	// here I will be testing invialtined things that the user can input 
	// Test 1: InvalufIDs ID (negative)
	// Test 2: InvalufIDs ID (too large)
	// Test 3: InvalufIDs name (empty)
	// Test 4: InvalufIDs name (special characters)
	// Test 5: Duplicate ID
	SECTION("Invalid student IDs") {
		REQUIRE_FALSE(tree.insert("John" , -1));
		REQUIRE(tree.searchId(-1) == "unsuccessful");

		REQUIRE_FALSE(tree.insert("Alice-" , 45679999));
		REQUIRE(tree.searchId(45679999) == "unsuccessful");

		REQUIRE_FALSE(tree.insert("" , 12345));
		REQUIRE(tree.searchId(12345) == "unsuccessful");

		REQUIRE_FALSE(tree.insert("A!!y" , 12346));
		REQUIRE(tree.searchId(12346) == "unsuccessful");

		REQUIRE(tree.insert("Bob" , 12347));
		REQUIRE_FALSE(tree.insert("Bobby" , 12347));
		REQUIRE(tree.searchId(12347) == "Bob");
	}
}
// TEST 2: Test the ROTATIONS and printLevelCount function
TEST_CASE("Test AVL Rotations" , "[rotations]") {
	AVLTree tree;

	//  1. I am adding in the nodes here so that there should be a Left-left rotation	
	// 1. Also I am verify level count is correct after rotations
	SECTION("Left-Left Case") {

		REQUIRE(tree.insert("StudentA" , 30));
		REQUIRE(tree.insert("StudentB" , 20));
		REQUIRE(tree.insert("StudentC" , 10));


		std::vector<std::string> result = tree.printInorder();
		REQUIRE(result.size() == 3);


		REQUIRE(tree.printLevelCount() == 2);
	}


	// 2. I am adding in the nodes here so that there should be a Right-Right rotation
	// 2. Also I am verify level count is correct after rotations
	SECTION("Right-Right Case") {
		REQUIRE(tree.insert("StudentA" , 10));
		REQUIRE(tree.insert("StudentB" , 20));
		REQUIRE(tree.insert("StudentC" , 30));

		std::vector<std::string> result = tree.printInorder();
		REQUIRE(result.size() == 3);
		REQUIRE(tree.printLevelCount() == 2);
	}
	// 3. I am adding in the nodes here so that there should be a Left-Right rotation
	// 3. Also I am verify level count is correct after rotations
	SECTION("Left-Right Case") {
		REQUIRE(tree.insert("StudentA" , 30));
		REQUIRE(tree.insert("StudentB" , 10));
		REQUIRE(tree.insert("StudentC" , 20));

		std::vector<std::string> result = tree.printInorder();
		REQUIRE(result.size() == 3);
		REQUIRE(tree.printLevelCount() == 2);
	}
	// 4. I am adding in the nodes here so that there should be a Right-Left rotation
	//	4. Also I am verify level count is correct after rotations
	SECTION("Right-Left Case") {
		REQUIRE(tree.insert("StudentA" , 10));
		REQUIRE(tree.insert("StudentB" , 30));
		REQUIRE(tree.insert("StudentC" , 20));

		std::vector<std::string> result = tree.printInorder();
		REQUIRE(result.size() == 3);
		REQUIRE(tree.printLevelCount() == 2);
	}
}

// TEST CASE 3: this test is testing a longer random test to make sure that the tree is working correctly. It test inserion and deltion but others should be right if it pass. All of it i mean 
TEST_CASE("Large Scale Insert and Remove Test" , "[large_scale]") {
	AVLTree tree;
	std::vector<int> ufIDs;


	for (int i = 1; i <= 100; i++) {
		ufIDs.push_back(i);
	}

	for (int i = 0; i < 100; i++) {
		std::string name = "Student" + std::to_string(i);
		REQUIRE(tree.insert(name , ufIDs[i]));
	}


	std::vector<std::string> afterInsert = tree.printInorder();
	REQUIRE(afterInsert.size() == 100);

	// Removing 10 random nodes
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(ufIDs.begin() , ufIDs.end() , gen);

	for (int i = 0; i < 10; i++) {
		REQUIRE(tree.remove(ufIDs[i]));
	}


	std::vector<std::string> afterRemove = tree.printInorder();
	REQUIRE(afterRemove.size() == 90);

	for (int i = 0; i < 10; i++) {
		REQUIRE(tree.searchId(ufIDs[i]) == "unsuccessful");
	}
}


// TEST CASE 4: this test is testing the removeInorder function
TEST_CASE("Test removeInorder functionality" , "[remove_inorder]") {
	AVLTree tree;


	REQUIRE(tree.insert("StudentA" , 50));
	REQUIRE(tree.insert("StudentB" , 30));
	REQUIRE(tree.insert("StudentC" , 70));
	REQUIRE(tree.insert("StudentD" , 20));
	REQUIRE(tree.insert("StudentE" , 40));
	REQUIRE(tree.insert("StudentF" , 60));
	REQUIRE(tree.insert("StudentG" , 80));


	REQUIRE(tree.printInorder().size() == 7);


	SECTION("Remove from mufIDsdle of inorder traversal") {
		REQUIRE(tree.removeInorder(2));
		auto result = tree.printInorder();
		REQUIRE(result.size() == 6);
	}

	SECTION("Remove first node in inorder") {
		REQUIRE(tree.removeInorder(0));
		auto result = tree.printInorder();
		REQUIRE(result.size() == 6);
	}

	SECTION("Remove last node in inorder") {
		REQUIRE(tree.removeInorder(6));
		auto result = tree.printInorder();
		REQUIRE(result.size() == 6);
	}

	SECTION("InvalufIDs removeInorder operations") {

		REQUIRE_FALSE(tree.removeInorder(-1));


		REQUIRE_FALSE(tree.removeInorder(7));
		REQUIRE_FALSE(tree.removeInorder(100));

		REQUIRE(tree.printInorder().size() == 7);
	}
}


// TEST CASE 5: here is another long test to make sure it works correctly
TEST_CASE("Comprehensive Tree Operations Test" , "[comprehensive]") {
	AVLTree tree;

	SECTION("Mixed operations test") {
		// Phase 1: Insert 50 nodes with random order of IDs
		std::vector<int> ufIDss;
		for (int i = 1; i <= 50; i++) {
			ufIDss.push_back(i);
		}
		std::random_device rd;
		std::mt19937 gen(rd());
		std::shuffle(ufIDss.begin() , ufIDss.end() , gen);

		for (int i = 0; i < 50; i++) {
			std::string name = "Student" + std::to_string(ufIDss[i]);
			REQUIRE(tree.insert(name , ufIDss[i]));
		}

		REQUIRE(tree.printInorder().size() == 50);

		// Phase 2: Test search operations

		auto results = tree.searchName("Student1");
		REQUIRE(!results.empty());

		results = tree.searchName("NonexistentStudent");
		REQUIRE(results.empty());

		REQUIRE(tree.searchId(ufIDss[0]) != "unsuccessful");

		REQUIRE(tree.searchId(99999) == "unsuccessful");

		// Phase 3: Mixed removal operations

		for (int i = 0; i < 5; i++) {
			REQUIRE(tree.remove(ufIDss[i]));
		}
		REQUIRE(tree.printInorder().size() == 45);

		for (int i = 0; i < 5; i++) {
			REQUIRE(tree.removeInorder(i));
		}
		REQUIRE(tree.printInorder().size() == 40);

		// Phase 4: Verify tree properties
		int levelCount = tree.printLevelCount();
		REQUIRE(levelCount >= 5);
		REQUIRE(levelCount <= 7);

		// Phase 5: Final insertions and removals
		for (int i = 0; i < 5; i++) {
			REQUIRE_FALSE(tree.insert("DuplicateStudent" , ufIDss[i]));
		}

		for (int i = 51; i <= 60; i++) {
			REQUIRE(tree.insert("NewStudent" + std::to_string(i) , i));
		}

		std::vector<std::string> finalInorder = tree.printInorder();
		REQUIRE(finalInorder.size() == 50);

		REQUIRE(tree.printPreorder().size() == finalInorder.size());
		REQUIRE(tree.printPostorder().size() == finalInorder.size());
	}
}