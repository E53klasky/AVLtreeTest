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

TEST_CASE("AVLTree Insertions and Balance" , "[insert]") {
	AVLTree tree;
	REQUIRE(tree.insert("Alice" , 12345678));
	REQUIRE(tree.insert("Bob" , 23456789));
	REQUIRE(tree.insert("Charlie" , 34567890));
	REQUIRE_FALSE(tree.insert("Alice" , 12345678)); // Duplicate ID should fail
}

TEST_CASE("AVLTree Height and Balancing" , "[balance]") {
	AVLTree tree;
	tree.insert("Alice" , 12345678);
	tree.insert("Bob" , 23456789);
	tree.insert("Charlie" , 34567890);
	REQUIRE(tree.printLevelCount() == 2); // Ensuring balance is maintained
}

TEST_CASE("AVLTree Search by ID" , "[searchId]") {
	AVLTree tree;
	tree.insert("Alice" , 12345678);
	tree.insert("Bob" , 23456789);
	REQUIRE(tree.searchId(12345678) == "Alice");
	REQUIRE(tree.searchId(23456789) == "Bob");
	REQUIRE(tree.searchId(99999999) == "unsuccessful"); // ID not present
}

TEST_CASE("AVLTree Search by Name" , "[searchName]") {
	AVLTree tree;
	tree.insert("Alice" , 12345678);
	tree.insert("Alice" , 87654321);
	auto result = tree.searchName("Alice");
	REQUIRE(result.size() == 2);
	REQUIRE(std::find(result.begin() , result.end() , 12345678) != result.end());
	REQUIRE(std::find(result.begin() , result.end() , 87654321) != result.end());
}

TEST_CASE("AVLTree Removal" , "[remove]") {
	AVLTree tree;
	tree.insert("Alice" , 12345678);
	tree.insert("Bob" , 23456789);
	REQUIRE(tree.remove(12345678));
	REQUIRE_FALSE(tree.remove(12345678)); // Already removed
}

TEST_CASE("AVLTree Removal and Balance" , "[removeBalance]") {
	AVLTree tree;
	tree.insert("Alice" , 12345678);
	tree.insert("Bob" , 23456789);
	tree.insert("Charlie" , 34567890);
	REQUIRE(tree.remove(12345678));
	REQUIRE(tree.printLevelCount() == 2);
}

TEST_CASE("AVLTree Print Inorder" , "[traversal]") {
	AVLTree tree;
	tree.insert("Alice" , 12345678);
	tree.insert("Bob" , 23456789);
	tree.insert("Charlie" , 34567890);
	auto result = tree.printInorder();
	REQUIRE(result.size() == 3);
	REQUIRE(result[0] == "Alice");
	REQUIRE(result[1] == "Bob");
	REQUIRE(result[2] == "Charlie");
}

TEST_CASE("AVLTree Print Preorder" , "[traversal]") {
	AVLTree tree;
	tree.insert("Alice" , 12345678);
	tree.insert("Bob" , 23456789);
	tree.insert("Charlie" , 34567890);
	auto result = tree.printPreorder();
	REQUIRE(result[0] == "Bob");
}

TEST_CASE("AVLTree Print Postorder" , "[traversal]") {
	AVLTree tree;
	tree.insert("Alice" , 12345678);
	tree.insert("Bob" , 23456789);
	tree.insert("Charlie" , 34567890);
	auto result = tree.printPostorder();
	REQUIRE(result.back() == "Bob");
}

TEST_CASE("AVLTree Level Count" , "[levels]") {
	AVLTree tree;
	tree.insert("Alice" , 12345678);
	REQUIRE(tree.printLevelCount() == 1);
	tree.insert("Bob" , 23456789);
	tree.insert("Charlie" , 34567890);
	REQUIRE(tree.printLevelCount() == 2);
}

TEST_CASE("AVLTree Remove Inorder" , "[removeInorder]") {
	AVLTree tree;
	tree.insert("Alice" , 12345678);
	tree.insert("Bob" , 23456789);
	tree.insert("Charlie" , 34567890);
	REQUIRE(tree.removeInorder(1));
	REQUIRE_FALSE(tree.searchId(23456789) == "Bob");
}

TEST_CASE("AVLTree Edge Case: Empty Tree Removal" , "[edge]") {
	AVLTree tree;
	REQUIRE_FALSE(tree.remove(12345678));
}

TEST_CASE("AVLTree Edge Case: Invalid Gator ID" , "[edge]") {
	AVLTree tree;
	REQUIRE_FALSE(tree.insert("Invalid" , 9999)); // ID must be 8 digits
	REQUIRE_FALSE(tree.insert("Invalid" , 123456789));
}

TEST_CASE("AVLTree Edge Case: Invalid Name" , "[edge]") {
	AVLTree tree;
	REQUIRE_FALSE(tree.insert("12345678" , 12345678)); // Name must not be a number
}



TEST_CASE("Insertion and Basic Search") {
	AVLTree tree;
	REQUIRE(tree.insert("Alice" , 12345678));
	REQUIRE(tree.insert("Bob" , 87654321));
	REQUIRE(tree.searchId(12345678) == "Alice");
	REQUIRE(tree.searchId(87654321) == "Bob");
	REQUIRE(tree.searchId(11111111) == "unsuccessful");
}

TEST_CASE("Insertion of Duplicate IDs") {
	AVLTree tree;
	REQUIRE(tree.insert("Alice" , 12345678));
	REQUIRE_FALSE(tree.insert("Bob" , 12345678));
}

TEST_CASE("Invalid ID Ranges") {
	AVLTree tree;
	REQUIRE_FALSE(tree.insert("Charlie" , 9999999));
	REQUIRE_FALSE(tree.insert("David" , 100000000));
}

TEST_CASE("Invalid Name Format") {
	AVLTree tree;
	REQUIRE_FALSE(tree.insert("John123" , 12345678));
	REQUIRE_FALSE(tree.insert("@lice" , 87654321));
}

TEST_CASE("Insertion and Removal") {
	AVLTree tree;
	REQUIRE(tree.insert("Eve" , 13579246));
	REQUIRE(tree.remove(13579246));
	REQUIRE(tree.searchId(13579246) == "unsuccessful");
	REQUIRE_FALSE(tree.remove(13579246));
}

TEST_CASE("Tree Balancing on Insertions") {
	AVLTree tree;
	REQUIRE(tree.insert("A" , 11111111));
	REQUIRE(tree.insert("B" , 22222222));
	REQUIRE(tree.insert("C" , 33333333)); // Should trigger rebalancing
	REQUIRE(tree.printLevelCount() == 2);
}

TEST_CASE("Tree Balancing on Removals") {
	AVLTree tree;
	REQUIRE(tree.insert("X" , 55555555));
	REQUIRE(tree.insert("Y" , 66666666));
	REQUIRE(tree.insert("Z" , 77777777));
	REQUIRE(tree.remove(55555555));
	REQUIRE(tree.printLevelCount() == 2);
}

TEST_CASE("Search by Name") {
	AVLTree tree;
	REQUIRE(tree.insert("Grace" , 12312312));
	REQUIRE(tree.insert("Grace" , 32132132));
	auto results = tree.searchName("Grace");
	REQUIRE(results.size() == 2);
}

TEST_CASE("Print Orders") {
	AVLTree tree;
	tree.insert("Alpha" , 11111111);
	tree.insert("Beta" , 22222222);
	tree.insert("Gamma" , 33333333);
	auto inorder = tree.printInorder();
	REQUIRE(inorder == std::vector<std::string>{"Alpha" , "Beta" , "Gamma"});
	auto preorder = tree.printPreorder();
	REQUIRE(preorder.front() == "Beta"); // Balanced tree check
	auto postorder = tree.printPostorder();
	REQUIRE(postorder.back() == "Beta");
}

TEST_CASE("Remove Non-Existent Node") {
	AVLTree tree;
	REQUIRE_FALSE(tree.remove(99999999));
}

TEST_CASE("Remove Root Node with Children") {
	AVLTree tree;
	tree.insert("Root" , 50000000);
	tree.insert("Left" , 25000000);
	tree.insert("Right" , 75000000);
	REQUIRE(tree.remove(50000000));
	REQUIRE(tree.searchId(50000000) == "unsuccessful");
}

TEST_CASE("Remove Inorder by Index") {
	AVLTree tree;
	tree.insert("One" , 11111111);
	tree.insert("Two" , 22222222);
	tree.insert("Three" , 33333333);
	REQUIRE(tree.removeInorder(1));
	auto inorder = tree.printInorder();
	REQUIRE(inorder.size() == 2);
}

// Large Tree Insertion and Height Check
TEST_CASE("Large Tree Insertion and Height Check" , "[heightCheck]") {
	AVLTree tree;
	for (int i = 1; i <= 100; ++i) {
		// Use intToName(i) so the name contains only letters.
		REQUIRE(tree.insert("Student" + intToName(i) , 10000000 + i));
	}
	REQUIRE(tree.printLevelCount() <= 7);
}
// Additional Test Case 1: Single Node Reinsert After Removal
TEST_CASE("Single Node Reinsert After Removal" , "[reinsert]") {
	AVLTree tree;
	REQUIRE(tree.insert("Alpha" , 10000001));
	REQUIRE(tree.searchId(10000001) == "Alpha");
	REQUIRE(tree.remove(10000001));
	REQUIRE(tree.searchId(10000001) == "unsuccessful");
	REQUIRE(tree.insert("Alpha" , 10000001));
	REQUIRE(tree.searchId(10000001) == "Alpha");
}

// Additional Test Case 2: Multiple Duplicate Insertion Attempts
TEST_CASE("Multiple Duplicate Insertion Attempts" , "[duplicates]") {
	AVLTree tree;
	REQUIRE(tree.insert("Beta" , 10000002));
	for (int i = 0; i < 5; ++i) {
		REQUIRE_FALSE(tree.insert("BetaDup" , 10000002));
	}
	REQUIRE(tree.printInorder().size() == 1);
}

// Additional Test Case 3: Mixed Valid and Invalid Insertions
TEST_CASE("Mixed Valid and Invalid Insertions" , "[mixed_inserts]") {
	AVLTree tree;
	// Valid insertions (alphabetical only)
	REQUIRE(tree.insert("Gamma" , 10000003));
	REQUIRE(tree.insert("Delta" , 10000004));
	// These names include digits or special characters and must be rejected.
	REQUIRE_FALSE(tree.insert("Epsilon1" , 10000005));
	REQUIRE_FALSE(tree.insert("Zeta@" , 10000006));
	// Check that only the valid nodes are present:
	REQUIRE(tree.printInorder().size() == 2);
}

// Additional Test Case 4: Search on Nonexistent UFIDs
TEST_CASE("Search on Nonexistent UFIDs" , "[search_nonexistent]") {
	AVLTree tree;
	REQUIRE(tree.insert("Theta" , 10000007));
	REQUIRE(tree.searchId(10000007) == "Theta");
	// A UFID that was not inserted should return "unsuccessful".
	REQUIRE(tree.searchId(20000000) == "unsuccessful");
}

// Additional Test Case 5: Bulk Removal by Index Test
TEST_CASE("Bulk Removal by Index Test" , "[bulk_remove]") {
	AVLTree tree;
	const int n = 10;
	for (int i = 1; i <= n; ++i) {
		REQUIRE(tree.insert("Node" + intToName(i) , 10000000 + i));
	}
	// Remove nodes one by one using removeInorder.
	for (int i = 0; i < n; ++i) {
		REQUIRE(tree.removeInorder(0));
	}
	REQUIRE(tree.printInorder().empty());
}

// Additional Test Case 6: Large Tree Deletion Test
TEST_CASE("Large Tree Deletion Test" , "[large_deletion]") {
	AVLTree tree;
	const int count = 50;
	for (int i = 1; i <= count; ++i) {
		REQUIRE(tree.insert("Node" + intToName(i) , 10000000 + i));
	}
	// Remove every second node by id.
	for (int i = 2; i <= count; i += 2) {
		REQUIRE(tree.remove(10000000 + i));
	}
	REQUIRE(tree.printInorder().size() == (count + 1) / 2);
}

// Additional Test Case 7: Tree Rotation Integrity Test
TEST_CASE("Tree Rotation Integrity Test" , "[rotation_integrity]") {
	AVLTree tree;
	// Insert nodes in descending order, which forces rotations.
	REQUIRE(tree.insert("Iota" , 30000000));
	REQUIRE(tree.insert("Kappa" , 20000000));
	REQUIRE(tree.insert("Lambda" , 10000000));
	auto inorder = tree.printInorder();
	// Expected sorted order by UFID.
	std::vector<std::string> expected = { "Lambda", "Kappa", "Iota" };
	REQUIRE(inorder == expected);
}

// Additional Test Case 8: Search by Name Test (if multiple results are allowed)
TEST_CASE("Search by Name Test" , "[search_name]") {
	AVLTree tree;
	// Insert two nodes with the same name but different UFIDs.
	REQUIRE(tree.insert("Mu" , 10000008));
	REQUIRE(tree.insert("Mu" , 10000009));
	auto results = tree.searchName("Mu");
	// Expect to find 2 nodes with the name "Mu".
	REQUIRE(results.size() == 2);
}

// Additional Test Case 9: Removal of Non-Existing Node
TEST_CASE("Removal of Non-Existing Node" , "[remove_nonexistent]") {
	AVLTree tree;
	// Removal of a node that was not inserted must fail.
	REQUIRE_FALSE(tree.remove(99999999));
}

// Additional Test Case 10: Insert with Invalid UFID
TEST_CASE("Insert with Invalid UFID" , "[invalid_ufid]") {
	AVLTree tree;
	// UFID too low.
	REQUIRE_FALSE(tree.insert("Nu" , 9999999));
	// UFID too high.
	REQUIRE_FALSE(tree.insert("Xi" , 100000000));
}

// Additional Test Case 11: Complex Mixed Operations
TEST_CASE("Complex Mixed Operations" , "[complex_ops]") {
	AVLTree tree;
	// Insert several nodes.
	REQUIRE(tree.insert("Omicron" , 10000010));
	REQUIRE(tree.insert("Pi" , 10000011));
	REQUIRE(tree.insert("Rho" , 10000012));
	// Remove one node.
	REQUIRE(tree.remove(10000011));
	// Insert additional nodes.
	REQUIRE(tree.insert("Sigma" , 10000013));
	REQUIRE(tree.insert("Tau" , 10000014));
	// The tree should now contain 4 nodes.
	REQUIRE(tree.printInorder().size() == 4);
}

// Additional Test Case 12: Repeated Insertion and Removal in a Loop
TEST_CASE("Repeated Insertion and Removal in a Loop" , "[loop_ins_rem]") {
	AVLTree tree;
	for (int i = 0; i < 10; ++i) {
		REQUIRE(tree.insert("LoopTest" , 10000000 + i));
		REQUIRE(tree.remove(10000000 + i));
	}
	REQUIRE(tree.printInorder().empty());
}

// Additional Test Case 13: Removal Inorder with Negative Index
TEST_CASE("Removal Inorder with Negative Index" , "[remove_inorder_negative]") {
	AVLTree tree;
	REQUIRE_FALSE(tree.removeInorder(-1));
}

// Updated Test Case 14: Removal Inorder with Index Out of Bounds
TEST_CASE("Removal Inorder with Index Out of Bounds" , "[remove_inorder_oob]") {
	AVLTree tree;
	// Insert a few nodes with valid names (only letters)
	for (int i = 0; i < 5; ++i) {
		REQUIRE(tree.insert("Case" + intToName(i) , 10000000 + i));
	}
	REQUIRE_FALSE(tree.removeInorder(10)); // index out-of-bounds
}

// Additional Test Case 15: Inorder Order Verification
TEST_CASE("Inorder Order Verification" , "[inorder_verify]") {
	AVLTree tree;
	// Insert nodes with UFIDs in non-sorted order
	std::vector<int> ufids = { 10000005, 10000003, 10000004, 10000001, 10000002 };
	for (int id : ufids) {
		REQUIRE(tree.insert("Node" + intToName(id % 26) , id));
	}
	auto inorder = tree.printInorder();
	// Inorder should yield 5 nodes (sorted by UFID).
	REQUIRE(inorder.size() == 5);
}

// Additional Test Case 16: Preorder vs Postorder Node Count
TEST_CASE("Preorder vs Postorder Node Count" , "[pre_post_count]") {
	AVLTree tree;
	for (int i = 1; i <= 10; ++i) {
		REQUIRE(tree.insert("Node" + intToName(i) , 10000000 + i));
	}
	REQUIRE(tree.printPreorder().size() == tree.printPostorder().size());
}

// Additional Test Case 17: Search by Name for Nonexistent Entry
TEST_CASE("Search by Name for Nonexistent Entry" , "[search_by_name_nonexistent]") {
	AVLTree tree;
	REQUIRE(tree.insert("Omega" , 10000011));
	auto result = tree.searchName("Nonexistent");
	REQUIRE(result.empty());
}

// Additional Test Case 18: Insertion of Node with Space in Name
TEST_CASE("Insertion of Node with Space in Name" , "[name_space]") {
	AVLTree tree;
	REQUIRE(tree.insert("Mary Jane" , 10000012));
	REQUIRE(tree.searchId(10000012) == "Mary Jane");
}

// Additional Test Case 19: Multiple Nodes with Same Name and Different UFIDs
TEST_CASE("Multiple Nodes with Same Name and Different UFIDs" , "[same_name_diff_ids]") {
	AVLTree tree;
	REQUIRE(tree.insert("SameName" , 10000013));
	REQUIRE(tree.insert("SameName" , 10000014));
	auto results = tree.searchName("SameName");
	REQUIRE(results.size() == 2);
}

// Additional Test Case 20: Remove Node Repeated Times
TEST_CASE("Remove Node Repeated Times" , "[repeat_remove]") {
	AVLTree tree;
	REQUIRE(tree.insert("Delta" , 10000015));
	REQUIRE(tree.remove(10000015));
	REQUIRE_FALSE(tree.remove(10000015)); // node already removed
}

// Additional Test Case 21: Unsorted Insertion and Inorder Verification
TEST_CASE("Unsorted Insertion and Inorder Verification" , "[unsorted_insertion]") {
	AVLTree tree;
	std::vector<int> ids = { 10000005, 10000001, 10000004, 10000003, 10000002 };
	for (int id : ids) {
		REQUIRE(tree.insert("Node" + intToName(id % 26) , id));
	}
	auto inorder = tree.printInorder();
	REQUIRE(inorder.size() == 5);
}

// Additional Test Case 22: Check Level Count on Empty Tree
TEST_CASE("Level Count on Empty Tree" , "[empty_level_count]") {
	AVLTree tree;
	REQUIRE(tree.printLevelCount() == 0);
}

// Additional Test Case 23: Insertion of Node with Single Letter Name
TEST_CASE("Insertion of Node with Single Letter Name" , "[single_letter]") {
	AVLTree tree;
	REQUIRE(tree.insert("A" , 10000016));
	auto inorder = tree.printInorder();
	REQUIRE(inorder.size() == 1);
	REQUIRE(inorder[0] == "A");
}

// Additional Test Case 24: Insertion of Node with Multiple Spaces in Name
TEST_CASE("Insertion of Node with Multiple Spaces" , "[multiple_spaces]") {
	AVLTree tree;
	REQUIRE(tree.insert("Charles Xavier" , 10000017));
	REQUIRE(tree.searchId(10000017) == "Charles Xavier");
}

// Additional Test Case 25: Remove Inorder and Verify Size Decreases
TEST_CASE("Remove Inorder Decreases Size" , "[remove_inorder_size]") {
	AVLTree tree;
	for (int i = 1; i <= 8; ++i) {
		REQUIRE(tree.insert("Node" + intToName(i) , 10000000 + i));
	}
	size_t sizeBefore = tree.printInorder().size();
	REQUIRE(tree.removeInorder(3));
	size_t sizeAfter = tree.printInorder().size();
	REQUIRE(sizeAfter == sizeBefore - 1);
}

// Additional Test Case 26: Sequential Insertion and Duplicate Rejection
TEST_CASE("Sequential Insertion and Duplicate Rejection" , "[sequential_duplicate]") {
	AVLTree tree;
	for (int i = 1; i <= 5; ++i) {
		REQUIRE(tree.insert("Seq" + intToName(i) , 10000000 + i));
	}
	// Insert a duplicate UFID which should fail.
	REQUIRE_FALSE(tree.insert("Duplicate" , 10000003));
	REQUIRE(tree.printInorder().size() == 5);
}

// Additional Test Case 27: Insertion with Boundary UFIDs
TEST_CASE("Insertion with Boundary UFIDs" , "[boundary_ufid]") {
	AVLTree tree;
	REQUIRE(tree.insert("MinBoundary" , 10000000));
	REQUIRE(tree.insert("MaxBoundary" , 99999999));
	REQUIRE(tree.searchId(10000000) == "MinBoundary");
	REQUIRE(tree.searchId(99999999) == "MaxBoundary");
}

// Additional Test Case 28: Remove All Nodes Using Remove by UFID
TEST_CASE("Remove All Nodes by UFID" , "[remove_all_by_id]") {
	AVLTree tree;
	for (int i = 1; i <= 10; ++i) {
		REQUIRE(tree.insert("Node" + intToName(i) , 10000000 + i));
	}
	for (int i = 1; i <= 10; ++i) {
		REQUIRE(tree.remove(10000000 + i));
	}
	REQUIRE(tree.printInorder().empty());
}

// Additional Test Case 29: Remove Non-Existent Node (Extra)
TEST_CASE("Remove Non-Existent Node (Extra)" , "[remove_nonexistent_extra]") {
	AVLTree tree;
	REQUIRE_FALSE(tree.remove(12345678));  // UFID not inserted
}

// Additional Test Case 30: Complex Tree Balance Verification
TEST_CASE("Complex Tree Balance Verification" , "[complex_balance]") {
	AVLTree tree;
	// Insert nodes in an order that forces rotations.
	std::vector<int> ids = { 10000050, 10000040, 10000060, 10000030, 10000045, 10000055, 10000070 };
	for (int id : ids) {
		REQUIRE(tree.insert("CB" + intToName(id % 26) , id));
	}
	// The tree should be balanced; expected level count is reasonable.
	REQUIRE(tree.printLevelCount() <= 4);
}

// Additional Test Case 31: Search By Name Returns Empty for Nonexistent Name
TEST_CASE("Search By Name Returns Empty" , "[search_name_empty]") {
	AVLTree tree;
	REQUIRE(tree.insert("AlphaBeta" , 10000018));
	auto results = tree.searchName("NonAlpha");
	REQUIRE(results.empty());
}

// Additional Test Case 32: Insertion With Leading and Trailing Spaces
TEST_CASE("Insertion With Leading and Trailing Spaces" , "[leading_trailing_spaces]") {
	AVLTree tree;
	// Note: if spaces are permitted at the start or end.
	REQUIRE(tree.insert("  TrimTest  " , 10000019));
	REQUIRE(tree.searchId(10000019) == "  TrimTest  ");
}

// Additional Test Case 33: Reverse Order Removal via removeInorder
TEST_CASE("Reverse Order Removal via removeInorder" , "[reverse_removal]") {
	AVLTree tree;
	for (int i = 1; i <= 10; ++i) {
		REQUIRE(tree.insert("Rev" + intToName(i) , 10000000 + i));
	}
	while (!tree.printInorder().empty()) {
		size_t sizeBefore = tree.printInorder().size();
		REQUIRE(tree.removeInorder(sizeBefore - 1));
	}
	REQUIRE(tree.printInorder().empty());
}

// Additional Test Case 34: Random Removal in a Loop Using removeInorder
TEST_CASE("Random Removal in a Loop Using removeInorder" , "[random_removal_loop]") {
	AVLTree tree;
	const int count = 20;
	for (int i = 1; i <= count; ++i) {
		REQUIRE(tree.insert("Rand" + intToName(i) , 10000000 + i));
	}
	std::random_device rd;
	std::mt19937 gen(rd());
	while (!tree.printInorder().empty()) {
		size_t sizeBefore = tree.printInorder().size();
		std::uniform_int_distribution<size_t> indexDist(0 , sizeBefore - 1);
		size_t idx = indexDist(gen);
		REQUIRE(tree.removeInorder(idx));
	}
	REQUIRE(tree.printInorder().empty());
}

// Additional Test Case 35: Insertion and Search with Edge UFIDs
TEST_CASE("Insertion and Search with Edge UFIDs" , "[edge_ufid_search]") {
	AVLTree tree;
	REQUIRE(tree.insert("EdgeMin" , 10000000));
	REQUIRE(tree.insert("EdgeMax" , 99999999));
	REQUIRE(tree.searchId(10000000) == "EdgeMin");
	REQUIRE(tree.searchId(99999999) == "EdgeMax");
}

// Additional Test Case 36: Remove Inorder on Single Node Tree
TEST_CASE("Remove Inorder on Single Node Tree" , "[remove_inorder_single]") {
	AVLTree tree;
	REQUIRE(tree.insert("Solo" , 10000020));
	REQUIRE(tree.printInorder().size() == 1);
	REQUIRE(tree.removeInorder(0));
	REQUIRE(tree.printInorder().empty());
}