/***********************************************************************************************************
//
//  main.c
//  ImplementingBinaryTree
//
//  Created by Kevin Geiszler on 10/14/17.
//  Copyright © 2017 home. All rights reserved.
//

This program demonstrates a binary search tree. The program reads information from an
input file, and places it into a binary search tree. A left child node is less than its
parent, and a right child node is greater than its parent. This tree does not allow
duplicate values (which is shown in the program). After the tree has been initialized, the
tree is displayed using iterative preorder, postorder, and inorder traversal methods. The
user is then prompted to delete a node from the tree by inputting their values. If the
node exists in the tree, then it will be deleted. The user will also be notified if the
node does not exist in the tree. The user can enter a blank line when they want to stop
deleting nodes from the tree. 


***********************************************************************************************************/

// Preprocessor directives

#include <stdio.h>
#include <stdlib.h>

#define MAXSTACK 50
#define EMPTYSTACK -1
#define TRUE 1
#define FALSE 0

struct treeNode
{
    int num;
    struct treeNode *pLeft;
    struct treeNode *pRight;
};

struct stack
{
    int top;
    struct treeNode *stackElement[MAXSTACK];
};

typedef struct stack STACK;
typedef struct treeNode TREENODE;
typedef TREENODE * TREENODEPTR;

/**********************************************************************************************************/

// Function prototypes

int isEmpty(STACK *pStack);
TREENODEPTR pop(STACK *pStack);
void push(STACK *pStack, TREENODEPTR num);
TREENODEPTR getTreeNode();
void insert(FILE *pOutputFile, TREENODEPTR *ppRoot, int value);
void initializeTree(FILE *pInputFile, FILE *pOutputFile, TREENODEPTR *ppRoot);
void recursiveInorderTraversal(FILE *pOutputFile, TREENODEPTR pRoot);
void recursivePreorderTraversal(FILE *pOutputFile, TREENODEPTR pRoot);
void recursivePostorderTraversal(FILE *pOutputFile, TREENODEPTR pRoot);
void iterativeInorderTraversal(FILE *pOutputFile, TREENODEPTR pRoot);
void iterativePreorderTraversal(FILE *pOutputFile, TREENODEPTR pRoot);
void iterativePostorderTraversal(FILE *pOutputFile, TREENODEPTR pRoot);
TREENODEPTR search(TREENODEPTR pRoot, int value);
TREENODEPTR getMin(FILE *pOutputFile, TREENODEPTR pRoot);
TREENODEPTR deleteNode(FILE *pOutputFile, TREENODEPTR pRoot, int value);
void promptToDelete(FILE *pOut, TREENODEPTR ppRoot);
void displayTraversals(FILE *pOutputFile, TREENODEPTR pRoot);

/**********************************************************************************************************/

// The main function

int main(int argc, const char * argv[]) {

    FILE *pInputFile, *pOutputFile;
    
    TREENODEPTR pRoot = NULL;
    
    // Check command line arguments
    if (argc != 3)
    {
        printf("Error, need the following file usage: %s inputFile outputFile\n", argv[0]);
        exit(1);
    }
    
    // Open input file
    if ((pInputFile = fopen(argv[1], "r")) == NULL)
    {
        printf("Error opening input file %s.\n", argv[1]);
        exit(1);
    }
    
    // Open output file
    if ((pOutputFile = fopen(argv[2], "w")) == NULL)
    {
        printf("Error opening output file %s.\n", argv[2]);
        exit(1);
    }
    
    rewind(pInputFile);
    
    printf("Reading information from input file...\n\n");
    
    // Scan input file and add items to the binary tree
    initializeTree(pInputFile, pOutputFile, &pRoot);
    
    fprintf(pOutputFile, "\n");
    
    // Input file is no longer needed
    fclose(pInputFile);
    
    // Display the tree using iterative preorder, postorder, and inorder traversal methods
    displayTraversals(pOutputFile, pRoot);
    
    // Promt the user the delete node from the tree
    promptToDelete(pOutputFile, pRoot);
    
    // Final message before the program ends
    fprintf(pOutputFile, "Here is the final tree using an iterative inorder method:\n\n");
    printf("Here is the final tree using an iterative inorder method:\n\n");
    iterativeInorderTraversal(pOutputFile, pRoot);
    fprintf(pOutputFile, "\n\n");
    printf("\n\n");
    
    fclose(pOutputFile);
    
    return 0;
}

/**********************************************************************************************************/

// Verifies if the stack is empty

int isEmpty(STACK *pStack)
{
    if (pStack->top == EMPTYSTACK)
        return(TRUE);
    else
        return(FALSE);
}

/**********************************************************************************************************/

// Removes an the item that is at the top of the stack and returns it

TREENODEPTR pop(STACK *pStack)
{
    if (isEmpty(pStack))
    {
        printf("%s", "stack underflow");
        exit(1);
    } //end if
    
    return (pStack->stackElement[(pStack->top)--]);
}

/**********************************************************************************************************/

// Places an item at the top of the stack

void push(STACK *pStack, TREENODEPTR pNode)
{
    if (pStack->top == MAXSTACK - 1)
    {
        printf("%s", "stack overflow");
        exit(1);
    } //end if
    else
        pStack->stackElement[++(pStack->top)] = pNode;
}

/**********************************************************************************************************/

// Creates a node

TREENODEPTR getTreeNode()
{
    TREENODEPTR pNewNode = (TREENODEPTR)malloc(sizeof(TREENODE));
    return pNewNode;
}

/**********************************************************************************************************/

// Recursively inserts a node into the tree.
// This tree does not allow duplicate values.

void insert(FILE *pOutputFile, TREENODEPTR *ppRoot, int value)
{
    TREENODEPTR pNewNode = getTreeNode();
    
    if (pNewNode != NULL)
    {
        pNewNode->num = value;
        pNewNode->pLeft = pNewNode->pRight = NULL;
    }
    else
    {
        printf("Error, not enough memory to create a node.\n\n");
        return;
    }
    
    // If the tree empty, then set the root to the new node
    if (*ppRoot == NULL)
    {
        *ppRoot = pNewNode;
        return;
    }

    // Place the node into the tree
    if (pNewNode->num < (*ppRoot)->num)
        insert(pOutputFile, &(*ppRoot)->pLeft, value);
    else if (pNewNode->num > (*ppRoot)->num)
        insert(pOutputFile, &(*ppRoot)->pRight, value);
    else
    {
        fprintf(pOutputFile, "%-3d cannot be added to the tree because it already exists.\n", value);
        printf("Cannot add %d because it already exists.\n\n", value);
    }

}

/**********************************************************************************************************/

// Reads the input file and fills the tree with each piece of data.

void initializeTree(FILE *pInputFile, FILE *pOutputFile, TREENODEPTR *ppRoot)
{
    int num;
    
    while(fscanf(pInputFile, "%d,", &num) == 1)
    {
        insert(pOutputFile, ppRoot, num);
        fprintf(pOutputFile, "%-3d has been read from the input file and inserted into the tree.\n", num);
    }
}

/**********************************************************************************************************/

// Recursively traversesthe tree and prinst each node in an inorder manner.

void recursiveInorderTraversal(FILE *pOutputFile, TREENODEPTR pRoot)
{
    if (pRoot != NULL)
    {
        recursiveInorderTraversal(pOutputFile, pRoot->pLeft);
        fprintf(pOutputFile, "%d ", pRoot->num);
        printf("%d ", pRoot->num);
        recursiveInorderTraversal(pOutputFile, pRoot->pRight);
    }
}

/**********************************************************************************************************/

// Recursively traverses the tree and prints each node in a preorder manner.

void recursivePreorderTraversal(FILE *pOutputFile, TREENODEPTR pRoot)
{
    if (pRoot != NULL)
    {
        fprintf(pOutputFile, "%d ", pRoot->num);
        printf("%d ", pRoot->num);
        recursivePreorderTraversal(pOutputFile, pRoot->pLeft);
        recursivePreorderTraversal(pOutputFile, pRoot->pRight);
    }
}

/**********************************************************************************************************/

// Recursively traverses the tree and prints each node in a postorder manner.

void recursivePostorderTraversal(FILE *pOutputFile, TREENODEPTR pRoot)
{
    if (pRoot != NULL)
    {
        recursivePostorderTraversal(pOutputFile, pRoot->pLeft);
        recursivePostorderTraversal(pOutputFile, pRoot->pRight);
        fprintf(pOutputFile, "%d ", pRoot->num);
        printf("%d ", pRoot->num);
    }
}

/**********************************************************************************************************/

// Non-recursive implementation of inorder traversal using a stack.

void iterativeInorderTraversal(FILE *pOutputFile, TREENODEPTR pRoot)
{
    if (pRoot == NULL)
    {
        fprintf(pOutputFile, "The tree is empty.\n\n");
        printf("The tree is empty.\n\n");
        return;
    }
    
    // Initialize the stack and create a current pointer that points to a tree node
    STACK stack;
    stack.top = EMPTYSTACK;
    TREENODEPTR pCurrent = pRoot;
    
    // Traverse the tree
    while (1)
    {
    
        // Traverse to the left most node and push each node into the stack in the process
        while (pCurrent != NULL)
        {
            push(&stack, pCurrent);
            pCurrent = pCurrent->pLeft;
        }
        
        // When pCurrent is at the left most node, print it and move to the right child (if possible),
        // then move down to the left most node from that initial right child (if possible)
        if (pCurrent == NULL && !isEmpty(&stack))
        {
            pCurrent = pop(&stack);
            fprintf(pOutputFile, "%d ", pCurrent->num);
            printf("%d ", pCurrent->num);
            pCurrent = pCurrent->pRight;
            
            while (pCurrent != NULL)
            {
                push(&stack, pCurrent);
                pCurrent = pCurrent->pLeft;
            }
        }
        
        // If the current pointer is NULL, and the stack is empty, then we are done
        if (pCurrent == NULL && isEmpty(&stack))
            return;
    }
}

/**********************************************************************************************************/

// Non-recursive implementation of preorder traversal using a stack.

void iterativePreorderTraversal(FILE *pOutputFile, TREENODEPTR pRoot)
{
    if (pRoot == NULL)
    {
        fprintf(pOutputFile, "The tree is empty.\n\n");
        printf("The tree is empty.\n\n");
        return;
    }
    
    // Initialize the stack and create a current pointer that points to a tree node
    STACK stack;
    stack.top = EMPTYSTACK;
    TREENODEPTR pCurrent = pRoot;
    
    // Push the root into the stack
    push(&stack, pCurrent);
    
    // Pop the top node out of the stack and push the node's children into the stack
    while (!isEmpty(&stack))
    {
        pCurrent = pop(&stack);
        fprintf(pOutputFile, "%d ", pCurrent->num);
        printf("%d ", pCurrent->num);
        
        if (pCurrent->pRight != NULL)
            push(&stack, pCurrent->pRight);
        
        if (pCurrent->pLeft != NULL)
            push(&stack, pCurrent->pLeft);
    }
}

/**********************************************************************************************************/

// Non-recursive implementation of postorder traversal using a stack.

void iterativePostorderTraversal(FILE *pOutputFile, TREENODEPTR pRoot)
{
    if (pRoot == NULL)
    {
        fprintf(pOutputFile, "The tree is empty.\n\n");
        printf("The tree is empty.\n\n");
        return;
    }
    
    STACK stack1, stack2;
    stack1.top = EMPTYSTACK;
    stack2.top = EMPTYSTACK;
    TREENODEPTR pCurrent = pRoot;
    
    // Push the root node into stack1
    push(&stack1, pCurrent);
    
    while (!isEmpty(&stack1))
    {
        // Pop from stack1 and push into stack2
        pCurrent = pop(&stack1);
        push(&stack2, pCurrent);
        
        // Push the left and right children on popped node (pCurrent) into stack1
        if (pCurrent->pLeft != NULL)
            push(&stack1, pCurrent->pLeft);
        if (pCurrent->pRight != NULL)
            push(&stack1, pCurrent->pRight);
    }
    
    // Popping each node from stack2 and printing it simulates postorder traversal
    while (!isEmpty(&stack2))
    {
        pCurrent = pop(&stack2);
        fprintf(pOutputFile, "%d ", pCurrent->num);
        printf("%d ", pCurrent->num);
    }
}

/**********************************************************************************************************/

// Search the tree for the node containing the desired value.
// This function returns the node containing the desired value.

TREENODEPTR search(TREENODEPTR pRoot, int value)
{
    TREENODEPTR pCurrent = pRoot;
    
    if (pRoot == NULL)
    {
        printf("The tree is empty.\n\n");
        return NULL;
    }
    
    while (pCurrent != NULL && pCurrent->num != value)
    {
        if (value < pCurrent->num)
            pCurrent = pCurrent->pLeft;
        else
            pCurrent = pCurrent->pRight;
    }
    
    return pCurrent;
}

/**********************************************************************************************************/

// Get the minimum value in the tree based on where the root is.

TREENODEPTR getMin(FILE *pOutputFile, TREENODEPTR pRoot)
{
    if (pRoot == NULL)
    {
        fprintf(pOutputFile, "The tree is empty.\n\n");
        printf("The tree is empty.\n\n");
        return pRoot;
    }
    
    TREENODEPTR pCurrent = pRoot;
    
    while (pCurrent->pLeft != NULL)
        pCurrent = pCurrent->pLeft;
    
    return pCurrent;
}

/**********************************************************************************************************/

// Recursively delete a node from the tree.
// There are three cases: A node with no children, a node with one child, and a node with two children.

TREENODEPTR deleteNode(FILE *pOutputFile, TREENODEPTR pRoot, int value)
{
    TREENODEPTR pCurrent = pRoot;
    TREENODEPTR pTemp;

    
    // Terminating condition
    if (pCurrent == NULL)
    {
        return pCurrent;
    }
    else if (value < pCurrent->num)
        pCurrent->pLeft = deleteNode(pOutputFile, pCurrent->pLeft, value);
    else if (value > pCurrent->num)
        pCurrent->pRight = deleteNode(pOutputFile, pCurrent->pRight, value);
    else
    {
        // Case 1: The node to be deleted has no children
        if(pCurrent->pLeft == NULL && pCurrent->pRight == NULL) {
            free(pCurrent);
            pCurrent = NULL;
        }
        // Case 2: The node to be deleted has only one child
        else if (pCurrent->pLeft == NULL) {
            pTemp = pCurrent;
            pCurrent = pCurrent->pRight;
            free(pTemp);
        }
        else if(pCurrent->pRight == NULL) {
            pTemp = pCurrent;
            pCurrent = pCurrent->pLeft;
            free(pTemp);
        }
        // Case 3: The node to be deleted has 2 children
        else
        {
            pTemp = getMin(pOutputFile, pCurrent->pRight);
            pCurrent->num = pTemp->num;
            pCurrent->pRight = deleteNode(pOutputFile, pCurrent->pRight,pTemp->num);
        }
    }
    return pCurrent;
}

/**********************************************************************************************************/

// Prompts the user to enter an item to be deleted from the tree. An error message is
// displayed if the value is not in the tree. The user can choose to stop deleting items
// from the list by entering a blank line.

void promptToDelete(FILE *pOutputFile, TREENODEPTR ppRoot)
{
    char string[20];
    int num;
    
    printf("Enter a number to be deleted from the tree (blank line to stop): ");
    
    while((fgets(string, sizeof(string), stdin) != NULL) && (string[0] != '\n'))
    {
        sscanf(string, "%d", &num);
        
        if (search(ppRoot, num))
        {
            ppRoot = deleteNode(pOutputFile, ppRoot, num);
            printf("\n%d has been removed from the tree. Here is the new tree using inorder traversal.\n\n", num);
            fprintf(pOutputFile, "The user chose to remove %d from the tree.\n\n", num);
            fprintf(pOutputFile, "Here is the new tree using inorder traversal:\n\n");
            iterativeInorderTraversal(pOutputFile, ppRoot);
            fprintf(pOutputFile, "\n\n");
            printf("\n\n");
        }
        else
        {
            printf("\n%d cannot be deleted because it's not in the tree.\n\n", num);
            fprintf(pOutputFile, "The user tried to remove %d from the tree, but it is not in the tree.\n\n", num);
        }
        
        printf("Enter a number to be deleted from the tree (blank line to stop): ");
    }
    
    fprintf(pOutputFile, "The user has finished deleting nodes from the tree.\n\n");
    printf("\n");
}

/**********************************************************************************************************/

// Display the tree to the console and write it to the output file by using an iterative traversal method.
// The tree is displayed three times. Once each for preorder, postorder, and inorder traversal methods.

void displayTraversals(FILE *pOutputFile, TREENODEPTR pRoot)
{
    fprintf(pOutputFile, "Printing the tree using an iterative preorder method:\n\n");
    printf("Printing the tree using an iterative preorder method:\n\n");
    iterativePreorderTraversal(pOutputFile, pRoot);
    fprintf(pOutputFile, "\n\n");
    printf("\n\n");
    
    fprintf(pOutputFile, "Printing the tree using an iterative postorder method:\n\n");
    printf("Printing the tree using an iterative postorder method:\n\n");
    iterativePostorderTraversal(pOutputFile, pRoot);
    fprintf(pOutputFile, "\n\n");
    printf("\n\n");
    
    fprintf(pOutputFile, "Printing the tree using an iterative inorder method:\n\n");
    printf("Printing the tree using an iterative inorder method:\n\n");
    iterativeInorderTraversal(pOutputFile, pRoot);
    fprintf(pOutputFile, "\n\n");
    printf("\n\n");
}

/**********************************************************************************************************/


































