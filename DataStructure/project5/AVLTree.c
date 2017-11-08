#include "AVLTree.h"

AVLTree newAVLTree()
{
	AVLTree tree = (AVLTree)malloc(sizeof(AVLTreeImp));
	tree->pRoot = NULL;
	return tree;
}

void freeAVLTree(AVLTree tree)
{
	postOrderAVLNodeFree(tree->pRoot);
	free(tree);	
}

void postOrderAVLNodeFree(NodeAVL *p)
{
	if(p != NULL)
	{
		postOrderAVLNodeFree(p->pLeft);
		postOrderAVLNodeFree(p->pRight);
		free(p);
	}
}


NodeAVL *allocateAVLNode(ElementAVL value)
{
	NodeAVL *pNew = (NodeAVL *)malloc(sizeof(NodeAVL));
	pNew->pLeft = NULL;
	pNew->pRight = NULL;
	pNew->pParent = NULL;
	pNew->element = value;
	pNew->iBalanceFactor = 0;
	
	return pNew;	
}



NodeAVL *searchAVL(AVLTree tree, int searchValue)
{
	return recursiveSearchAVL(tree->pRoot,searchValue);
}

//Recursive algorithm for searching for a node with key value equal to searchValue.  Return a pointer to the node if you find it or return NULL if it does not exist.
NodeAVL *recursiveSearchAVL(NodeAVL *p, int searchValue)
{
	
	if(p==NULL)
		return NULL;
	
	if(p->element.key == searchValue)
		return p;
	else if(p->element.key > searchValue)
		return recursiveSearchAVL(p->pLeft,searchValue);
	else
		return recursiveSearchAVL(p->pRight,searchValue);
	
}


int insertAVL(AVLTree tree, ElementAVL value)
{
	
	if(tree->pRoot == NULL)
	{
		//The recursiveInsert() function assumes that the pointer we pass is not NULL, so if pRoot is NULL then we handle this case here.
		tree->pRoot = allocateAVLNode(value);
		return TRUE;
	}
	else
	{
		int result = recursiveInsertAVL(tree->pRoot,value);
		
		//If we rotated the root, the new root would be the parent of the old root.
		if(tree->pRoot->pParent != NULL){
			
			tree->pRoot = tree->pRoot->pParent;
		}				
		
		return result;
	}
	
}


//Recursive algorithm to insert value into an AVL tree.
//Our base case is slightly different than the base case shown in class.  Instead of checking if the current node is NULL, we check to see if the next node would be NULL if we continued.  This allows us to update the parent pointer. 
//Given the implementation, p will never be NULL. 
int recursiveInsertAVL(NodeAVL *p, ElementAVL value)
{	
	if( p->element.key == value.key )
	{
		return FALSE;
	}
	else if( p->element.key > value.key )
	{
		if(p->pLeft != NULL)
			return recursiveInsertAVL(p->pLeft, value);
		else
		{
			//value.key is less than current node, but current node does not have a left child.
			//The new node should be the left child of the current node.
			//Ater insertion, check to see if we need to rebalance the tree.
			p->pLeft = allocateAVLNode(value);
			p->pLeft->pParent = p;
			checkForRebalancing(p->pLeft);
			return TRUE;
		}
	}
	else
	{
		if(p->pRight != NULL)
			return recursiveInsertAVL(p->pRight, value);
		else
		{
			//value.key is greater than current node, but current node does not have a right child.
			//The new node should be the right child of the current node.
			//Ater insertion, check to see if we need to rebalance the tree.
			p->pRight = allocateAVLNode(value);
			p->pRight->pParent = p;
			checkForRebalancing(p->pRight);
			return TRUE;
		}
		
	}
	
}

//We are passed p which is a pointer to the newly inserted node into the tree.
//We walk from p towards the root of the tree until we find a node that needs to be rotated or until we determine that the tree is balanced.
//We perform any rotations and update any balance factors that need to be updated.
void checkForRebalancing(NodeAVL *p)
{
	
	NodeAVL *z = p;
	NodeAVL *x;
	
	for(x=z->pParent; x != NULL; x = x->pParent)
	{
		//x is the parent of z.
		if(z == x->pLeft)
		{
			//z is the left child of x.  Check to see if x was imbalanced to the left prior to insertion.
			if(x->iBalanceFactor < 0)
			{
				//Both are true: (1) z is left child of x, and (2) x was imbalanced to the left.
				//We need to fix with rotations.
				if(z->iBalanceFactor <= 0)
				{
					//z is NOT imbalanced to the right, so a right rotate on x will fix the tree.
					rightRotate(x);
					
					//Update the balance factors of x and z.
					x->iBalanceFactor = 0;
					z->iBalanceFactor = 0;
					
				}
				else
				{
					//z is imbalanced to the right, so a right rotation on x will cause x to be imbalanced to the left.
					//To fix, we left rotate z and then we right rotate x to fix the tree.
					//We create w to store the right child of z (before insertion), as it's balance factor will affect the ending balance factors.
					//Note w cannot be NULL since z is imbalanced to the right.
					
					NodeAVL *w = z->pRight;
					leftRotate(z);
					rightRotate(x);
					
					//Update the balance factors.
					if(w->iBalanceFactor > 0)
					{
						w->iBalanceFactor = 0;
						x->iBalanceFactor = 0;
						z->iBalanceFactor = -1;
					}
					else if(w->iBalanceFactor == 0)
					{
						w->iBalanceFactor = 0;
						x->iBalanceFactor = 0;
						z->iBalanceFactor = 0;
					}
					else
					{
						w->iBalanceFactor = 0;
						x->iBalanceFactor = 1;
						z->iBalanceFactor = 0;
						
					}
					
				}
				
				return;
				
			}
			else
			{
				//x was not imbalanced to the left, so we only need to update x's balance factor.
				
				if(x->iBalanceFactor == 1)
				{
					x->iBalanceFactor = 0;
					return;
				}
				
				//If here then x's balance factor before was 0.
				//Update balance factor, and set z to be it's parent for the next iteration of the for loop.
				x->iBalanceFactor = -1;
				z = x;				
			}			
			
		}
		else
		{
			//z is the right child of x.  Check to see if x was imbalanced to the right prior to insertion.
			if(x->iBalanceFactor > 0)
			{
				//Both are true: (1) z is right child of x, and (2) x was imbalanced to the right.
				//We need to fix with rotations.
				if(z->iBalanceFactor >= 0)
				{
					//z is NOT imbalanced to the left, so a left rotate on x will fix the tree.
					leftRotate(x);
					
					//Update the balance factors of x and z.
					x->iBalanceFactor = 0;
					z->iBalanceFactor = 0;
					
				}
				else
				{
					//z is imbalanced to the left, so a left rotation on x will cause x to be imbalanced to the right.
					//To fix, we right rotate z and then we left rotate x to fix the tree.
					//We create w to store the left child of z (before insertion), as it's balance factor will affect the ending balance factors.
					//Note w cannot be NULL since z is imbalanced to the left.
					
					NodeAVL *w = z->pLeft;
					rightRotate(z);
					leftRotate(x);
					
					//Update the balance factors.
					if(w->iBalanceFactor > 0)
					{
						w->iBalanceFactor = 0;
						x->iBalanceFactor = -1;
						z->iBalanceFactor = 0;
					}
					else if(w->iBalanceFactor == 0)
					{
						w->iBalanceFactor = 0;
						x->iBalanceFactor = 0;
						z->iBalanceFactor = 0;
					}
					else
					{
						w->iBalanceFactor = 0;
						x->iBalanceFactor = 0;
						z->iBalanceFactor = 1;
						
					}
					
				}
				
				return;
				
			}
			else
			{
				//x was not imbalanced to the right, so we only need to update x's balance factor.
				
				if(x->iBalanceFactor == -1)
				{
					x->iBalanceFactor = 0;
					return;
				}
				
				//If here then x's balance factor before was 0.
				//Update balance factor, and set z to be it's parent for the next iteration of the for loop.
				x->iBalanceFactor = 1;
				z = x;				
			}		
			
			
		}		
		
	}
	
	
}



void printInOrderAVL(AVLTree tree)
{
	recursivePrintInOrderAVL(tree->pRoot);
}

void recursivePrintInOrderAVL(NodeAVL *p)
{
	if(p != NULL)
	{
		recursivePrintInOrderAVL(p->pLeft);
		printf("%ld\n",p->element.key);
		recursivePrintInOrderAVL(p->pRight);
	}	
}


void printPreOrderAVL(AVLTree tree)
{
	recursivePrintPreOrderAVL(tree->pRoot);
}

void recursivePrintPreOrderAVL(NodeAVL *p)
{
	if(p != NULL)
	{
		
		printf("%ld\n",p->element.key);
		recursivePrintPreOrderAVL(p->pLeft);
		recursivePrintPreOrderAVL(p->pRight);
	}	
}


