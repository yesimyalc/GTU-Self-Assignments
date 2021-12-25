
public class AVLTree<E> extends RotatingBinarySearchTree<E>
{
	/**
	 * Indicates if the height of a node is incresed.
	 */
	private boolean increased;
	/**
	 * Indicates if the height of a node is decreased.
	 */
	private boolean decreased;
	
	/**
	 * A container class that holds the data inside the AVLTree
	 * The class holds the balance of the element too. 
	 * - balance meaning left heavy, + balance meaning right heavy
	 */
	protected static class AVLNode<E> extends Node<E>
	{
		private static final int Left_Heavy=-1;
		private static final int Right_Heavy=1;
		private static final int Balanced=0;
		
		int balance;
		
		public AVLNode(E data)
		{
			super(data);
			balance=Balanced;
		}
		
		public String toString()
		{
			return data.toString()+" "+Integer.toString(balance);
		}
	}
	
	/**
	 * Changes the balance of the localRoot by the given change amount
	 */
	private void changeBalance(AVLNode<E> localRoot, int change)
	{
		localRoot.balance+=change;
		if(localRoot.balance==AVLNode.Balanced)
		{
			increased=false;
			decreased=true;
		}
		else
			decreased=false;
	}
	
	/**
	 * Rebalances to the left starting from the localRoot
	 * @return the new localRoot
	 */
	private AVLNode<E> rebalanceLeft(AVLNode<E> localRoot, int oldBalance)
	{
		AVLNode<E> leftC=(AVLNode<E>)localRoot.left;
		Integer oldLeftRightB=null;
		AVLNode<E> leftRightC=null;
		if(leftC.balance>AVLNode.Balanced)
		{
			leftRightC=(AVLNode<E>) leftC.right;
			if(leftRightC.balance==AVLNode.Balanced)
			{
				localRoot.balance=AVLNode.Balanced;
				leftC.balance=AVLNode.Balanced;
			}
			else if(leftRightC.balance<AVLNode.Balanced)
			{
				localRoot.balance=AVLNode.Right_Heavy;
				leftC.balance=AVLNode.Balanced;
				oldLeftRightB=leftRightC.balance;
				leftRightC.balance=AVLNode.Balanced;
			}
			else if(leftRightC.balance>AVLNode.Balanced)
			{
				localRoot.balance=AVLNode.Balanced;
				leftC.balance=AVLNode.Left_Heavy;
				oldLeftRightB=leftRightC.balance;
				leftRightC.balance=AVLNode.Balanced;
			}
			localRoot.left=(AVLTree.AVLNode<E>) rotateLeft(leftC);
		}
		else if(leftC.balance==AVLNode.Balanced)
		{
			localRoot.balance=AVLNode.Left_Heavy;
			leftC.balance=AVLNode.Right_Heavy;
		}
		else
		{
			localRoot.balance=AVLNode.Balanced;
			leftC.balance=AVLNode.Balanced;
		}
		
		if(oldLeftRightB!=null)
		{
			if(leftRightC.balance==oldBalance)
				decreased=false;
			else if(oldBalance==1 && leftRightC.balance==-1)
				decreased=false;
			else if(oldBalance==-1 && leftRightC.balance==1)
				decreased=false;
			else
				decreased=true;
		}
		else
		{
			if(leftC.balance==oldBalance)
				decreased=false;
			else if(oldBalance==1 && leftC.balance==-1)
				decreased=false;
			else if(oldBalance==-1 && leftC.balance==1)
				decreased=false;
			else
				decreased=true;
		}
		return (AVLNode<E>)rotateRight(localRoot);
	}
	
	/**
	 * Rebalances to the right starting from the localRoot
	 * @return the new localRoot
	 */
	private AVLNode<E> rebalanceRight(AVLNode<E> localRoot, int oldBalance)
	{
		AVLNode<E> rightC=(AVLNode<E>)localRoot.right;
		Integer oldRightLeftB=null;
		AVLNode<E> rightLeftC=null;
		if(rightC.balance<AVLNode.Balanced)
		{
			rightLeftC=(AVLNode<E>) rightC.left;
			if(rightLeftC.balance==AVLNode.Balanced)
			{
				localRoot.balance=AVLNode.Balanced;
				rightC.balance=AVLNode.Balanced;
			}
			else if(rightLeftC.balance>AVLNode.Balanced)
			{
				localRoot.balance=AVLNode.Left_Heavy;
				rightC.balance=AVLNode.Balanced;
				oldRightLeftB=rightLeftC.balance;
				rightLeftC.balance=AVLNode.Balanced;
			}
			else if(rightLeftC.balance<AVLNode.Balanced)
			{
				localRoot.balance=AVLNode.Balanced;
				rightC.balance=AVLNode.Right_Heavy;
				oldRightLeftB=rightLeftC.balance;
				rightLeftC.balance=AVLNode.Balanced;
			}
			localRoot.right=(AVLTree.AVLNode<E>) rotateRight(rightC);
		}
		else if(rightC.balance==AVLNode.Balanced)
		{
			localRoot.balance=AVLNode.Right_Heavy;
			rightC.balance=AVLNode.Left_Heavy;
		}
		else
		{
			localRoot.balance=AVLNode.Balanced;
			rightC.balance=AVLNode.Balanced;
		}
		
		
		if(oldRightLeftB!=null)
		{
			if(rightLeftC.balance==oldBalance)
				decreased=false;
			else if(oldBalance==1 && rightLeftC.balance==-1)
				decreased=false;
			else if(oldBalance==-1 && rightLeftC.balance==1)
				decreased=false;
			else
				decreased=true;
		}
		else
		{
			if(rightC.balance==oldBalance)
				decreased=false;
			else if(oldBalance==1 && rightC.balance==-1)
				decreased=false;
			else if(oldBalance==-1 && rightC.balance==1)
				decreased=false;
			else
				decreased=true;
		}
		return (AVLNode<E>)rotateLeft(localRoot);
	}
	
	/** Recursive add method.
	 post: The data field addReturn is set true if the item is added to
	 the tree, false if the item is already in the tree.
	 Also does the necessary rebalancing
	 @param localRoot The local root of the subtree
	 @param item The object to be inserted
	 @return The new local root that now contains the
	 inserted item
	*/
	private AVLNode<E> add(AVLNode<E> localRoot, E item)
	{
		if (localRoot == null) 
		{
			isAdded=true;
			increased = true;
			size++;
			return new AVLNode<E>(item);
		}
		if (((Comparable)item).compareTo(localRoot.data) == 0) 
		{
			// Item is already in the tree.
			increased = false;
			isAdded = false;
			return localRoot;
		}
		else if (((Comparable)item).compareTo(localRoot.data) < 0) 
		{
			// item < data
			 localRoot.left = add((AVLNode<E>) localRoot.left, item);
			 if (increased) 
			 {
				 changeBalance(localRoot, -1);
				 if (localRoot.balance < AVLNode.Left_Heavy) 
				 {
					  increased = false;
					  return rebalanceLeft(localRoot, 0);
				 }
			  }
			 return localRoot;
		}
		else
		{
		// item > data
		 localRoot.right = add((AVLNode<E>) localRoot.right, item);
		 if (increased) 
		 {
			   changeBalance(localRoot, 1);
			   if (localRoot.balance > AVLNode.Right_Heavy) 
			   {
				    increased = false;
				    return rebalanceRight(localRoot, 0);
			   }
		  }
		 return localRoot;
		}
	}
	
	/** Find the node that is the
	 inorder predecessor and replace it
	 with its left child (if any).
	 Also does the necessary rebalancing
	 post: The inorder predecessor is removed from the tree.
	 @param parent The parent of possible inorder
	 predecessor (ip)
	 @return The data in the ip
	*/
	private E findLargestChild(AVLNode<E> parent)
	{
		// If the right child has no right child, it is
		// the inorder predecessor.
		if (parent.right.right == null) 
		{
			changeBalance(parent, -1);
			E returnValue = parent.right.data;
			parent.right = parent.right.left;
			return returnValue;
		} 
		else 
		{
			E temp=findLargestChild((AVLNode<E>)parent.right);
			if(((AVLNode<E>)parent.right).balance<AVLNode.Left_Heavy)
				parent.right=rebalanceLeft(((AVLNode<E>)parent.right), -1);
			if(decreased)
			{
				int temp2=parent.balance;
				changeBalance(parent, -1);
			}

			return temp;
		}
	}
	
	/** Recursive delete method.
	 post: The item is not in the tree;
	 deleteReturn is equal to the deleted item
	 as it was stored in the tree or null
	 if the item was not found.
	 Also does the necessary rebalancing.
	 @param localRoot The root of the current subtree
	 @param item The item to be deleted
	 @return The modified local root that does not contain
	 the item
	*/
	private AVLNode<E> delete(AVLNode<E> localRoot, E item)
	{
		if (localRoot == null) 
		 {
			 // item is not in the tree.
			 removed = null;
			 decreased=false;
			 return localRoot;
		 }
		 // Search for item to delete.
		 int compResult = ((Comparable<E>)item).compareTo(localRoot.data);		 
		 if (compResult < 0) 
		 {
			 // item is smaller than localRoot.data.
			 localRoot.left = delete((AVLNode<E>)localRoot.left, item);
			 if(localRoot.left!=null && ((AVLNode<E>)localRoot.left).balance>AVLNode.Right_Heavy)
				 localRoot.left=rebalanceRight(((AVLNode<E>)localRoot.left), 1);
			 if(decreased)
				{
				 	int temp=localRoot.balance;
					changeBalance(localRoot, 1);
					if(localRoot.balance>AVLNode.Right_Heavy)
					{
						return rebalanceRight(localRoot, temp);
					}
				}	
			 return localRoot;
		 } 
		 else if (compResult > 0) 
		 {
			 // item is larger than localRoot.data.
			 localRoot.right = delete((AVLNode<E>)localRoot.right, item);
			 if(localRoot.right!=null && ((AVLNode<E>)localRoot.right).balance>AVLNode.Right_Heavy)
				 localRoot.right=rebalanceRight(((AVLNode<E>)localRoot.right), 1);
			 if(decreased)
				{
				 	int temp=localRoot.balance;
					changeBalance(localRoot, -1);
					if(localRoot.balance<AVLNode.Left_Heavy)
					{
						return rebalanceLeft(localRoot, temp);
					}
				}	
			 return localRoot;
		 } 
		 else 
		 {
			 // item is at local root.
			 removed = localRoot.data;
			 size--;
			 if (localRoot.left == null) 
			 {
				 // If there is no left child, return right child
				 // which can also be null.
				 decreased=true;
				 return (AVLNode<E>) localRoot.right;
			 } 
			else if (localRoot.right == null) 
			{
				 // If there is no right child, return left child.
				 decreased=true;
				 return (AVLNode<E>) localRoot.left;
			} 
			else 
			{
				 // Node being deleted has 2 children, replace the data
				 // with inorder predecessor. 
				 if (localRoot.left.right == null) 
				 {
					 changeBalance(localRoot, 1);
					 // The left child has no right child.
					 // Replace the data with the data in the
					 // left child.
					 localRoot.data = localRoot.left.data;
					 // Replace the left child with its left child.
					 localRoot.left = localRoot.left.left;
					 return localRoot;
				 } 
				 else 
				 {
					 // Search for the inorder predecessor (ip) and
					 // replace deleted node's data with ip.
					 localRoot.data = findLargestChild((AVLNode<E>)localRoot.left);
					 if(((AVLNode<E>)localRoot.left).balance<AVLNode.Left_Heavy)
							localRoot.left=rebalanceLeft(((AVLNode<E>)localRoot.left), localRoot.balance+1);
					 if(decreased)
						{
						 	int temp=localRoot.balance;
							changeBalance(localRoot, 1);
							if(localRoot.balance>AVLNode.Right_Heavy)
							{
								return rebalanceRight(localRoot, temp);
							}
						}	
					 return localRoot;
				 }
			 }
		 }
	}
	
	/** Starter method add.
	 pre: The object to insert must implement the
	 Comparable interface.
	 @param item The object being inserted
	 @return true if the object is inserted, false
	 if the object already exists in the tree
	*/
	public boolean add(E item) 
	{
		isAdded=false;
		increased=false;
		root=add((AVLNode<E>)root, item);
				
		return isAdded;
	}
	
	/** Starter method delete.
	 post: The object is not in the tree.
	 @param target The object to be deleted
	 @return The object deleted from the tree
	 or null if the object was not in the tree
	 @throws ClassCastException if target does not implement
	 Comparable
	*/
	public E delete(E target) 
	{
		removed=null;
		decreased=false;
		root=delete((AVLNode<E>)root, target);
		
		return removed;
	}
	
	/**
	 * @throws NotRedBlackTreeException 
	 */
	protected boolean isRed() throws NotRedBlackTreeException
	{
		throw new NotRedBlackTreeException("The tree is not a RedBlackTree. It is an AVLTree.");
	}
}
