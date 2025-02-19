// #include <catch2/catch_test_macros.hpp>
// #include <iostream>
// #include <vector>
// #include <string>
// #include <functional>
// #include <random>
// #include <algorithm>
// #include <iterator>
// #include <set>
// #include "../src/AVLTree.h"
// #include "../src/Student.h"
// #include "../src/AVLNODE.h"

// using namespace std;



// std::string intToName(int i) {
//     std::string name;
//     while (i > 0) {
//         --i;
//         name.insert(name.begin() , 'A' + (i % 26));
//         i /= 26;
//     }
//     return name;
// }

// TEST_CASE("Test incorrect commands" , "[invalid_commands]") {
//     AVLTree tree;


//     REQUIRE_FALSE(tree.insert("A11y" , "45679999"));


//     REQUIRE_FALSE(tree.insert("Bob@Smith" , "12345678"));

//     REQUIRE_FALSE(tree.insert("Carol" , "1234567"));

//     REQUIRE_FALSE(tree.insert("Dave" , "123456789"));

//     REQUIRE_FALSE(tree.insert("" , "12345678"));
// }

// TEST_CASE("Test edge cases" , "[edge_cases]") {
//     AVLTree tree;

//     REQUIRE_FALSE(tree.remove(12345678));

//     REQUIRE(tree.searchId(12345678) == "unsuccessful");

//     REQUIRE_FALSE(tree.removeInorder(0));

//     REQUIRE(tree.insert("Test" , "12345678"));

//     REQUIRE_FALSE(tree.removeInorder(1));
// }

// TEST_CASE("Test all rotation cases" , "[rotations]") {
//     AVLTree tree;


//     REQUIRE(tree.insert("NodeC" , "30000000"));
//     REQUIRE(tree.insert("NodeB" , "20000000"));
//     REQUIRE(tree.insert("NodeA" , "10000000"));
//     auto preorder1 = tree.printPreorder();
//     REQUIRE(preorder1[0] == "NodeB");

//     tree = AVLTree();

//     REQUIRE(tree.insert("NodeA" , "10000000"));
//     REQUIRE(tree.insert("NodeB" , "20000000"));
//     REQUIRE(tree.insert("NodeC" , "30000000"));
//     auto preorder2 = tree.printPreorder();
//     REQUIRE(preorder2[0] == "NodeB");

//     tree = AVLTree();


//     REQUIRE(tree.insert("NodeC" , "30000000"));
//     REQUIRE(tree.insert("NodeA" , "10000000"));
//     REQUIRE(tree.insert("NodeB" , "20000000"));
//     auto preorder3 = tree.printPreorder();
//     REQUIRE(preorder3[0] == "NodeB");
//     tree = AVLTree();


//     REQUIRE(tree.insert("NodeA" , "10000000"));
//     REQUIRE(tree.insert("NodeC" , "30000000"));
//     REQUIRE(tree.insert("NodeB" , "20000000"));
//     auto preorder4 = tree.printPreorder();
//     REQUIRE(preorder4[0] == "NodeB");
// }

// TEST_CASE("Test all deletion cases" , "[deletion]") {
//     AVLTree tree;

//     REQUIRE(tree.insert("NodeB" , "20000000"));
//     REQUIRE(tree.insert("NodeA" , "10000000"));
//     REQUIRE(tree.insert("NodeC" , "30000000"));

//     REQUIRE(tree.remove(10000000));
//     REQUIRE(tree.searchId(10000000) == "unsuccessful");

//     REQUIRE(tree.insert("NodeD" , "40000000"));
//     REQUIRE(tree.remove(30000000));
//     REQUIRE(tree.searchId(30000000) == "unsuccessful");
//     REQUIRE(tree.searchId(40000000) == "NodeD");

//     REQUIRE(tree.insert("NodeE" , "25000000"));
//     REQUIRE(tree.insert("NodeF" , "22000000"));
//     REQUIRE(tree.remove(20000000));
//     REQUIRE(tree.searchId(20000000) == "unsuccessful");
//     auto inorder = tree.printInorder();
//     REQUIRE(inorder.size() == 3);
// }

// TEST_CASE("Large-scale insertion and random deletion test" , "[large_scale]") {
//     AVLTree tree;
//     std::vector<int> inserted_ids;

//     for (int i = 1; i <= 100; i++) {
//         char ufid[9];
//         std::snprintf(ufid , 9 , "%08d" , 10000000 + i);
//         std::string nodeName = "Node" + intToName(i);
//         REQUIRE(tree.insert(nodeName , std::string(ufid)));
//         inserted_ids.push_back(10000000 + i);
//     }

//     auto initial_inorder = tree.printInorder();
//     REQUIRE(initial_inorder.size() == 100);

//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::vector<int> removed_ids;

//     for (int i = 0; i < 10; i++) {
//         std::uniform_int_distribution<> dis(0 , inserted_ids.size() - 1);
//         int index = dis(gen);
//         int id_to_remove = inserted_ids[index];
//         REQUIRE(tree.remove(id_to_remove));
//         removed_ids.push_back(id_to_remove);
//         inserted_ids.erase(inserted_ids.begin() + index);
//     }

//     auto final_inorder = tree.printInorder();
//     REQUIRE(final_inorder.size() == 90);

//     for (int id : inserted_ids) {
//         REQUIRE(tree.searchId(id) != "unsuccessful");
//     }

//     for (int id : removed_ids) {
//         REQUIRE(tree.searchId(id) == "unsuccessful");
//     }
// }