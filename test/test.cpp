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



// TEST_CASE("Test UFIDs with leading zeros" , "[leading_zeros]") {
//     AVLTree tree;

//     REQUIRE(tree.insert("StudentOne" , "00000001"));
//     REQUIRE(tree.insert("StudentTwo" , "00000010"));
//     REQUIRE(tree.insert("StudentThree" , "00000100"));

//     REQUIRE(tree.searchId(1) == "StudentOne");
//     REQUIRE(tree.searchId(10) == "StudentTwo");
//     REQUIRE(tree.searchId(100) == "StudentThree");

//     REQUIRE(tree.remove(1));
//     REQUIRE(tree.searchId(1) == "unsuccessful");
//     REQUIRE(tree.remove(10));
//     REQUIRE(tree.searchId(10) == "unsuccessful");
// }


// TEST_CASE("Test edge cases for removeInorder" , "[remove_inorder_edge]") {
//     AVLTree tree;

//     REQUIRE_FALSE(tree.removeInorder(0));

//     REQUIRE_FALSE(tree.removeInorder(-1));


//     REQUIRE(tree.insert("First" , "00000001"));
//     REQUIRE(tree.insert("Second" , "00000002"));
//     REQUIRE(tree.insert("Third" , "00000003"));

//     REQUIRE_FALSE(tree.removeInorder(3));

//     REQUIRE(tree.removeInorder(1));
//     auto inorder = tree.printInorder();
//     REQUIRE(inorder.size() == 2);
// }


// TEST_CASE("Test all traversal methods consistency" , "[traversal_consistency]") {
//     AVLTree tree;

//     REQUIRE(tree.insert("A" , "00000001"));
//     REQUIRE(tree.insert("B" , "00000002"));
//     REQUIRE(tree.insert("C" , "00000003"));
//     REQUIRE(tree.insert("D" , "00000004"));
//     REQUIRE(tree.insert("E" , "00000005"));

//     auto inorder = tree.printInorder();
//     auto preorder = tree.printPreorder();
//     auto postorder = tree.printPostorder();

//     REQUIRE(inorder.size() == 5);
//     REQUIRE(preorder.size() == 5);
//     REQUIRE(postorder.size() == 5);

//     std::set<std::string> inorder_set(inorder.begin() , inorder.end());
//     std::set<std::string> preorder_set(preorder.begin() , preorder.end());
//     std::set<std::string> postorder_set(postorder.begin() , postorder.end());

//     REQUIRE(inorder_set == preorder_set);
//     REQUIRE(preorder_set == postorder_set);
// }

// TEST_CASE("Test invalid input handling" , "[invalid_input]") {
//     AVLTree tree;

//     REQUIRE_FALSE(tree.insert("Test" , "1234567"));
//     REQUIRE_FALSE(tree.insert("Test" , "123456789"));
//     REQUIRE_FALSE(tree.insert("Test" , "abcd1234"));
//     REQUIRE_FALSE(tree.insert("Test" , "0000000a"));

//     REQUIRE_FALSE(tree.insert("Test123" , "12345678"));
//     REQUIRE_FALSE(tree.insert("Test@Name" , "12345678"));
//     REQUIRE_FALSE(tree.insert("" , "12345678"));

//     REQUIRE(tree.insert("First" , "12345678"));
//     REQUIRE_FALSE(tree.insert("Second" , "12345678"));
// }

