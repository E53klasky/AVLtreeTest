#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <string>
#include <functional>
#include <map>
#include <regex>
#include <random>
#include <set>
#include "../src/AVLTree.h"
#include "../src/Student.h"
#include "../src/AVLNODE.h"

using namespace std;

// Helper function to convert an integer to an alphabetical string (base-26, A–Z only).
std::string intToName(int num) {
	if (num == 0)
		return "A";
	std::string result;
	while (num > 0) {
		char letter = 'A' + (num % 26);
		result = letter + result;
		num /= 26;
	}
	return result;
}

// TEST 1: Test the insert function with invalid commands
TEST_CASE("Test InvalufIDs Commands" , "[invalufIDs]") {
	AVLTree tree;
	// Note: Valid UFIDs are 8-digit numbers.
	// Test 1: Invalid UFID (negative)
	REQUIRE_FALSE(tree.insert("John" , -1));
	REQUIRE(tree.searchId(-1) == "unsuccessful");

	// Test 2: Invalid name: contains a hyphen (only letters and spaces allowed)
	REQUIRE_FALSE(tree.insert("Alice-" , 45679999));
	REQUIRE(tree.searchId(45679999) == "unsuccessful");

	// Test 3: Empty name should be rejected (with a valid UFID)
	REQUIRE_FALSE(tree.insert("" , 12345678));
	REQUIRE(tree.searchId(12345678) == "unsuccessful");

	// Test 4: Invalid name: contains special characters
	REQUIRE_FALSE(tree.insert("A!!y" , 12345679));
	REQUIRE(tree.searchId(12345679) == "unsuccessful");

	// Test 5: Valid insertion and duplicate check
	REQUIRE(tree.insert("Bob" , 12345680));
	REQUIRE_FALSE(tree.insert("Bobby" , 12345680));
	REQUIRE(tree.searchId(12345680) == "Bob");
}

// TEST 2: Test the ROTATIONS and printLevelCount function
TEST_CASE("Test AVL Rotations" , "[rotations]") {
	AVLTree tree;

	// Left-Left Case: (Insert in descending order)
	SECTION("Left-Left Case") {
		// Using eight-digit numbers with an offset.
		REQUIRE(tree.insert("StudentA" , 30000000));
		REQUIRE(tree.insert("StudentB" , 20000000));
		REQUIRE(tree.insert("StudentC" , 10000000));

		vector<string> result = tree.printInorder();
		REQUIRE(result.size() == 3);
		REQUIRE(tree.printLevelCount() == 2);
	}

	// Right-Right Case: (Insert in ascending order)
	SECTION("Right-Right Case") {
		REQUIRE(tree.insert("StudentA" , 10000000));
		REQUIRE(tree.insert("StudentB" , 20000000));
		REQUIRE(tree.insert("StudentC" , 30000000));

		vector<string> result = tree.printInorder();
		REQUIRE(result.size() == 3);
		REQUIRE(tree.printLevelCount() == 2);
	}

	// Left-Right Case:
	SECTION("Left-Right Case") {
		REQUIRE(tree.insert("StudentA" , 30000000));
		REQUIRE(tree.insert("StudentB" , 10000000));
		REQUIRE(tree.insert("StudentC" , 20000000));

		vector<string> result = tree.printInorder();
		REQUIRE(result.size() == 3);
		REQUIRE(tree.printLevelCount() == 2);
	}

	// Right-Left Case:
	SECTION("Right-Left Case") {
		REQUIRE(tree.insert("StudentA" , 10000000));
		REQUIRE(tree.insert("StudentB" , 30000000));
		REQUIRE(tree.insert("StudentC" , 20000000));

		vector<string> result = tree.printInorder();
		REQUIRE(result.size() == 3);
		REQUIRE(tree.printLevelCount() == 2);
	}
}

// TEST CASE 3: Large Scale Insert and Remove Test
TEST_CASE("Large Scale Insert and Remove Test" , "[large_scale]") {
	AVLTree tree;
	vector<int> ufIDs;

	// Instead of 1-100, use 10000000 + i (i from 1 to 100)
	for (int i = 1; i <= 100; i++) {
		ufIDs.push_back(10000000 + i);
	}

	for (int i = 0; i < 100; i++) {
		// Generate a valid name that contains only letters.
		// Use our helper function to convert i to a letter string.
		string name = "Student" + intToName(i);
		REQUIRE(tree.insert(name , ufIDs[i]));
	}

	vector<string> afterInsert = tree.printInorder();
	REQUIRE(afterInsert.size() == 100);

	// Removing 10 random nodes
	random_device rd;
	mt19937 gen(rd());
	shuffle(ufIDs.begin() , ufIDs.end() , gen);

	for (int i = 0; i < 10; i++) {
		REQUIRE(tree.remove(ufIDs[i]));
	}

	vector<string> afterRemove = tree.printInorder();
	REQUIRE(afterRemove.size() == 90);

	for (int i = 0; i < 10; i++) {
		REQUIRE(tree.searchId(ufIDs[i]) == "unsuccessful");
	}
}

// TEST CASE 4: Test removeInorder functionality
TEST_CASE("Test removeInorder functionality" , "[remove_inorder]") {
	AVLTree tree;

	// Adjust the UFIDs by adding 10000000 so they are valid.
	REQUIRE(tree.insert("StudentA" , 10000050));
	REQUIRE(tree.insert("StudentB" , 10000030));
	REQUIRE(tree.insert("StudentC" , 10000070));
	REQUIRE(tree.insert("StudentD" , 10000020));
	REQUIRE(tree.insert("StudentE" , 10000040));
	REQUIRE(tree.insert("StudentF" , 10000060));
	REQUIRE(tree.insert("StudentG" , 10000080));

	REQUIRE(tree.printInorder().size() == 7);

	SECTION("Remove from middle of inorder traversal") {
		REQUIRE(tree.removeInorder(2)); // removes the node at index 2
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

	SECTION("Invalid removeInorder operations") {
		REQUIRE_FALSE(tree.removeInorder(-1));
		REQUIRE_FALSE(tree.removeInorder(7));
		REQUIRE_FALSE(tree.removeInorder(100));
		REQUIRE(tree.printInorder().size() == 7);
	}
}


// TEST CASE 5: Comprehensive Tree Operations Test
TEST_CASE("Comprehensive Tree Operations Test" , "[comprehensive]") {
	AVLTree tree;

	SECTION("Mixed operations test") {
		// Phase 1: Insert 50 nodes with random order of UFIDs
		vector<int> ufIDss;
		for (int i = 1; i <= 50; i++) {
			ufIDss.push_back(10000000 + i);
		}
		random_device rd;
		mt19937 gen(rd());
		shuffle(ufIDss.begin() , ufIDss.end() , gen);

		for (int i = 0; i < 50; i++) {
			// Generate a valid name (pure letters) using our helper
			string name = "Student" + intToName(ufIDss[i] % 1000);
			REQUIRE(tree.insert(name , ufIDss[i]));
		}
		REQUIRE(tree.printInorder().size() == 50);

		// Phase 2: Test search operations
		auto results = tree.searchName("Student" + intToName(ufIDss[0] % 1000));
		REQUIRE(!results.empty());

		results = tree.searchName("NonexistentStudent");
		REQUIRE(results.empty());

		REQUIRE(tree.searchId(ufIDss[0]) != "unsuccessful");
		REQUIRE(tree.searchId(99999999) == "unsuccessful");

		// Phase 3: Mixed removal operations
		// Remove first five using remove
		for (int i = 0; i < 5; i++) {
			REQUIRE(tree.remove(ufIDss[i]));
		}
		REQUIRE(tree.printInorder().size() == 45);

		// Then remove five using removeInorder
		for (int i = 0; i < 5; i++) {
			REQUIRE(tree.removeInorder(i));
		}
		REQUIRE(tree.printInorder().size() == 40);

		// Phase 4: Verify tree properties
		int levelCount = tree.printLevelCount();
		REQUIRE(levelCount >= 5);
		REQUIRE(levelCount <= 7);

		// Phase 5: Final insertions and removals
		// Instead of blindly testing duplicates on the first 5 ufIDss (which might have been removed),
		// iterate over ufIDss and test duplicate insertion only for ids that are currently present.
		int duplicatesTested = 0;
		for (size_t i = 0; i < ufIDss.size() && duplicatesTested < 5; i++) {
			if (tree.searchId(ufIDss[i]) != "unsuccessful") {
				// Since this id is found in the tree, a duplicate insertion should fail.
				REQUIRE_FALSE(tree.insert("DuplicateStudent" , ufIDss[i]));
				duplicatesTested++;
			}
		}

		for (int i = 51; i <= 60; i++) {
			REQUIRE(tree.insert("NewStudent" + intToName(i) , 10000000 + i));
		}

		vector<string> finalInorder = tree.printInorder();
		REQUIRE(finalInorder.size() == 50);

		REQUIRE(tree.printPreorder().size() == finalInorder.size());
		REQUIRE(tree.printPostorder().size() == finalInorder.size());
	}
}



// Additional Random Mixed Operations Test
TEST_CASE("Additional Random Mixed Operations Test" , "[random]") {
	AVLTree tree;

	// Generate a list of valid UFIDs (50 nodes)
	vector<int> ufIDs;
	for (int i = 1; i <= 50; i++) {
		ufIDs.push_back(10000000 + i);
	}

	// Shuffle the UFIDs for randomized insertion order:
	random_device rd;
	mt19937 gen(rd());
	shuffle(ufIDs.begin() , ufIDs.end() , gen);

	vector<int> insertedIDs;
	// Insert nodes using valid, alphabetic names.
	// We use the helper intToName (already defined in test.cpp) to generate a valid name.
	for (int id : ufIDs) {
		string name = "R" + intToName(id % 100);  // This produces names like "RA", "RB", etc.
		if (tree.insert(name , id))
			insertedIDs.push_back(id);
	}
	// Ensure the in-order list size matches the number of inserted nodes.
	auto inorder = tree.printInorder();
	CAPTURE(insertedIDs.size());
	REQUIRE(inorder.size() == insertedIDs.size());

	// Perform random removals using tree.remove (by id) if nodes exist.
	if (!insertedIDs.empty()) {
		shuffle(insertedIDs.begin() , insertedIDs.end() , gen);
		int numToRemove = min(5 , (int)insertedIDs.size());
		for (int i = 0; i < numToRemove; i++) {
			int idToRemove = insertedIDs[i];
			// Removal should succeed.
			REQUIRE(tree.remove(idToRemove));
			// After removal, searching for the id should yield "unsuccessful".
			REQUIRE(tree.searchId(idToRemove) == "unsuccessful");
		}
	}

	// If the tree is not empty, perform an additional random removal using removeInorder.
	int treeSize = tree.printInorder().size();
	if (treeSize > 0) {
		uniform_int_distribution<> dist(0 , treeSize - 1);
		int randomIndex = dist(gen);
		bool removed = tree.removeInorder(randomIndex);
		REQUIRE(removed);
	}

	// Additional Searches: Check that for a random node still in the tree, search returns a valid name.
	vector<int> remainingIDs;
	for (int id : ufIDs) {
		if (tree.searchId(id) != "unsuccessful")
			remainingIDs.push_back(id);
	}
	if (!remainingIDs.empty()) {
		uniform_int_distribution<> dist2(0 , remainingIDs.size() - 1);
		int randomRemaining = remainingIDs[dist2(gen)];
		string foundName = tree.searchId(randomRemaining);
		CAPTURE(randomRemaining , foundName);
		REQUIRE(foundName != "unsuccessful");
	}

	// Finally, ensure the tree's level count is valid.
	REQUIRE(tree.printLevelCount() >= 1);
}



// Additional Random Comprehensive Full Test
TEST_CASE("Random Comprehensive Full Test" , "[random_full]") {
	AVLTree tree;
	// External record mapping UFID to name (acting as our "golden copy")
	std::map<int , std::string> externalRecord;

	// Random generators
	std::random_device rd;
	std::mt19937 gen(rd());
	// UFID range: small range for testing (inclusive)
	std::uniform_int_distribution<int> ufidDist(10000000 , 10001000);
	// Operation distribution: 1-4: insert, 5-7: remove by id, 8-9: removeInorder, 10: search.
	std::uniform_int_distribution<int> opDist(1 , 10);

	// Lambda to generate a random alphabetic name (length between 3 and 8)
	auto randomName = [&gen]() -> std::string {
		std::uniform_int_distribution<int> lenDist(3 , 8);
		int len = lenDist(gen);
		std::string s;
		for (int i = 0; i < len; i++) {
			std::uniform_int_distribution<int> charDist(0 , 25);
			char c = 'A' + charDist(gen);
			s.push_back(c);
		}
		return s;
		};

		// Run a series of 100 random operations
	for (int i = 0; i < 100; i++) {
		int op = opDist(gen);
		if (op <= 4) {
			// Insertion: generate a random UFID and random name.
			int id = ufidDist(gen);
			std::string name = randomName();
			bool inserted = tree.insert(name , id);
			if (externalRecord.count(id)) {
				// Already exists: insertion must fail.
				REQUIRE_FALSE(inserted);
			}
			else {
				REQUIRE(inserted);
				externalRecord[id] = name;
			}
		}
		else if (op <= 7) {
			// Removal by id:
			if (!externalRecord.empty()) {
				// Choose a random element from externalRecord:
				auto it = externalRecord.begin();
				std::advance(it , gen() % externalRecord.size());
				int id = it->first;
				bool removed = tree.remove(id);
				REQUIRE(removed);
				externalRecord.erase(id);
			}
			else {
			 // If the tree is empty, removal should fail.
				int id = ufidDist(gen);
				REQUIRE_FALSE(tree.remove(id));
			}
		}
		else if (op <= 9) {
			// Removal by removeInorder:
			int treeSize = tree.printInorder().size();
			if (treeSize > 0) {
				// Compute expected inorder ordering from externalRecord.
				// Since externalRecord is a sorted map, its keys are in ascending order.
				std::vector<int> sortedIDs;
				std::vector<std::string> expectedNames;
				for (const auto& entry : externalRecord) {
					sortedIDs.push_back(entry.first);
					expectedNames.push_back(entry.second);
				}
				// Verify that current inorder (which returns names) matches expectations.
				std::vector<std::string> actualNames = tree.printInorder();
				REQUIRE(actualNames.size() == expectedNames.size());
				for (size_t j = 0; j < expectedNames.size(); j++) {
					REQUIRE(actualNames[j] == expectedNames[j]);
				}
				// Now choose a random index in [0, treeSize-1] for removal.
				std::uniform_int_distribution<int> indexDist(0 , treeSize - 1);
				int index = indexDist(gen);
				// The UFID corresponding to this inorder position is:
				int removedID = sortedIDs[index];
				bool removed = tree.removeInorder(index);
				REQUIRE(removed);
				externalRecord.erase(removedID);
			}
			else {
			 // Tree empty; removeInorder should fail.
				REQUIRE_FALSE(tree.removeInorder(0));
			}
		}
		else {
			// Search operation: choose a random UFID and check searchId.
			int id = ufidDist(gen);
			std::string res = tree.searchId(id);
			if (externalRecord.count(id)) {
				// It should find the student.
				REQUIRE(res == externalRecord[id]);
			}
			else {
				REQUIRE(res == "unsuccessful");
			}
		}
		// In each iteration, check that the tree's traversal sizes are consistent.
		std::vector<std::string> inord = tree.printInorder();
		REQUIRE(inord.size() == externalRecord.size());
		REQUIRE(tree.printPreorder().size() == inord.size());
		REQUIRE(tree.printPostorder().size() == inord.size());
		if (!externalRecord.empty())
			REQUIRE(tree.printLevelCount() >= 1);
	}

	// Final consistency check:
	// Rebuild expected inorder ordering from externalRecord.
	std::vector<int> sortedIDs;
	std::vector<std::string> expectedNames;
	for (const auto& entry : externalRecord) {
		sortedIDs.push_back(entry.first);
		expectedNames.push_back(entry.second);
	}
	std::vector<std::string> finalInorder = tree.printInorder();
	REQUIRE(expectedNames.size() == finalInorder.size());
	for (size_t i = 0; i < expectedNames.size(); i++) {
		REQUIRE(finalInorder[i] == expectedNames[i]);
	}
}


// Additional Test: Empty Tree Operations Test
TEST_CASE("Empty Tree Operations Test" , "[empty_tree]") {
	AVLTree tree;
	// The tree should not have any nodes.
	REQUIRE(tree.printInorder().empty());
	REQUIRE(tree.printPreorder().empty());
	REQUIRE(tree.printPostorder().empty());
	REQUIRE(tree.printLevelCount() == 0);

	// Removal operations should fail on an empty tree.
	REQUIRE_FALSE(tree.remove(10000000));
	REQUIRE_FALSE(tree.removeInorder(0));

	// Searching should yield no results.
	REQUIRE(tree.searchId(10000000) == "unsuccessful");
	REQUIRE(tree.searchName("AnyName").empty());
}

// Additional Test: Single Node Operations Test
TEST_CASE("Single Node Operations Test" , "[single_node]") {
	AVLTree tree;
	int ufid = 12345678;
	std::string name = "Alice"; // valid name (letters only)
	REQUIRE(tree.insert(name , ufid));

	// Traversals should reflect one single node.
	auto inorder = tree.printInorder();
	REQUIRE(inorder.size() == 1);
	REQUIRE(inorder[0] == name);
	REQUIRE(tree.printPreorder().size() == 1);
	REQUIRE(tree.printPostorder().size() == 1);
	REQUIRE(tree.printLevelCount() == 1);

	// Searching works as expected.
	REQUIRE(tree.searchId(ufid) == name);

	// Duplicate insertion should fail.
	REQUIRE_FALSE(tree.insert("Bob" , ufid));

	// Removing the node should succeed.
	REQUIRE(tree.remove(ufid));
	// Tree must be empty after removal.
	REQUIRE(tree.printInorder().empty());
	REQUIRE(tree.searchId(ufid) == "unsuccessful");
}

// Additional Test: Stress Test with Many Random Operations
TEST_CASE("Stress Test with Many Random Operations" , "[stress]") {
	AVLTree tree;
	// External record to track what should be in the tree.
	std::map<int , std::string> record;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> ufidDist(10000000 , 10010000);

	// Lambda to generate a random name (3 to 6 letters).
	auto randomName = [&gen]() -> std::string {
		std::uniform_int_distribution<int> lenDist(3 , 6);
		int len = lenDist(gen);
		std::string s;
		for (int i = 0; i < len; i++) {
			std::uniform_int_distribution<int> charDist(0 , 25);
			s.push_back('A' + charDist(gen));
		}
		return s;
		};

		// Perform 500 random operations.
	std::uniform_int_distribution<int> opDist(1 , 4);
	for (int i = 0; i < 500; ++i) {
		int op = opDist(gen);
		int id = ufidDist(gen);
		if (op == 1) {  // Insert
			std::string name = randomName();
			bool inserted = tree.insert(name , id);
			if (record.find(id) == record.end()) {
				REQUIRE(inserted);
				record[id] = name;
			}
			else {
			 // Duplicate insertion should fail.
				REQUIRE_FALSE(inserted);
			}
		}
		else if (op == 2) {  // Remove by id
			if (!record.empty()) {
				auto it = record.begin();
				std::advance(it , gen() % record.size());
				int removeID = it->first;
				bool removed = tree.remove(removeID);
				REQUIRE(removed);
				record.erase(removeID);
			}
			else {
				REQUIRE_FALSE(tree.remove(id));
			}
		}
		else if (op == 3) {  // removeInorder
			int size = tree.printInorder().size();
			if (size > 0) {
				std::uniform_int_distribution<int> indexDist(0 , size - 1);
				int idx = indexDist(gen);
				// Reconstruct sorted order from the record:
				std::vector<int> sortedIDs;
				for (auto& entry : record)
					sortedIDs.push_back(entry.first);
				std::sort(sortedIDs.begin() , sortedIDs.end());
				int targetID = sortedIDs[idx];
				bool removed = tree.removeInorder(idx);
				REQUIRE(removed);
				record.erase(targetID);
			}
			else {
				REQUIRE_FALSE(tree.removeInorder(0));
			}
		}
		else if (op == 4) { // Search
			std::string res = tree.searchId(id);
			if (record.find(id) != record.end()) {
				REQUIRE(res == record[id]);
			}
			else {
				REQUIRE(res == "unsuccessful");
			}
		}
		// Consistency checks after each operation.
		REQUIRE(tree.printInorder().size() == record.size());
		REQUIRE(tree.printPreorder().size() == record.size());
		REQUIRE(tree.printPostorder().size() == record.size());
		if (record.empty())
			REQUIRE(tree.printLevelCount() == 0);
		else
			REQUIRE(tree.printLevelCount() >= 1);
	}
}

// Additional Test: Traversal Integrity Test
TEST_CASE("Traversal Integrity Test" , "[traversal]") {
	AVLTree tree;
	// Insert nodes in a controlled sorted order.
	std::vector<int> ufIDs = { 10000010, 10000020, 10000030, 10000040, 10000050 };
	for (int id : ufIDs) {
		// Use intToName to generate a valid alphabetical suffix.
		std::string name = "Student" + intToName(id % 100);
		REQUIRE(tree.insert(name , id));
	}

	// Check that inorder traversal is in ascending order.
	auto inorder = tree.printInorder();
	REQUIRE(inorder.size() == ufIDs.size());

	// Check that preorder and postorder have the same number of nodes.
	auto preorder = tree.printPreorder();
	auto postorder = tree.printPostorder();
	REQUIRE(preorder.size() == ufIDs.size());
	REQUIRE(postorder.size() == ufIDs.size());

	// Now remove all nodes one by one using removeInorder.
	while (!tree.printInorder().empty()) {
		int currentSize = tree.printInorder().size();
		REQUIRE(tree.removeInorder(0));
		int newSize = tree.printInorder().size();
		REQUIRE(newSize == currentSize - 1);
	}
	// After all removals, the tree should be empty.
	REQUIRE(tree.printInorder().empty());
	REQUIRE(tree.printPreorder().empty());
	REQUIRE(tree.printPostorder().empty());
	REQUIRE(tree.printLevelCount() == 0);
}



// Additional Test: Boundary Conditions Test
TEST_CASE("Boundary Conditions Test" , "[boundary]") {
	AVLTree tree;
	// Valid lower boundary.
	REQUIRE(tree.insert("MinEdge" , 10000000));
	// Valid upper boundary.
	REQUIRE(tree.insert("MaxEdge" , 99999999));
	// Below allowed range should fail.
	REQUIRE_FALSE(tree.insert("BelowMin" , 9999999));
	// Above allowed range should fail.
	REQUIRE_FALSE(tree.insert("AboveMax" , 100000000));

	// Search must reflect those results.
	REQUIRE(tree.searchId(10000000) == "MinEdge");
	REQUIRE(tree.searchId(99999999) == "MaxEdge");
	REQUIRE(tree.searchId(9999999) == "unsuccessful");
	REQUIRE(tree.searchId(100000000) == "unsuccessful");

	// Traversal should show 2 nodes, in ascending gatorID order.
	auto inorder = tree.printInorder();
	REQUIRE(inorder.size() == 2);
}

// Additional Test: Rotation Specific Tests
TEST_CASE("Rotation Specific Tests" , "[rotation_specific]") {
	// Left-Left Rotation Test
	AVLTree treeLL;
	REQUIRE(treeLL.insert("C" , 30000000));
	REQUIRE(treeLL.insert("B" , 20000000));
	REQUIRE(treeLL.insert("A" , 10000000));
	std::vector<std::string> expectedLL = { "A", "B", "C" };
	REQUIRE(treeLL.printInorder() == expectedLL);

	// Right-Right Rotation Test
	AVLTree treeRR;
	REQUIRE(treeRR.insert("A" , 10000000));
	REQUIRE(treeRR.insert("B" , 20000000));
	REQUIRE(treeRR.insert("C" , 30000000));
	std::vector<std::string> expectedRR = { "A", "B", "C" };
	REQUIRE(treeRR.printInorder() == expectedRR);

	// Left-Right Rotation Test
	AVLTree treeLR;
	REQUIRE(treeLR.insert("C" , 30000000));
	REQUIRE(treeLR.insert("A" , 10000000));
	REQUIRE(treeLR.insert("B" , 20000000));
	std::vector<std::string> expectedLR = { "A", "B", "C" };
	REQUIRE(treeLR.printInorder() == expectedLR);

	// Right-Left Rotation Test
	AVLTree treeRL;
	REQUIRE(treeRL.insert("A" , 10000000));
	REQUIRE(treeRL.insert("C" , 30000000));
	REQUIRE(treeRL.insert("B" , 20000000));
	std::vector<std::string> expectedRL = { "A", "B", "C" };
	REQUIRE(treeRL.printInorder() == expectedRL);
}

// Additional Test: Sequential Removal Test (Using removeInorder repeatedly)
TEST_CASE("Sequential Removal Test" , "[sequential_removal]") {
	AVLTree tree;
	// Insert 50 nodes with increasing order, valid names via intToName.
	for (int i = 1; i <= 50; i++) {
		int id = 10000000 + i;
		std::string name = "Node" + intToName(i % 26);
		REQUIRE(tree.insert(name , id));
	}
	auto inorder = tree.printInorder();
	REQUIRE(inorder.size() == 50);

	// Repeatedly remove the first node (smallest) using removeInorder.
	while (!tree.printInorder().empty()) {
		int sizeBefore = tree.printInorder().size();
		REQUIRE(tree.removeInorder(0));
		int sizeAfter = tree.printInorder().size();
		REQUIRE(sizeAfter == sizeBefore - 1);
	}
	// The tree must be empty now.
	REQUIRE(tree.printInorder().empty());
	REQUIRE(tree.printLevelCount() == 0);
}

// Additional Test: Massive Random Operations Test
TEST_CASE("Massive Random Operations Test" , "[massive_random]") {
	AVLTree tree;
	std::map<int , std::string> record;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> ufidDist(10000000 , 10020000);

	// Lambda: Random alphabetic name (length 5 to 10).
	auto randomName = [&gen]() -> std::string {
		std::uniform_int_distribution<int> lenDist(5 , 10);
		int len = lenDist(gen);
		std::string s;
		for (int i = 0; i < len; i++) {
			std::uniform_int_distribution<int> charDist(0 , 25);
			s.push_back('A' + charDist(gen));
		}
		return s;
		};

	std::uniform_int_distribution<int> opDist(1 , 4);
	for (int i = 0; i < 1000; ++i) {
		int op = opDist(gen);
		int id = ufidDist(gen);
		if (op == 1) { // Insert
			std::string name = randomName();
			bool inserted = tree.insert(name , id);
			if (record.find(id) == record.end()) {
				REQUIRE(inserted);
				record[id] = name;
			}
			else {
				REQUIRE_FALSE(inserted);
			}
		}
		else if (op == 2) { // Remove by id
			if (record.empty()) {
				REQUIRE_FALSE(tree.remove(id));
			}
			else {
				auto it = record.begin();
				std::advance(it , gen() % record.size());
				int removeId = it->first;
				bool removed = tree.remove(removeId);
				REQUIRE(removed);
				record.erase(removeId);
			}
		}
		else if (op == 3) { // removeInorder
			int size = tree.printInorder().size();
			if (size > 0) {
				std::uniform_int_distribution<int> indexDist(0 , size - 1);
				int index = indexDist(gen);
				// Build sorted vector of ids from the record (map is already sorted by key)
				std::vector<int> sortedIDs;
				for (auto& pair : record)
					sortedIDs.push_back(pair.first);
				int targetId = sortedIDs[index];
				bool removed = tree.removeInorder(index);
				REQUIRE(removed);
				record.erase(targetId);
			}
			else {
				REQUIRE_FALSE(tree.removeInorder(0));
			}
		}
		else { // Search
			std::string res = tree.searchId(id);
			if (record.count(id)) {
				REQUIRE(res == record[id]);
			}
			else {
				REQUIRE(res == "unsuccessful");
			}
		}
		// Ensure traversals sizes are consistent.
		auto inord = tree.printInorder();
		REQUIRE(inord.size() == record.size());
		REQUIRE(tree.printPreorder().size() == record.size());
		REQUIRE(tree.printPostorder().size() == record.size());
		if (record.empty())
			REQUIRE(tree.printLevelCount() == 0);
		else
			REQUIRE(tree.printLevelCount() >= 1);
	}
	// Final consistency check.
	auto inorderFinal = tree.printInorder();
	REQUIRE(inorderFinal.size() == record.size());
}

// Additional Test: Duplicate Insertion Test
TEST_CASE("Duplicate Insertion Test" , "[duplicate]") {
	AVLTree tree;
	int id = 10000001;
	std::string name = "Unique";
	REQUIRE(tree.insert(name , id));
	// Attempt duplicate insertions many times.
	for (int i = 0; i < 10; i++) {
		REQUIRE_FALSE(tree.insert("Duplicate" , id));
	}
	// The tree should still hold only one node.
	auto inorder = tree.printInorder();
	REQUIRE(inorder.size() == 1);
}

// Additional Test: Large Scale Sequential Insertions Test
TEST_CASE("Large Scale Sequential Insertions Test" , "[large_inserts]") {
	AVLTree tree;
	const int numNodes = 200;
	// Insert nodes with sequential UFIDs and valid names.
	for (int i = 1; i <= numNodes; i++) {
		int ufid = 10000000 + i;
		std::string name = "LSI" + intToName(i % 100);
		bool inserted = tree.insert(name , ufid);
		REQUIRE(inserted);
		// After each insertion, the inorder size should equal the current count.
		REQUIRE(tree.printInorder().size() == static_cast<size_t>(i));
	}
	// Final check.
	REQUIRE(tree.printInorder().size() == numNodes);
}

// Additional Test: Large Scale Sequential Removals Test
TEST_CASE("Large Scale Sequential Removals Test" , "[large_removals]") {
	AVLTree tree;
	const int numNodes = 200;
	std::vector<int> ufids;
	for (int i = 1; i <= numNodes; i++) {
		int ufid = 10000000 + i;
		ufids.push_back(ufid);
		std::string name = "LSR" + intToName(i % 100);
		REQUIRE(tree.insert(name , ufid));
	}
	// Sequentially remove the smallest element using removeInorder(0).
	for (int i = 0; i < numNodes; i++) {
		size_t currentSize = tree.printInorder().size();
		REQUIRE(tree.removeInorder(0));
		REQUIRE(tree.printInorder().size() == currentSize - 1);
	}
	// The tree must now be empty.
	REQUIRE(tree.printInorder().empty());
	REQUIRE(tree.printLevelCount() == 0);
}

// Additional Test: Mixed Operation Count Test
TEST_CASE("Mixed Operation Count Test" , "[mixed_ops]") {
	AVLTree tree;
	std::map<int , std::string> record;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> ufidDist(10000000 , 10005000);
	std::uniform_int_distribution<int> opDist(1 , 4);

	const int numOps = 500;
	for (int i = 0; i < numOps; i++) {
		int op = opDist(gen);
		int id = ufidDist(gen);
		if (op == 1) { // Insertion.
			std::string name = "MOC" + intToName(i % 100);
			bool inserted = tree.insert(name , id);
			if (record.find(id) == record.end()) {
				REQUIRE(inserted);
				record[id] = name;
			}
			else {
				REQUIRE_FALSE(inserted);
			}
		}
		else if (op == 2) { // Removal by id.
			if (!record.empty()) {
				auto it = record.begin();
				std::advance(it , gen() % record.size());
				int removeId = it->first;
				bool removed = tree.remove(removeId);
				REQUIRE(removed);
				record.erase(removeId);
			}
			else {
				REQUIRE_FALSE(tree.remove(id));
			}
		}
		else if (op == 3) { // removeInorder.
			size_t sizeBefore = tree.printInorder().size();
			if (sizeBefore > 0) {
				std::uniform_int_distribution<int> indexDist(0 , sizeBefore - 1);
				int idx = indexDist(gen);
				// Build sorted vector from record keys.
				std::vector<int> sortedIDs;
				for (auto& entry : record)
					sortedIDs.push_back(entry.first);
				int target = sortedIDs[idx];
				bool removed = tree.removeInorder(idx);
				REQUIRE(removed);
				record.erase(target);
			}
			else {
				REQUIRE_FALSE(tree.removeInorder(0));
			}
		}
		else { // Search.
			std::string res = tree.searchId(id);
			if (record.count(id)) {
				REQUIRE(res == record[id]);
			}
			else {
				REQUIRE(res == "unsuccessful");
			}
		}
		// Consistency checks.
		size_t inordSize = tree.printInorder().size();
		REQUIRE(inordSize == record.size());
		REQUIRE(tree.printPreorder().size() == inordSize);
		REQUIRE(tree.printPostorder().size() == inordSize);
		if (record.empty())
			REQUIRE(tree.printLevelCount() == 0);
		else
			REQUIRE(tree.printLevelCount() >= 1);
	}
}

// Additional Test: Duplicate and Re-Insert Test
TEST_CASE("Duplicate and Re-Insert Test" , "[dup_reinsert]") {
	AVLTree tree;
	const int testId = 10000001;
	std::string originalName = "Original";
	REQUIRE(tree.insert(originalName , testId));
	// Attempt duplicate insertions 100 times.
	for (int i = 0; i < 100; i++) {
		REQUIRE_FALSE(tree.insert("Duplicate" + intToName(i) , testId));
	}
	// Remove the original node.
	REQUIRE(tree.remove(testId));
	// Now re-insertion should succeed.
	std::string newName = "Reinserted";
	REQUIRE(tree.insert(newName , testId));
	// And duplicate insertion should again fail.
	REQUIRE_FALSE(tree.insert("DuplicateAgain" , testId));
}

// Additional Test: Randomized Rotation Consistency Test
TEST_CASE("Randomized Rotation Consistency Test" , "[random_rotations]") {
	// Run 20 iterations; in each, insert 50 nodes in random order and check that the tree is balanced.
	std::random_device rd;
	std::mt19937 gen(rd());
	for (int trial = 0; trial < 20; trial++) {
		AVLTree tree;
		std::vector<int> insertedIDs;
		for (int i = 0; i < 50; i++) {
			// Generate a random UFID.
			int id = 10000000 + gen() % 100000;
			if (tree.insert("RRCT" + intToName(i) , id))
				insertedIDs.push_back(id);
		}
		// Sort the inserted IDs.
		std::sort(insertedIDs.begin() , insertedIDs.end());
		insertedIDs.erase(std::unique(insertedIDs.begin() , insertedIDs.end()) , insertedIDs.end());
		// Verify that the inorder traversal size equals the number of unique inserted IDs.
		auto inorder = tree.printInorder();
		REQUIRE(inorder.size() == insertedIDs.size());
	}
}

// Additional Test: Tree Stability Under Repeated Operations
TEST_CASE("Tree Stability Under Repeated Operations" , "[stability]") {
	AVLTree tree;
	const int cycles = 50;
	std::random_device rd;
	std::mt19937 gen(rd());
	for (int cycle = 0; cycle < cycles; cycle++) {
		// Insert 20 nodes in each cycle.
		std::vector<int> ids;
		for (int i = 1; i <= 20; i++) {
			int id = 10000000 + cycle * 100 + i;
			std::string name = "Stable" + intToName(i);
			if (tree.insert(name , id))
				ids.push_back(id);
		}
		// Ensure nodes were added.
		REQUIRE(tree.printInorder().size() >= ids.size());
		// Remove all nodes added in this cycle.
		for (int id : ids) {
			REQUIRE(tree.remove(id));
		}
		// Confirm none of the cycle's nodes remain.
		for (int id : ids) {
			REQUIRE(tree.searchId(id) == "unsuccessful");
		}
	}
}

// Additional Test: Stress Test with Limited UFID Range
TEST_CASE("Stress Test with Limited UFID Range" , "[limited_ufid]") {
	// Use a narrow UFID range so duplicate attempts occur.
	AVLTree tree;
	const int lower = 10000000;
	const int upper = 10000050;
	std::map<int , std::string> record;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> ufidDist(lower , upper);
	std::uniform_int_distribution<int> opDist(1 , 4);

	for (int i = 0; i < 500; i++) {
		int op = opDist(gen);
		int id = ufidDist(gen);
		if (op == 1) { // Insert.
			std::string name = "LUR" + intToName(i % 100);
			bool inserted = tree.insert(name , id);
			if (record.find(id) == record.end()) {
				REQUIRE(inserted);
				record[id] = name;
			}
			else {
				REQUIRE_FALSE(inserted);
			}
		}
		else if (op == 2) { // Remove by id.
			if (!record.empty()) {
				auto it = record.begin();
				std::advance(it , gen() % record.size());
				int removeId = it->first;
				bool removed = tree.remove(removeId);
				REQUIRE(removed);
				record.erase(removeId);
			}
			else {
				REQUIRE_FALSE(tree.remove(id));
			}
		}
		else if (op == 3) { // removeInorder.
			size_t sizeBefore = tree.printInorder().size();
			if (sizeBefore > 0) {
				std::uniform_int_distribution<int> indexDist(0 , sizeBefore - 1);
				int idx = indexDist(gen);
				std::vector<int> sortedIDs;
				for (auto& e : record)
					sortedIDs.push_back(e.first);
				int target = sortedIDs[idx];
				bool removed = tree.removeInorder(idx);
				REQUIRE(removed);
				record.erase(target);
			}
			else {
				REQUIRE_FALSE(tree.removeInorder(0));
			}
		}
		else { // Search.
			std::string res = tree.searchId(id);
			if (record.find(id) != record.end()) {
				REQUIRE(res == record[id]);
			}
			else {
				REQUIRE(res == "unsuccessful");
			}
		}
	}
	// Final consistency.
	REQUIRE(tree.printInorder().size() == record.size());
}

// Additional Test: Full Traversal Validation Under Load
TEST_CASE("Full Traversal Validation Under Load" , "[full_traversal]") {
	AVLTree tree;
	const int numNodes = 300;
	std::vector<int> ufids;
	// Insert nodes with sequential UFIDs.
	for (int i = 1; i <= numNodes; i++) {
		int id = 10000000 + i;
		ufids.push_back(id);
		std::string name = "FTVL" + intToName(i % 100);
		REQUIRE(tree.insert(name , id));
	}
	// Verify that each inserted UFID yields a valid result.
	for (int id : ufids) {
		std::string res = tree.searchId(id);
		REQUIRE(res != "unsuccessful");
	}
	// Also, check that preorder and postorder traversals contain the same number of nodes.
	REQUIRE(tree.printPreorder().size() == numNodes);
	REQUIRE(tree.printPostorder().size() == numNodes);
}

// Additional Test: 1000 Insertions, 1000 Removals, and Final Consistency
TEST_CASE("1000 Insertions, 1000 Removals, and Final Consistency" , "[massive_ops]") {
	AVLTree tree;
	const int numOps = 1000;
	std::vector<int> ufids;
	// Insert 1000 nodes sequentially.
	for (int i = 1; i <= numOps; i++) {
		int id = 10000000 + i;
		ufids.push_back(id);
		std::string name = "MOPS" + intToName(i % 100);
		REQUIRE(tree.insert(name , id));
	}
	REQUIRE(tree.printInorder().size() == numOps);
	// Randomly remove nodes until the tree is empty.
	std::random_device rd;
	std::mt19937 gen(rd());
	while (!ufids.empty()) {
		std::uniform_int_distribution<int> indexDist(0 , ufids.size() - 1);
		int index = indexDist(gen);
		int idToRemove = ufids[index];
		bool removed = tree.remove(idToRemove);
		REQUIRE(removed);
		ufids.erase(ufids.begin() + index);
		// Consistency check on size.
		REQUIRE(tree.printInorder().size() == ufids.size());
	}
	// Final consistency: the tree must be empty.
	REQUIRE(tree.printInorder().empty());
	REQUIRE(tree.printLevelCount() == 0);
}

// Additional Test: Randomized Insert
TEST_CASE("Randomized Insert" , "[random_insert]") {
	AVLTree tree;
	std::map<int , std::string> record;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> ufidDist(10000000 , 10010000);
	for (int i = 0; i < 1000; i++) {
		int id = ufidDist(gen);
		std::string name = "RI" + intToName(i % 100);
		bool inserted = tree.insert(name , id);
		if (record.find(id) == record.end()) {
			REQUIRE(inserted);
			record[id] = name;
		}
		else {
			REQUIRE_FALSE(inserted);
		}
	}
	// Consistency check.
	REQUIRE(tree.printInorder().size() == record.size());
}

// Additional Test: Randomized Remove
TEST_CASE("Randomized Remove" , "[random_remove]") {
	AVLTree tree;
	std::map<int , std::string> record;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> ufidDist(10000000 , 10010000);
	for (int i = 0; i < 1000; i++) {
		int id = ufidDist(gen);
		std::string name = "RR" + intToName(i % 100);
		bool inserted = tree.insert(name , id);
		if (record.find(id) == record.end()) {
			REQUIRE(inserted);
			record[id] = name;
		}
		else {
			REQUIRE_FALSE(inserted);
		}
	}
	// Randomly remove nodes until the tree is empty.
	while (!record.empty()) {
		std::uniform_int_distribution<int> indexDist(0 , record.size() - 1);
		int index = indexDist(gen);
		auto it = record.begin();
		std::advance(it , index);
		int idToRemove = it->first;
		bool removed = tree.remove(idToRemove);
		REQUIRE(removed);
		record.erase(idToRemove);
	}
	// Final consistency check.
	REQUIRE(tree.printInorder().empty());
	REQUIRE(tree.printLevelCount() == 0);
}

// Additional Test: Randomized Search
TEST_CASE("Randomized Search" , "[random_search]") {
	AVLTree tree;
	std::map<int , std::string> record;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> ufidDist(10000000 , 10010000);
	for (int i = 0; i < 1000; i++) {
		int id = ufidDist(gen);
		std::string name = "RS" + intToName(i % 100);
		bool inserted = tree.insert(name , id);
		if (record.find(id) == record.end()) {
			REQUIRE(inserted);
			record[id] = name;
		}
		else {
			REQUIRE_FALSE(inserted);
		}
	}
	// Randomly search for nodes.
	for (int i = 0; i < 1000; i++) {
		int id = ufidDist(gen);
		std::string res = tree.searchId(id);
		if (record.find(id) != record.end()) {
			REQUIRE(res == record[id]);
		}
		else {
			REQUIRE(res == "unsuccessful");
		}
	}
}

// Additional Test: Randomized RemoveInorder
TEST_CASE("Randomized RemoveInorder" , "[random_removeinorder]") {
	AVLTree tree;
	std::map<int , std::string> record;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> ufidDist(10000000 , 10010000);
	for (int i = 0; i < 1000; i++) {
		int id = ufidDist(gen);
		std::string name = "RI" + intToName(i % 100);
		bool inserted = tree.insert(name , id);
		if (record.find(id) == record.end()) {
			REQUIRE(inserted);
			record[id] = name;
		}
		else {
			REQUIRE_FALSE(inserted);
		}
	}
	// Randomly remove nodes using removeInorder.
	while (!record.empty()) {
		std::uniform_int_distribution<int> indexDist(0 , record.size() - 1);
		int index = indexDist(gen);
		std::vector<int> sortedIDs;
		for (auto& e : record)
			sortedIDs.push_back(e.first);
		bool removed = tree.removeInorder(index);
		REQUIRE(removed);
		record.erase(sortedIDs[index]);
	}
	// Final consistency check.
	REQUIRE(tree.printInorder().empty());
	REQUIRE(tree.printLevelCount() == 0);
}

// Additional Test: Randomized Operations
TEST_CASE("Randomized Operations" , "[random_ops]") {
	AVLTree tree;
	std::map<int , std::string> record;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> ufidDist(10000000 , 10010000);
	std::uniform_int_distribution<int> opDist(1 , 4);
	for (int i = 0; i < 1000; i++) {
		int op = opDist(gen);
		int id = ufidDist(gen);
		if (op == 1) { // Insert.
			std::string name = "RO" + intToName(i % 100);
			bool inserted = tree.insert(name , id);
			if (record.find(id) == record.end()) {
				REQUIRE(inserted);
				record[id] = name;
			}
			else {
				REQUIRE_FALSE(inserted);
			}
		}
		else if (op == 2) { // Remove by id.
			if (!record.empty()) {
				auto it = record.begin();
				std::advance(it , gen() % record.size());
				int removeId = it->first;
				bool removed = tree.remove(removeId);
				REQUIRE(removed);
				record.erase(removeId);
			}
			else {
				REQUIRE_FALSE(tree.remove(id));
			}
		}
		else if (op == 3) { // removeInorder.
			size_t sizeBefore = tree.printInorder().size();
			if (sizeBefore > 0) {
				std::uniform_int_distribution<int> indexDist(0 , sizeBefore - 1);
				int idx = indexDist(gen);
				std::vector<int> sortedIDs;
				for (auto& e : record)
					sortedIDs.push_back(e.first);
				int target = sortedIDs[idx];
				bool removed = tree.removeInorder(idx);
				REQUIRE(removed);
				record.erase(target);
			}
			else {
				REQUIRE_FALSE(tree.removeInorder(0));
			}
		}
		else { // Search.
			std::string res = tree.searchId(id);
			if (record.find(id) != record.end()) {
				REQUIRE(res == record[id]);
			}
			else {
				REQUIRE(res == "unsuccessful");
			}
		}
		// Consistency check.
		size_t inordSize = tree.printInorder().size();
		REQUIRE(inordSize == record.size());
		REQUIRE(tree.printPreorder().size() == inordSize);
		REQUIRE(tree.printPostorder().size() == inordSize);
		if (record.empty())
			REQUIRE(tree.printLevelCount() == 0);
		else
			REQUIRE(tree.printLevelCount() >= 1);
	}
}

// Additional Test: Randomized Operations with Limited UFID Range
TEST_CASE("Randomized Operations with Limited UFID Range" , "[random_limited]") {
	AVLTree tree;
	const int lower = 10000000;
	const int upper = 10010000;
	std::map<int , std::string> record;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> ufidDist(lower , upper);
	std::uniform_int_distribution<int> opDist(1 , 4);
	for (int i = 0; i < 1000; i++) {
		int op = opDist(gen);
		int id = ufidDist(gen);
		if (op == 1) { // Insert.
			std::string name = "ROL" + intToName(i % 100);
			bool inserted = tree.insert(name , id);
			if (record.find(id) == record.end()) {
				REQUIRE(inserted);
				record[id] = name;
			}
			else {
				REQUIRE_FALSE(inserted);
			}
		}
		else if (op == 2) { // Remove by id.
			if (!record.empty()) {
				auto it = record.begin();
				std::advance(it , gen() % record.size());
				int removeId = it->first;
				bool removed = tree.remove(removeId);
				REQUIRE(removed);
				record.erase(removeId);
			}
			else {
				REQUIRE_FALSE(tree.remove(id));
			}
		}
		else if (op == 3) { // removeInorder.
			size_t sizeBefore = tree.printInorder().size();
			if (sizeBefore > 0) {
				std::uniform_int_distribution<int> indexDist(0 , sizeBefore - 1);
				int idx = indexDist(gen);
				std::vector<int> sortedIDs;
				for (auto& e : record)
					sortedIDs.push_back(e.first);
				int target = sortedIDs[idx];
				bool removed = tree.removeInorder(idx);
				REQUIRE(removed);
				record.erase(target);
			}
			else {
				REQUIRE_FALSE(tree.removeInorder(0));
			}
		}
		else { // Search.
			std::string res = tree.searchId(id);
			if (record.find(id) != record.end()) {
				REQUIRE(res == record[id]);
			}
			else {
				REQUIRE(res == "unsuccessful");
			}
		}
	}
}



// Additional Test: Randomized Operations with Limited UFID Range
TEST_CASE("Randomized Operatoins with Limited UFID Range" , "[random_limited]") {
	AVLTree tree;
	const int lower = 10000000;
	const int upper = 10010000;
	std::map<int , std::string> record;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> ufidDist(lower , upper);
	std::uniform_int_distribution<int> opDist(1 , 4);
	for (int i = 0; i < 1000; i++) {
		int op = opDist(gen);
		int id = ufidDist(gen);
		if (op == 1) { // Insert.
			std::string name = "ROL" + intToName(i % 100);
			bool inserted = tree.insert(name , id);
			if (record.find(id) == record.end()) {
				REQUIRE(inserted);
				record[id] = name;
			}
			else {
				REQUIRE_FALSE(inserted);
			}
		}
		else if (op == 2) { // Remove by id.
			if (!record.empty()) {
				auto it = record.begin();
				std::advance(it , gen() % record.size());
				int removeId = it->first;
				bool removed = tree.remove(removeId);
				REQUIRE(removed);
				record.erase(removeId);
			}
			else {
				REQUIRE_FALSE(tree.remove(id));
			}
		}
		else if (op == 3) { // removeInorder.
			size_t sizeBefore = tree.printInorder().size();
			if (sizeBefore > 0) {
				std::uniform_int_distribution<int> indexDist(0 , sizeBefore - 1);
				int idx = indexDist(gen);
				std::vector<int> sortedIDs;
				for (auto& e : record)
					sortedIDs.push_back(e.first);
				int target = sortedIDs[idx];
				bool removed = tree.removeInorder(idx);
				REQUIRE(removed);
				record.erase(target);
			}
			else {
				REQUIRE_FALSE(tree.removeInorder(0));
			}
		}
		else { // Search.
			std::string res = tree.searchId(id);
			if (record.find(id) != record.end()) {
				REQUIRE(res == record[id]);
			}
			else {
				REQUIRE(res == "unsuccessful");
			}
		}
	}
}


// Test case to test printInorder, printPostorder, and printPreorder
TEST_CASE("Test case to test inorder postorder and preorder" , "[traversal]") {
	AVLTree tree;

	// Insert nodes into the tree using valid alphabetical names (no digits).
	REQUIRE(tree.insert("NodeA" , 10000001));
	REQUIRE(tree.insert("NodeB" , 10000002));
	REQUIRE(tree.insert("NodeC" , 10000003));
	REQUIRE(tree.insert("NodeD" , 10000004));
	REQUIRE(tree.insert("NodeE" , 10000005));

	// Expected traversals based on the final balanced tree.
	std::vector<std::string> expectedInorder = { "NodeA", "NodeB", "NodeC", "NodeD", "NodeE" };
	std::vector<std::string> expectedPreorder = { "NodeB", "NodeA", "NodeD", "NodeC", "NodeE" };
	std::vector<std::string> expectedPostorder = { "NodeA", "NodeC", "NodeE", "NodeD", "NodeB" };

	// Check inorder traversal
	auto inorder = tree.printInorder();
	REQUIRE(inorder == expectedInorder);

	// Check preorder traversal
	auto preorder = tree.printPreorder();
	REQUIRE(preorder == expectedPreorder);

	// Check postorder traversal
	auto postorder = tree.printPostorder();
	REQUIRE(postorder == expectedPostorder);
}

// Test case to test printLevelCount
TEST_CASE("Test case to test level count" , "[level_count]") {
	AVLTree tree;

	// Insert nodes into the tree using valid alphabetical names (no digits).
	REQUIRE(tree.insert("NodeA" , 10000001));
	REQUIRE(tree.insert("NodeB" , 10000002));
	REQUIRE(tree.insert("NodeC" , 10000003));
	REQUIRE(tree.insert("NodeD" , 10000004));
	REQUIRE(tree.insert("NodeE" , 10000005));

	// Expected level count based on the final balanced tree.
	int expectedLevelCount = 3;

	// Check level count
	REQUIRE(tree.printLevelCount() == expectedLevelCount);
}

// Test case to test random insertions and removals and random printInorder printPreorder and printPostorder
// Test case to test random insertions and removals and random printInorder printPreorder and printPostorder
TEST_CASE("Test case to test random insertions" , "[random_insertions]") {
	AVLTree tree;
	std::map<int , std::string> record;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> ufidDist(10000000 , 10010000);
	std::uniform_int_distribution<int> opDist(1 , 4);

	// Perform 100 random operations
	for (int i = 0; i < 100; ++i) {
		int op = opDist(gen);
		int id = ufidDist(gen);
		std::string name = "Node" + intToName(id % 100);

		if (op == 1) { // Insert
			bool inserted = tree.insert(name , id);
			if (record.find(id) == record.end()) {
				REQUIRE(inserted);
				record[id] = name;
			}
			else {
				REQUIRE_FALSE(inserted);
			}
		}
		else if (op == 2) { // Remove by id
			if (!record.empty()) {
				auto it = record.begin();
				std::advance(it , gen() % record.size());
				int removeId = it->first;
				bool removed = tree.remove(removeId);
				REQUIRE(removed);
				record.erase(removeId);
			}
			else {
				REQUIRE_FALSE(tree.remove(id));
			}
		}
		else if (op == 3) { // removeInorder
			size_t sizeBefore = tree.printInorder().size();
			if (sizeBefore > 0) {
				std::uniform_int_distribution<int> indexDist(0 , sizeBefore - 1);
				int idx = indexDist(gen);
				std::vector<int> sortedIDs;
				for (auto& e : record)
					sortedIDs.push_back(e.first);
				int target = sortedIDs[idx];
				bool removed = tree.removeInorder(idx);
				REQUIRE(removed);
				record.erase(target);
			}
			else {
				REQUIRE_FALSE(tree.removeInorder(0));
			}
		}
		else { // Search
			std::string res = tree.searchId(id);
			if (record.find(id) != record.end()) {
				REQUIRE(res == record[id]);
			}
			else {
				REQUIRE(res == "unsuccessful");
			}
		}

		// Consistency check
		size_t inordSize = tree.printInorder().size();
		REQUIRE(inordSize == record.size());
		REQUIRE(tree.printPreorder().size() == inordSize);
		REQUIRE(tree.printPostorder().size() == inordSize);
		if (record.empty())
			REQUIRE(tree.printLevelCount() == 0);
		else
			REQUIRE(tree.printLevelCount() >= 1);
	}
}

// Test case to test random insertions with unique UFIDs
TEST_CASE("random test case to insert unique nodes" , "[random_insertions]") {
	AVLTree tree;
	std::vector<int> ufidVec;
	std::set<int> uniqueUFIDs;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> ufidDist(10000000 , 10010000);

	// Generate 200 unique UFIDs.
	while (uniqueUFIDs.size() < 200) {
		uniqueUFIDs.insert(ufidDist(gen));
	}
	// Convert the set to a vector.
	ufidVec.assign(uniqueUFIDs.begin() , uniqueUFIDs.end());

	// Insert 200 nodes with these unique UFIDs.
	for (size_t i = 0; i < 200; i++) {
		// "Node" + intToName(i) generates a valid name without digits.
		bool inserted = tree.insert("Node" + intToName(i) , ufidVec[i]);
		REQUIRE(inserted);  // Each insertion must succeed.
	}
	REQUIRE(tree.printInorder().size() == 200);
}

// even more testing for the tree
TEST_CASE("Randomized Operations with Limited UFID Range" , "[random_check]") {
	AVLTree tree;
	std::map<int , std::string> record;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> ufidDist(10000000 , 10010000);
	std::uniform_int_distribution<int> opDist(1 , 4);

	for (int i = 0; i < 1000; i++) {
		int op = opDist(gen);
		int id = ufidDist(gen);
		if (op == 1) { // Insert
			std::string name = "RO" + intToName(i % 100);
			bool inserted = tree.insert(name , id);
			if (record.find(id) == record.end()) {
				REQUIRE(inserted);
				record[id] = name;
			}
			else {
				REQUIRE_FALSE(inserted);
			}
		}
		else if (op == 2) { // Remove by id
			if (!record.empty()) {
				auto it = record.begin();
				std::advance(it , gen() % record.size());
				int removeId = it->first;
				bool removed = tree.remove(removeId);
				REQUIRE(removed);
				record.erase(removeId);
			}
			else {
				REQUIRE_FALSE(tree.remove(id));
			}
		}
		else if (op == 3) { // removeInorder
			size_t sizeBefore = tree.printInorder().size();
			if (sizeBefore > 0) {
				std::uniform_int_distribution<int> indexDist(0 , sizeBefore - 1);
				int idx = indexDist(gen);
				std::vector<int> sortedIDs;
				for (auto& e : record)
					sortedIDs.push_back(e.first);
				int target = sortedIDs[idx];
				bool removed = tree.removeInorder(idx);
				REQUIRE(removed);
				record.erase(target);
			}
			else {
				REQUIRE_FALSE(tree.removeInorder(0));
			}
		}
		else { // Search
			std::string res = tree.searchId(id);
			if (record.find(id) != record.end()) {
				REQUIRE(res == record[id]);
			}
			else {
				REQUIRE(res == "unsuccessful");
			}
		}
	}
	REQUIRE(tree.printInorder().size() == record.size());
}

