/***************************************************************
BrowserList.h
Purpose:
    Defines structs for the BrowserList.
    Defines prototypes for BrowserList functions.
***************************************************************/
#include "DoublyLinkedList.h"


//typedef for a BrowserListImp which stores a DoublyLinkedList of all of the URLs visited plus a pointer
//to the node containing the current webpage.
typedef struct
{
    DoublyLinkedList list;
    NodeDL *pCurrentURL;
} BrowserListImp;

typedef BrowserListImp *BrowserList;



//Allocates a new BrowserListImp and returns its address.
BrowserList newBrowserList();


//Free a BrowserList.
void freeBrowserList(BrowserList browserList);


//Go to the URL specified in the given Element and update the BrowserLIst accordingly.
void goToURL(BrowserList browserList, Element element);


//Go back to the webpage prior to the current webpage in the BrowserList.  Return TRUE if successful
//and return FALSE if not (e.g., if no such webpage exists).
int back(BrowserList browserList);


//Go forward to the webpage after the current webpage in the BrowserList.  Return TRUE if successful
//and return FALSE if not (e.g., no such webpage exists).
int forward(BrowserList browserList);


//Print the contents of the BrowserList, one webpage per line.  Place *'s around the URL of the current webpage.
void printBrowserList(BrowserList browserList);
