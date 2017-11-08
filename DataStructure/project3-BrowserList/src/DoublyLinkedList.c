/*
 * =====================================================================================
 *
 *       Filename:  DoublyLinkedList.c
 *
 *    Description:  
 *        Implement the functions declared in DoubleLinkedList.h
 *
 *        Version:  1.0
 *        Created:  11/08/2017 08:34:08 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

# include "DoublyLinkedList.h"

//-----------------------------------------------------------------------------------
//FunctionName: newDoublyLinkedList
//
//Descritption:
//    Funtion: Malloc a new DoublyLinkedListImp and return it's address.
//    RetrunValue: An address.
//-----------------------------------------------------------------------------------
DoublyLinkedList newDoublyLinkedList(){
  return (DoublyLinkedList)malloc(sizeof(DoublyLinkedListImp));
}

//-----------------------------------------------------------------------------------
//FunctionName: freeDoublyLinkedList
//
//Descritption:
//    Free the DoublyLinkedList and any nodes that still exist in the list
//    1. check the head, and free all the nodes in the list
//    2. free the list metadata.
//Input:
//    address of DoublyLinkedList
//
//RetrunValue: 
//    N/A.
//-----------------------------------------------------------------------------------
void freeDoublyLinkedList(DoublyLinkedList list){
  while(list->pHead != NULL){
    list->pHead = list->pHead->pNext;
    if(list->pHead->pPrev != NULL)
      free(list->pHead->pPrev);
  } 
  free(list);
}

//---------------------------------------------------------------------------------
//FunctionName: allocateNode
//
//Description:
//  Allocate a new node and store "value" as the Element in the node.  
//
//Input:
//  URL of this new node.
//
//ReturnValue:
//  Return the address of the node.
//---------------------------------------------------------------------------------  
NodeDL *allocateNode(Element value){
  NodeDL *newNode = (NodeDL*)malloc(sizeof(NodeDL));
  int i=0;
  while(i<MAX_URL_LENGTH){
   
    newNode->element.szURL[i]= value.szURL[i];
    i++;

  }
  return newNode;
}

//---------------------------------------------------------------------------------
//FunctionName: append
//
//Description:
//  1. Create a node to store the given Element
//  2. Add it to the end of the DoublyLinkedList
//
//Input:
//  1. list metadata
//  2. the URL value
//
//ReturnValue:
//  N/A
//---------------------------------------------------------------------------------
void append(DoublyLinkedList list, Element value){
  NodeDL *newNode     = allocateNode(value);
  list->pFoot->pNext  = newNode;
  newNode->pPrev      = list->pFoot;
  newNode->pNext       = NULL;
  list->pFoot         = newNode;
}




