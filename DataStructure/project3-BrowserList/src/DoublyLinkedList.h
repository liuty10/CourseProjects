/************************************************************************
DoublyLinkedList.h

Purpose:
    Define constants used in the project
    Struct definitions for a general Doubly Linked List.
    Define function prototypes used by general Doubly Linked Lists.
************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>


//#define constant values
#define MAX_URL_LENGTH 50

#define TRUE 1
#define FALSE 0


//typedef for the Element struct which constains a c string to store a URL in the BrowserList
typedef struct
{
    char szURL[MAX_URL_LENGTH];
} Element;


//Typedef for a node in the doubly linked list (has next and previous pointers).
typedef struct NodeDL
{
    Element element;
    struct NodeDL *pNext;
    struct NodeDL *pPrev;
} NodeDL;

//Typedef for a doubly linked list implementation.  
//Contains a pointer to the first node in the list and the last node in the list (pHead and pFoot respectively).
typedef struct
{
    NodeDL *pHead;
    NodeDL *pFoot;
} DoublyLinkedListImp;

typedef DoublyLinkedListImp *DoublyLinkedList;


/*****Prototypes*******/

//Malloc a new DoublyLinkedListImp and return it's address.
DoublyLinkedList newDoublyLinkedList();


//Free the DoublyLinkedList and any nodes that still exist in the list
void freeDoublyLinkedList(DoublyLinkedList list);


//Allocate a new node and store "value" as the Element in the node.  Return the address of the node.
NodeDL *allocateNode(Element value);


//Create a node to store the given Element and add it to the end of the DoublyLinkedList.
void append(DoublyLinkedList list, Element value);
