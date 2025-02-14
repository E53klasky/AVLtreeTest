#include <catch2/catch_test_macros.hpp>
#include <iostream>
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

std::string intToName(int i) {
	std::string name;
	while (i > 0) {
		--i;
		name.insert(name.begin() , 'A' + (i % 26));
		i /= 26;
	}
	return name;
}


TEST_CASE("Random Removal in a Loop Using removeInorder" , "[random_removal_loop]") {
	AVLTree tree;
	const int count = 20;
	for (int i = 1; i <= count; ++i) {
		char ufid[9];
		std::snprintf(ufid , 9 , "%08d" , 10000000 + i);
		std::string nodeName = "Rand" + intToName(i);
		REQUIRE(tree.insert(nodeName , std::string(ufid)));
	}
	std::random_device rd;
	std::mt19937 gen(rd());
	while (!tree.printInorder().empty()) {
		size_t sizeBefore = tree.printInorder().size();
		std::uniform_int_distribution<size_t> indexDist(0 , sizeBefore - 1);
		size_t idx = indexDist(gen);
		int index = int(idx);
		REQUIRE(tree.removeInorder(index));
	}
	REQUIRE(tree.printInorder().empty());
}

TEST_CASE("Insertion and Search with Edge UFIDs" , "[edge_ufid_search]") {
	AVLTree tree;
	REQUIRE(tree.insert("EdgeMin" , "10000000"));
	REQUIRE(tree.insert("EdgeMax" , "99999999"));
	REQUIRE(tree.searchId(10000000) == "EdgeMin");
	REQUIRE(tree.searchId(99999999) == "EdgeMax");
}

TEST_CASE("Remove Inorder on Single Node Tree" , "[remove_inorder_single]") {
	AVLTree tree;
	REQUIRE(tree.insert("Solo" , "10000020"));
	REQUIRE(tree.printInorder().size() == 1);
	REQUIRE(tree.removeInorder(0));
	REQUIRE(tree.printInorder().empty());
}


// 1. Basic Insert Validation and Edge Cases
//
TEST_CASE("Invalid Insert Commands" , "[invalid_inserts]") {
	AVLTree tree;
	// Assuming names must only contain alphabetic characters (or spaces)
	REQUIRE_FALSE(tree.insert("A11y" , "45679999"));        // invalid name: contains digits
	REQUIRE_FALSE(tree.insert("Bob@" , "10000002"));         // invalid name: contains special character
	// UFID must be exactly 8 digits – these should fail:
	REQUIRE_FALSE(tree.insert("Carol" , "9999"));            // too short
	REQUIRE_FALSE(tree.insert("Dave" , "100000000"));        // too long
	// Empty name is not allowed:
	REQUIRE_FALSE(tree.insert("" , "10000003"));
}

TEST_CASE("Inserting and Searching for a Single Node" , "[single_node]") {
	AVLTree tree;
	REQUIRE(tree.insert("Alice" , "10000001"));
	REQUIRE(tree.searchId(10000001) == "Alice");
}


//////////////////////////////////
// 1. Basic Invalid Input Tests //
//////////////////////////////////
TEST_CASE("Invalid Insert Commands" , "[insert][validation]") {
	AVLTree tree;
	// Name validations:
	SECTION("Reject names with digits or special characters") {
		REQUIRE_FALSE(tree.insert("A11y" , "45679999"));  // Contains digit '1'
		REQUIRE_FALSE(tree.insert("Bob@" , "10000002"));   // Contains '@'
	}
	// UFID validations:
	SECTION("Reject UFIDs of wrong length") {
		REQUIRE_FALSE(tree.insert("Carol" , "9999"));         // Too short
		REQUIRE_FALSE(tree.insert("Dave" , "100000000"));       // Too long
	}
	// Empty name:
	SECTION("Reject empty name") {
		REQUIRE_FALSE(tree.insert("" , "10000003"));
	}
}

/////////////////////////////////////////
// 2. Valid Insert and Search by UFID  //
/////////////////////////////////////////
TEST_CASE("Insertion and Basic Search" , "[insert][search]") {
	AVLTree tree;
	SECTION("Insert valid nodes and search by UFID") {
		// Insert valid entries
		REQUIRE(tree.insert("Alice" , "10000001"));
		REQUIRE(tree.insert("Bob" , "10000002"));
		// searchId takes an int; convert using std::stoi.
		REQUIRE(tree.searchId(std::stoi("10000001")) == "Alice");
		REQUIRE(tree.searchId(std::stoi("10000002")) == "Bob");
		// Searching for a non-existent key should return "unsuccessful"
		REQUIRE(tree.searchId(11111111) == "unsuccessful");
	}
}

////////////////////////////////////////
// 3. Duplicate Insertion Test        //
////////////////////////////////////////
TEST_CASE("Duplicate Insertions" , "[insert][duplicates]") {
	AVLTree tree;
	REQUIRE(tree.insert("Alice" , "10000001"));
	// Re-inserting a node with the same UFID must fail.
	REQUIRE_FALSE(tree.insert("AliceClone" , "10000001"));
}

////////////////////////////////////////
// 4. Removal Tests (By Key)           //
////////////////////////////////////////
TEST_CASE("Insertion and Removal by Key" , "[remove]") {
	AVLTree tree;
	REQUIRE(tree.insert("Eve" , "10000010"));
	// Remove by key (convert UFID to int)
	REQUIRE(tree.remove(std::stoi("10000010")));
	// After removal, search should yield "unsuccessful"
	REQUIRE(tree.searchId(std::stoi("10000010")) == "unsuccessful");
	// Removing the same key again should fail.
	REQUIRE_FALSE(tree.remove(std::stoi("10000010")));
}

//////////////////////////////////////////
// 5. Remove Inorder Tests (By Index)   //
//////////////////////////////////////////
TEST_CASE("Remove Inorder by Index" , "[remove_inorder]") {
	AVLTree tree;
	// Insert three nodes.
	REQUIRE(tree.insert("One" , "10000001"));
	REQUIRE(tree.insert("Two" , "10000002"));
	REQUIRE(tree.insert("Three" , "10000003"));
	// Remove the node at inorder index 1 (which should remove "Two")
	REQUIRE(tree.removeInorder(1));
	auto inorder = tree.printInorder();  // returns std::vector<std::string>
	REQUIRE(inorder.size() == 2);
	// "Two" should now be absent.
	REQUIRE(tree.searchId(std::stoi("10000002")) == "unsuccessful");
}

//////////////////////////////////////////
// 6. Traversal Order Tests             //
//////////////////////////////////////////
TEST_CASE("Traversal Orders" , "[traversals]") {
	AVLTree tree;
	// Insert in an order that is not sorted by UFID.
	REQUIRE(tree.insert("Alpha" , "10000001"));
	REQUIRE(tree.insert("Gamma" , "10000003"));
	REQUIRE(tree.insert("Beta" , "10000002"));

	auto inorder = tree.printInorder();
	std::vector<std::string> expectedInorder = { "Alpha", "Beta", "Gamma" };
	REQUIRE(inorder == expectedInorder);

	auto preorder = tree.printPreorder();
	auto postorder = tree.printPostorder();
	// Verify that the number of nodes in all traversals is consistent.
	REQUIRE(preorder.size() == inorder.size());
	REQUIRE(postorder.size() == inorder.size());
}

//////////////////////////////////////////
// 7. Tree Level Count and Balance      //
//////////////////////////////////////////
TEST_CASE("Tree Level Count on Empty and Non-empty Trees" , "[balance]") {
	AVLTree tree;
	// For an empty tree, level count should be 0.
	REQUIRE(tree.printLevelCount() == 0);

	// Insert nodes to form a small tree.
	REQUIRE(tree.insert("Root" , "10000005"));
	REQUIRE(tree.insert("Left" , "10000003"));
	REQUIRE(tree.insert("Right" , "10000007"));
	int levelCount = tree.printLevelCount();
	// For a balanced AVL tree with 3 nodes, height should be 2 or less.
	REQUIRE(levelCount <= 2);
}

//////////////////////////////////////////
// 8. Search By Name Tests              //
//////////////////////////////////////////
TEST_CASE("Search By Name" , "[search_name]") {
	AVLTree tree;
	// Insert multiple nodes with identical names.
	REQUIRE(tree.insert("Grace" , "10000004"));
	REQUIRE(tree.insert("Grace" , "10000005"));
	std::vector<int> names = tree.searchName("Grace");
	// Expect to find both nodes.
	REQUIRE(names.size() == 2);
	// The returned type is int, so we need to compare with the UFIDs.
	for (const auto& r : names) {
		REQUIRE((r == 10000004 || r == 10000005));
	}

	// Query for a name that doesn't exist.
	auto noResults = tree.searchName("Nonexistent");
	REQUIRE(noResults.empty());
}

////////////////////////////////////////////////////
// 9. Leading and Trailing Spaces in Names Test   //
////////////////////////////////////////////////////
TEST_CASE("Insertion With Leading and Trailing Spaces" , "[insert][trim]") {
	AVLTree tree;
	// Insert a node whose name includes extra spaces.
	REQUIRE(tree.insert("  TrimTest  " , "10000019"));
	// The search should return the name exactly as stored.
	REQUIRE(tree.searchId(std::stoi("10000019")) == "  TrimTest  ");
}

////////////////////////////////////////////////////
// 10. Forced Rotations and Balancing Tests         //
////////////////////////////////////////////////////
TEST_CASE("Forced Rotations and Balancing" , "[balance][rotations]") {
	// Left-Left rotation: Inserting nodes in descending order.
	{
		AVLTree tree;
		REQUIRE(tree.insert("NodeC" , "10000003"));
		REQUIRE(tree.insert("NodeB" , "10000002"));
		REQUIRE(tree.insert("NodeA" , "10000001"));
		auto inorder = tree.printInorder();
		std::vector<std::string> expected = { "NodeA", "NodeB", "NodeC" };
		REQUIRE(inorder == expected);
	}
	// Right-Right rotation: Inserting nodes in ascending order.
	{
		AVLTree tree;
		REQUIRE(tree.insert("NodeA" , "10000001"));
		REQUIRE(tree.insert("NodeB" , "10000002"));
		REQUIRE(tree.insert("NodeC" , "10000003"));
		auto inorder = tree.printInorder();
		std::vector<std::string> expected = { "NodeA", "NodeB", "NodeC" };
		REQUIRE(inorder == expected);
	}
	// (Additional cases for left-right/right-left rotation may be added as needed.)
}

////////////////////////////////////////////////////
// 11. Sequential Removal: Remove Inorder Until Empty
////////////////////////////////////////////////////
TEST_CASE("Sequential Removal (Remove Inorder Until Empty)" , "[remove_all]") {
	AVLTree tree;
	const int totalNodes = 10;
	// Bulk-insert nodes.
	for (int i = 1; i <= totalNodes; ++i) {
		char ufid[9];
		std::snprintf(ufid , sizeof(ufid) , "%08d" , 10000000 + i);
		std::string nodeName = "Node" + intToName(i);
		REQUIRE(tree.insert(nodeName , std::string(ufid)));
	}
	// Remove nodes one by one using removeInorder(0).
	while (!tree.printInorder().empty()) {
		REQUIRE(tree.removeInorder(0));
	}
	REQUIRE(tree.printInorder().empty());
}

////////////////////////////////////////////////////
// 12. Random Removal Loop Using removeInorder
////////////////////////////////////////////////////
TEST_CASE("Random Removal Loop Until Empty" , "[random_removal]") {
	AVLTree tree;
	const int count = 20;
	// Insert a series of nodes.
	for (int i = 1; i <= count; ++i) {
		char ufid[9];
		std::snprintf(ufid , sizeof(ufid) , "%08d" , 10000000 + i);
		std::string name = "Rand" + intToName(i);
		REQUIRE(tree.insert(name , std::string(ufid)));
	}
	std::random_device rd;
	std::mt19937 gen(rd());
	// Randomly remove nodes using removeInorder until the tree is empty.
	while (!tree.printInorder().empty()) {
		size_t sizeBefore = tree.printInorder().size();
		std::uniform_int_distribution<size_t> dist(0 , sizeBefore - 1);
		size_t idx = dist(gen);
		REQUIRE(tree.removeInorder(idx));
	}
	REQUIRE(tree.printInorder().empty());
}

////////////////////////////////////////////////////
// 13. Bulk Insertion and Random Removal Stress Test //
////////////////////////////////////////////////////
TEST_CASE("Bulk Insertion and Random Removal Stress Test" , "[bulk][stress][random]") {
	AVLTree tree;
	const int totalNodes = 100;
	std::vector<std::string> ufidList;
	// Bulk insert nodes.
	for (int i = 1; i <= totalNodes; ++i) {
		char ufid[9];
		std::snprintf(ufid , sizeof(ufid) , "%08d" , 10000000 + i);
		ufidList.push_back(std::string(ufid));
		std::string name = "Student" + intToName(i);
		REQUIRE(tree.insert(name , ufidList.back()));
	}
	REQUIRE(tree.printInorder().size() == static_cast<size_t>(totalNodes));

	// Randomly remove 50 nodes.
	std::random_device rd;
	std::mt19937 gen(rd());
	for (int i = 0; i < 50; ++i) {
		if (!ufidList.empty()) {
			std::uniform_int_distribution<size_t> dist(0 , ufidList.size() - 1);
			size_t index = dist(gen);
			REQUIRE(tree.remove(std::stoi(ufidList[index])));
			ufidList.erase(ufidList.begin() + index);
		}
	}
	// Validate consistency: remaining nodes count should match.
	size_t remaining = tree.printInorder().size();
	REQUIRE(remaining == ufidList.size());
}

////////////////////////////////////////////////////////////
// 14. Mixed Randomized Insertions and Removals Test
////////////////////////////////////////////////////////////
TEST_CASE("Mixed Randomized Insertions and Removals" , "[mixed][random]") {
	AVLTree tree;
	const int iterations = 200;
	std::vector<std::string> activeUFIDs;
	std::random_device rd;
	std::mt19937 gen(rd());

	for (int i = 0; i < iterations; ++i) {
		// Randomly choose an action: 0 = insert, 1 = remove (if any exist)
		std::uniform_int_distribution<int> actionDist(0 , 1);
		int action = actionDist(gen);
		if (action == 0 || activeUFIDs.empty()) { // Force insertion if tree is empty.
			int id = 10000000 + static_cast<int>(activeUFIDs.size() + 1);
			char ufid[9];
			std::snprintf(ufid , sizeof(ufid) , "%08d" , id);
			std::string name = "Mixed" + intToName(id % 100);
			if (tree.insert(name , std::string(ufid)))
				activeUFIDs.push_back(std::string(ufid));
		}
		else {  // Removal action.
			std::uniform_int_distribution<size_t> dist(0 , activeUFIDs.size() - 1);
			size_t index = dist(gen);
			int intKey = std::stoi(activeUFIDs[index]);
			if (tree.remove(intKey))
				activeUFIDs.erase(activeUFIDs.begin() + index);
		}
	}
	// Final consistency check: each remaining UFID should be found.
	for (const auto& ufid : activeUFIDs)
		REQUIRE(tree.searchId(std::stoi(ufid)) != "unsuccessful");
}