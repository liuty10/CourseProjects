#include "BinarySearchTree.h"

BinarySearchTree newBinarySearchTree()
{
	BinarySearchTree tree = (BinarySearchTree)malloc(sizeof(BinarySearchTreeImp));
	tree->pRoot = NULL;
	return tree;
}

void freeBinarySearchTree(BinarySearchTree tree)
{
	postOrderNodeFree(tree->pRoot);
	free(tree);	
}

void postOrderNodeFree(NodeT *p)
{
	if(p != NULL)
	{
		postOrderNodeFree(p->pLeft);
		postOrderNodeFree(p->pRight);
		free(p);
	}
}


NodeT *allocateNode(Element value)
{
	NodeT *pNew = (NodeT *)malloc(sizeof(NodeT));
	pNew->pLeft = NULL;
	pNew->pRight = NULL;
	pNew->element = value;
	
	return pNew;	
}



NodeT *search(BinarySearchTree tree, long searchValue)
{
	return recursiveSearch(tree->pRoot,searchValue);
}

//Recursive algorithm for searching for a node with key value equal to searchValue.  Return a pointer to the node if you find it or return NULL if it does not exist.
NodeT *recursiveSearch(NodeT *p, long searchValue)
{
	
	if(p==NULL)
		return NULL;
	
	if(p->element.key == searchValue)
		return p;
	else if(p->element.key > searchValue)
		return recursiveSearch(p->pLeft,searchValue);
	else
		return recursiveSearch(p->pRight,searchValue);
	
}


int insert(BinarySearchTree tree, Element value)
{	
	return recursiveInsert(&(tree->pRoot),value);
	
}



int recursiveInsert(NodeT **p, Element value)
{
	if(*p == NULL)
	{
		*p = allocateNode(value);
		return TRUE;		
	}
	
	if( (*p)->element.key == value.key )
		return FALSE;
	else if( (*p)->element.key > value.key )
		return recursiveInsert( &((*p)->pLeft), value);
	else
		return recursiveInsert( &((*p)->pRight), value);
	
}


void printInOrder(BinarySearchTree tree)
{
	recursivePrintInOrder(tree->pRoot);
}

void recursivePrintInOrder(NodeT *p)
{
	if(p != NULL)
	{
		recursivePrintInOrder(p->pLeft);
		printf("%ld\n",p->element.key);
		recursivePrintInOrder(p->pRight);
	}	
}


void printPreOrder(BinarySearchTree tree)
{
	recursivePrintPreOrder(tree->pRoot);
}

void recursivePrintPreOrder(NodeT *p)
{
	if(p != NULL)
	{
		
		printf("%ld\n",p->element.key);
		recursivePrintPreOrder(p->pLeft);
		recursivePrintPreOrder(p->pRight);
	}	
}


