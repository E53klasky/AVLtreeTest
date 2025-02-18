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



TEST_CASE("Invalid Insert Commands" , "[invalid_inserts]") {
    AVLTree tree;

    REQUIRE_FALSE(tree.insert("A11y" , "45679999"));
    REQUIRE_FALSE(tree.insert("Bob@" , "10000002"));
    REQUIRE_FALSE(tree.insert("Carol" , "9999"));
    REQUIRE_FALSE(tree.insert("Dave" , "100000000"));
    REQUIRE_FALSE(tree.insert("" , "10000003"));
}

TEST_CASE("Inserting and Searching for a Single Node" , "[single_node]") {
    AVLTree tree;
    REQUIRE(tree.insert("Alice" , "10000001"));
    REQUIRE(tree.searchId(10000001) == "Alice");
}


TEST_CASE("Invalid Insert Commands" , "[insert][validation]") {
    AVLTree tree;
    SECTION("Reject names with digits or special characters") {
        REQUIRE_FALSE(tree.insert("A11y" , "45679999"));
        REQUIRE_FALSE(tree.insert("Bob@" , "10000002"));
    }

    SECTION("Reject UFIDs of wrong length") {
        REQUIRE_FALSE(tree.insert("Carol" , "9999"));
        REQUIRE_FALSE(tree.insert("Dave" , "100000000"));
    }
    SECTION("Reject empty name") {
        REQUIRE_FALSE(tree.insert("" , "10000003"));
    }
}


TEST_CASE("Insertion and Basic Search" , "[insert][search]") {
    AVLTree tree;
    SECTION("Insert valid nodes and search by UFID") {
        REQUIRE(tree.insert("Alice" , "10000001"));
        REQUIRE(tree.insert("Bob" , "10000002"));
        REQUIRE(tree.searchId(std::stoi("10000001")) == "Alice");
        REQUIRE(tree.searchId(std::stoi("10000002")) == "Bob");
        REQUIRE(tree.searchId(11111111) == "unsuccessful");
    }
}


TEST_CASE("Duplicate Insertions" , "[insert][duplicates]") {
    AVLTree tree;
    REQUIRE(tree.insert("Alice" , "10000001"));
    REQUIRE_FALSE(tree.insert("AliceClone" , "10000001"));
}

TEST_CASE("Insertion and Removal by Key" , "[remove]") {
    AVLTree tree;
    REQUIRE(tree.insert("Eve" , "10000010"));
    REQUIRE(tree.remove(std::stoi("10000010")));
    REQUIRE(tree.searchId(std::stoi("10000010")) == "unsuccessful");
    REQUIRE_FALSE(tree.remove(std::stoi("10000010")));
}

TEST_CASE("Remove Inorder by Index" , "[remove_inorder]") {
    AVLTree tree;
    REQUIRE(tree.insert("One" , "10000001"));
    REQUIRE(tree.insert("Two" , "10000002"));
    REQUIRE(tree.insert("Three" , "10000003"));
    REQUIRE(tree.removeInorder(1));
    auto inorder = tree.printInorder();
    REQUIRE(inorder.size() == 2);
    REQUIRE(tree.searchId(std::stoi("10000002")) == "unsuccessful");
}


TEST_CASE("Traversal Orders" , "[traversals]") {
    AVLTree tree;
    REQUIRE(tree.insert("Alpha" , "10000001"));
    REQUIRE(tree.insert("Gamma" , "10000003"));
    REQUIRE(tree.insert("Beta" , "10000002"));

    auto inorder = tree.printInorder();
    std::vector<std::string> expectedInorder = { "Alpha", "Beta", "Gamma" };
    REQUIRE(inorder == expectedInorder);

    auto preorder = tree.printPreorder();
    auto postorder = tree.printPostorder();
    REQUIRE(preorder.size() == inorder.size());
    REQUIRE(postorder.size() == inorder.size());
}

TEST_CASE("Tree Level Count on Empty and Non-empty Trees" , "[balance]") {
    AVLTree tree;
    REQUIRE(tree.printLevelCount() == 0);
    REQUIRE(tree.insert("Root" , "10000005"));
    REQUIRE(tree.insert("Left" , "10000003"));
    REQUIRE(tree.insert("Right" , "10000007"));
    int levelCount = tree.printLevelCount();
    REQUIRE(levelCount <= 2);
}

TEST_CASE("Search By Name" , "[search_name]") {
    AVLTree tree;
    REQUIRE(tree.insert("Grace" , "10000004"));
    REQUIRE(tree.insert("Grace" , "10000005"));
    std::vector<int> names = tree.searchName("Grace");
    REQUIRE(names.size() == 2);
    for (const auto& r : names) {
        REQUIRE((r == 10000004 || r == 10000005));
    }

    auto noResults = tree.searchName("Nonexistent");
    REQUIRE(noResults.empty());
}

TEST_CASE("Insertion With Leading and Trailing Spaces" , "[insert][trim]") {
    AVLTree tree;
    REQUIRE(tree.insert("  TrimTest  " , "10000019"));
    REQUIRE(tree.searchId(std::stoi("10000019")) == "  TrimTest  ");
}


TEST_CASE("Forced Rotations and Balancing" , "[balance][rotations]") {
    {
        AVLTree tree;
        REQUIRE(tree.insert("NodeC" , "10000003"));
        REQUIRE(tree.insert("NodeB" , "10000002"));
        REQUIRE(tree.insert("NodeA" , "10000001"));
        auto inorder = tree.printInorder();
        std::vector<std::string> expected = { "NodeA", "NodeB", "NodeC" };
        REQUIRE(inorder == expected);
    }
    {
        AVLTree tree;
        REQUIRE(tree.insert("NodeA" , "10000001"));
        REQUIRE(tree.insert("NodeB" , "10000002"));
        REQUIRE(tree.insert("NodeC" , "10000003"));
        auto inorder = tree.printInorder();
        std::vector<std::string> expected = { "NodeA", "NodeB", "NodeC" };
        REQUIRE(inorder == expected);
    }
}


TEST_CASE("Sequential Removal (Remove Inorder Until Empty)" , "[remove_all]") {
    AVLTree tree;
    const int totalNodes = 1000;
    for (int i = 1; i <= totalNodes; ++i) {
        char ufid[9];
        std::snprintf(ufid , sizeof(ufid) , "%08d" , 10000000 + i);
        std::string nodeName = "Node" + intToName(i);
        REQUIRE(tree.insert(nodeName , std::string(ufid)));
    }
    while (!tree.printInorder().empty()) {
        REQUIRE(tree.removeInorder(0));
    }
    REQUIRE(tree.printInorder().empty());
}

TEST_CASE("Random Removal Loop Until Empty" , "[random_removal]") {
    AVLTree tree;
    const int count = 20;
    for (int i = 1; i <= count; ++i) {
        char ufid[9];
        std::snprintf(ufid , sizeof(ufid) , "%08d" , 10000000 + i);
        std::string name = "Rand" + intToName(i);
        REQUIRE(tree.insert(name , std::string(ufid)));
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    while (!tree.printInorder().empty()) {
        size_t sizeBefore = tree.printInorder().size();
        std::uniform_int_distribution<size_t> dist(0 , sizeBefore - 1);
        size_t idx = dist(gen);
        REQUIRE(tree.removeInorder(idx));
    }
    REQUIRE(tree.printInorder().empty());
}


TEST_CASE("edge case")
{
    AVLTree tree;
    tree.insert("ben" , "00000001");
    tree.insert("ben" , "00000002");
    tree.insert("ben" , "00000003");
    tree.insert("ben" , "00000004");
    tree.insert("ben" , "00000005");
    tree.insert("ben" , "00000006");
    tree.insert("ben" , "00000007");
    tree.insert("ben" , "00000008");
    tree.insert("ben" , "00000009");
    tree.insert("ben" , "00000010");
    tree.insert("ben" , "00000011");
    tree.insert("ben" , "00000012");
    tree.insert("ben" , "00000013");
    tree.insert("ben" , "00000014");
    tree.insert("ben" , "00000015");
    tree.insert("ben" , "00000016");
    tree.insert("ben" , "00000001");
    tree.insert("ben" , "00000001");

    REQUIRE(tree.searchId(1) == "ben");
    REQUIRE(tree.searchId(2) == "ben");
    REQUIRE(tree.searchId(3) == "ben");
    REQUIRE(tree.searchId(4) == "ben");
    REQUIRE(tree.searchId(5) == "ben");
    REQUIRE(tree.searchId(6) == "ben");
    REQUIRE(tree.searchId(7) == "ben");
    REQUIRE(tree.searchId(8) == "ben");
    REQUIRE(tree.searchId(9) == "ben");
    REQUIRE(tree.searchId(10) == "ben");
    REQUIRE(tree.searchId(11) == "ben");
    REQUIRE(tree.searchId(12) == "ben");
    REQUIRE(tree.searchId(13) == "ben");
    REQUIRE(tree.searchId(14) == "ben");
    REQUIRE(tree.searchId(15) == "ben");
    REQUIRE(tree.searchId(16) == "ben");


}
