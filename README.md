### Introduction

This C program demonstrates a binary search tree. The program reads information from an input file and places it into a binary search tree. A left child node is less than its parent, and a right child node is greater than its parent. This tree does not allow duplicate values (which is shown in the program). After the tree has been initialized, the tree is displayed using iterative preorder, postorder, and inorder traversal methods. The user is then prompted to delete a node from the tree by inputting their values. If the node exists in the tree, then it will be deleted. The user will also be notified if the node does not exist in the tree. The user can enter a blank line when they want to stop deleting nodes from the tree. The tree is printed using iterative inorder traversal after each deletion. The tree is displayed one last time after the user decides to stop deleting nodes. The user's actions are recorded in an output file.

This program also includes recursive traversal functions for completeness, but they are not implemented in the program.

The when the binary search tree has been built, it will look as follows:

<img width="1032" alt="screen shot 2017-10-15 at 4 17 05 pm" src="https://user-images.githubusercontent.com/30678180/37878434-4babe16c-301e-11e8-8b75-262642dcd9fe.png">

### Usage

Open Terminal in the current folder holding `binarySearchTree.c` and `inputFile.txt`.

Compile the program using: `gcc -o bst binarySearchTree.c`

Run the program using: `bst inputFile.txt outputFile.txt`

An output file called `outputFile.txt` will be created in the current folder if one does not already exist. The file will have recorded the actions made to the binary search tree.

### Other Comments

To me, the most challenging part of this project was creating the iterative preorder, inorder, and postorder traversal functions. The iterative preorder and inorder traversal functions require a stack data structure, while the postorder traversal function requires two stack data structures.
