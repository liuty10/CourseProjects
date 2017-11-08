/*
 * =====================================================================================
 *
 *       Filename:  BrowserList.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/08/2017 10:10:36 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "BrowserList.h"

//-------------------------------------------------------------------------
//FunctionName: newBrowserList
//
//Description:
//  Allocates a new BrowserListImp and returns its address.
//
//ReturnValue:
//  address of BrowserList
//------------------------------------------------------------------------
BrowserList newBrowserList(){
  BrowserList browserList = (BrowserList)malloc(sizeof(BrowserListImp));
  browserList->pCurrentURL = NULL;
  browserList->list = (DoublyLinkedList) newDoublyLinkedList();
  browserList->list->pHead = NULL;
  browserList->list->pFoot = NULL;
  return browserList;
}


//-------------------------------------------------------------------------
//FunctionName: freeBrowserList
//
//Description:
//  Free a BrowserList
//
//ReturnValue:
//  N/A  
//------------------------------------------------------------------------
void freeBrowserList(BrowserList browserList){
  freeDoublyLinkedList(browserList->list);
}


//-------------------------------------------------------------------------
//FunctionName:goToURL 
//
//Description:
//  1. add a new node after pCurrentURL node
//  2. delete the nodes after pCurrentURL node
//  3. change the new node to be pCurrentURL node
//  4. the pFoot should also point to the new node
//Input:
//  browserList
//  element: initialize the URL of the new node
//
//ReturnValue:
//  N/A  
//------------------------------------------------------------------------
void goToURL(BrowserList browserList, Element element){
  NodeDL *newNode = allocateNode(element);
  if(browserList->list->pFoot != NULL)
  {
    while(browserList->list->pFoot != browserList->pCurrentURL){
    browserList->list->pFoot = browserList->list->pFoot->pPrev;
    free(browserList->list->pFoot->pNext);
    }
    browserList->pCurrentURL->pNext = newNode;
    newNode->pPrev = browserList->pCurrentURL;
    browserList->pCurrentURL = newNode;
    browserList->list->pFoot = newNode;
  }else{
    browserList->pCurrentURL = newNode;
    browserList->list->pHead = newNode;
    browserList->list->pFoot = newNode;
  }
}


//------------------------------------------------------------------------
//FunctionName: back
//
//Description:
//  1. Go back to the webpage prior to the current webpage in the BrowserList.  
//  2. Return TRUE if successful
//     Return FALSE if not (e.g., if no such webpage exists).
//
//Input:
//
//ReturnValue:
//  TRUE: if successful
//  FALSE: if fail
//------------------------------------------------------------------------
int back(BrowserList browserList){
  if(browserList->pCurrentURL != NULL &&browserList->pCurrentURL->pPrev != NULL)
    browserList->pCurrentURL = browserList->pCurrentURL->pPrev;
  else
    return FALSE;
  
  return TRUE;
}//end back()


//Go forward to the webpage after the current webpage in the BrowserList.  Return TRUE if successful
//and return FALSE if not (e.g., no such webpage exists).
int forward(BrowserList browserList){
  if(browserList->pCurrentURL->pNext != NULL)
    browserList->pCurrentURL = browserList->pCurrentURL->pNext;
  else
    return FALSE;
  
  return TRUE;
  
}


//Print the contents of the BrowserList, one webpage per line.  Place *'s around the URL of the current webpage.
void printBrowserList(BrowserList browserList){
 NodeDL *tempNode = browserList->list->pHead;
 while(tempNode != NULL){
    if(tempNode == browserList->pCurrentURL)
      printf("*%s\n",tempNode->element.szURL);
    else
      printf(" %s\n",tempNode->element.szURL);
    
    tempNode = tempNode->pNext;
 }
}

