#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <random>
#include <algorithm>
#include <iterator>
#include <set>
#include "../src/AVLTree.h"
#include "../src/Student.h"
#include "../src/AVLNODE.h"

using namespace std;


// Helper function to convert integer to string name
std::string intToName(int i) {
    std::string name;
    while (i > 0) {
        --i;
        name.insert(name.begin() , 'A' + (i % 26));
        i /= 26;
    }
    return name;
}

TEST_CASE("Test incorrect commands" , "[invalid_commands]") {
    AVLTree tree;

    // Test 1: Invalid name with numbers
    REQUIRE_FALSE(tree.insert("A11y" , "45679999"));

    // Test 2: Invalid name with special characters
    REQUIRE_FALSE(tree.insert("Bob@Smith" , "12345678"));

    // Test 3: UFID too short
    REQUIRE_FALSE(tree.insert("Carol" , "1234567"));

    // Test 4: UFID too long
    REQUIRE_FALSE(tree.insert("Dave" , "123456789"));

    // Test 5: Empty name
    REQUIRE_FALSE(tree.insert("" , "12345678"));
}

TEST_CASE("Test edge cases" , "[edge_cases]") {
    AVLTree tree;

    // Edge case 1: Remove from empty tree
    REQUIRE_FALSE(tree.remove(12345678));

    // Edge case 2: Search in empty tree
    REQUIRE(tree.searchId(12345678) == "unsuccessful");

    // Edge case 3: Remove inorder from empty tree
    REQUIRE_FALSE(tree.removeInorder(0));

    // Setup for next edge case
    REQUIRE(tree.insert("Test" , "12345678"));

    // Edge case 4: Remove index out of bounds
    REQUIRE_FALSE(tree.removeInorder(1));
}

TEST_CASE("Test all rotation cases" , "[rotations]") {
    AVLTree tree;

    // Left-Left case
    REQUIRE(tree.insert("NodeC" , "30000000"));
    REQUIRE(tree.insert("NodeB" , "20000000"));
    REQUIRE(tree.insert("NodeA" , "10000000"));
    auto preorder1 = tree.printPreorder();
    REQUIRE(preorder1[0] == "NodeB"); // NodeB should be root after rotation

    tree = AVLTree(); // Reset tree

    // Right-Right case
    REQUIRE(tree.insert("NodeA" , "10000000"));
    REQUIRE(tree.insert("NodeB" , "20000000"));
    REQUIRE(tree.insert("NodeC" , "30000000"));
    auto preorder2 = tree.printPreorder();
    REQUIRE(preorder2[0] == "NodeB"); // NodeB should be root after rotation

    tree = AVLTree(); // Reset tree

    // Left-Right case
    REQUIRE(tree.insert("NodeC" , "30000000"));
    REQUIRE(tree.insert("NodeA" , "10000000"));
    REQUIRE(tree.insert("NodeB" , "20000000"));
    auto preorder3 = tree.printPreorder();
    REQUIRE(preorder3[0] == "NodeB"); // NodeB should be root after rotation

    tree = AVLTree(); // Reset tree

    // Right-Left case
    REQUIRE(tree.insert("NodeA" , "10000000"));
    REQUIRE(tree.insert("NodeC" , "30000000"));
    REQUIRE(tree.insert("NodeB" , "20000000"));
    auto preorder4 = tree.printPreorder();
    REQUIRE(preorder4[0] == "NodeB"); // NodeB should be root after rotation
}

TEST_CASE("Test all deletion cases" , "[deletion]") {
    AVLTree tree;

    // Setup for deletion tests
    REQUIRE(tree.insert("NodeB" , "20000000")); // Will be root
    REQUIRE(tree.insert("NodeA" , "10000000")); // Left child
    REQUIRE(tree.insert("NodeC" , "30000000")); // Right child

    // Case 1: Delete leaf node (no children)
    REQUIRE(tree.remove(10000000)); // Remove NodeA
    REQUIRE(tree.searchId(10000000) == "unsuccessful");

    // Case 2: Delete node with one child
    REQUIRE(tree.insert("NodeD" , "40000000")); // Add right child to NodeC
    REQUIRE(tree.remove(30000000)); // Remove NodeC
    REQUIRE(tree.searchId(30000000) == "unsuccessful");
    REQUIRE(tree.searchId(40000000) == "NodeD"); // NodeD should still be in tree

    // Case 3: Delete node with two children
    REQUIRE(tree.insert("NodeE" , "25000000")); // Add between NodeB and NodeD
    REQUIRE(tree.insert("NodeF" , "22000000")); // Add left child to NodeE
    REQUIRE(tree.remove(20000000)); // Remove root (NodeB)
    REQUIRE(tree.searchId(20000000) == "unsuccessful");
    auto inorder = tree.printInorder();
    REQUIRE(inorder.size() == 3); // Should still have 3 nodes
}

TEST_CASE("Large-scale insertion and random deletion test" , "[large_scale]") {
    AVLTree tree;
    std::vector<int> inserted_ids;

    // Insert 100 nodes
    for (int i = 1; i <= 100; i++) {
        char ufid[9];
        std::snprintf(ufid , 9 , "%08d" , 10000000 + i);
        std::string nodeName = "Node" + intToName(i);
        REQUIRE(tree.insert(nodeName , std::string(ufid)));
        inserted_ids.push_back(10000000 + i);
    }

    // Verify 100 insertions
    auto initial_inorder = tree.printInorder();
    REQUIRE(initial_inorder.size() == 100);

    // Remove 10 random nodes
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> removed_ids;

    for (int i = 0; i < 10; i++) {
        std::uniform_int_distribution<> dis(0 , inserted_ids.size() - 1);
        int index = dis(gen);
        int id_to_remove = inserted_ids[index];
        REQUIRE(tree.remove(id_to_remove));
        removed_ids.push_back(id_to_remove);
        inserted_ids.erase(inserted_ids.begin() + index);
    }

    // Verify 90 nodes remain
    auto final_inorder = tree.printInorder();
    REQUIRE(final_inorder.size() == 90);

    // Verify all remaining IDs are still in tree
    for (int id : inserted_ids) {
        REQUIRE(tree.searchId(id) != "unsuccessful");
    }

    // Verify removed IDs are not in tree
    for (int id : removed_ids) {
        REQUIRE(tree.searchId(id) == "unsuccessful");
    }
}