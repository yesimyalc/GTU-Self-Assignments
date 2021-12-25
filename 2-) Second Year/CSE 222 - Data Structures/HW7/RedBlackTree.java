public class RedBlackTree<E extends Comparable<E>> extends RotatingBinarySearchTree<E> 
{
	/** Nested class to represent a Redâ€�Black node. */
	private static class RedBlackNode<E> extends Node<E> 
	{
		// Additional data members
		/** Color indicator. True if red, false if black. */
		private boolean isRed;
		
		// Constructor
		/** Create a RedBlackNode with the default color of red
		and the given data field.
		@param item The data field
		*/
		public RedBlackNode(E item) 
		{
			super(item);
			isRed = true;
		}
		
		// Methods
		/** Return a string representation of this object.
		The color (red or black) is appended to the node's contents.
		@return String representation of this object
		*/
		@Override 
		public String toString() 
		{
			if (isRed) 
			return "Red : " + super.toString();
			else
			return "Black: " + super.toString();
		}
	}
	
	/**
     * Check to see whether both children of the given node are red.
     * If so, make them black and change the localRoot's color to red.
     * @param localRoot The node to check whether both children are red or not
     */
    private void moveBlackDown(RedBlackNode<E> localRoot)
    {
        if(localRoot.left != null && localRoot.right != null) //If a child is null, it is black
            if(((RedBlackNode<E>) localRoot.left).isRed && ((RedBlackNode<E>) localRoot.right).isRed)
            {   //if both children are red, swap colors
                ((RedBlackNode<E>) localRoot.left).isRed = false;
                ((RedBlackNode<E>) localRoot.right).isRed = false;
                localRoot.isRed = true;
            }
    }

	 /**
     * Recursive add method
     * @param localRoot The local root of the Red Black Tree
     * @param item The item to be inserted
     * @return The new local root
     */
	private Node<E> add(RedBlackNode<E> localRoot, E item)
    {
        int compare = item.compareTo(localRoot.data);

        if(compare == 0)
        {
            isAdded = false;
            return localRoot;
        }
        else if(compare < 0)
        {
            if(localRoot.left == null)
            {
                //Create new left child
                localRoot.left = new RedBlackNode<E>(item);
                isAdded = true;
                size++;
                return localRoot;
            }
            else
            { //Need to search
                //Check for two red children, swap colors if found
                moveBlackDown(localRoot);
                //Recursively add on the left
                localRoot.left = add((RedBlackNode<E>) localRoot.left, item);
                //See whether the left child is now red
                if(((RedBlackNode<E>) localRoot.left).isRed)
                {
                    if(localRoot.left.left != null && ((RedBlackNode<E>) localRoot.left.left).isRed)
                    {
                        // Left-left grand-child is also red
                        // Single rotation is necessary; change colors
                        ((RedBlackNode<E>) localRoot.left).isRed = false;
                        localRoot.isRed = true;
                        return rotateRight(localRoot);
                    }
                    else if(localRoot.left.right != null && ((RedBlackNode<E>) localRoot.left.right).isRed)
                    {
                        // Left-right grand-child is also red
                        // Double rotation is necessary; change colors
                        localRoot.left = rotateLeft(localRoot.left);
                        ((RedBlackNode<E>) localRoot.left).isRed = false;
                        localRoot.isRed = true;
                        return rotateRight(localRoot);
                    }
                }
                return localRoot;
            }
        }
        else
        {
            //item is greater than localRoot.data
            if(localRoot.right == null)
            {
                //Create new right child
                localRoot.right = new RedBlackNode<E>(item);
                isAdded = true;
                size++;
                return localRoot;
            }
            else
            { //Need to search
                //Check for two red children, swap if needed
                moveBlackDown(localRoot);
                //Recursively add on the right
                localRoot.right = add((RedBlackNode<E>) localRoot.right, item);
                //See whether right child is now red
                if(((RedBlackNode<E>) localRoot.right).isRed)
                {
                    if(localRoot.right.right != null && ((RedBlackNode<E>) localRoot.right.right).isRed)
                    {
                        //Right-right grand-child is also red
                        //Single rotation is necessary, change colors
                        ((RedBlackNode<E>) localRoot.right).isRed = false;
                        localRoot.isRed = true;
                        return rotateLeft(localRoot);
                    }
                    else if (localRoot.right.left != null && ((RedBlackNode<E>) localRoot.right.left).isRed)
                    {
                        //Right-left grand-child is also red
                        //Double rotation is necessary; change colors
                        localRoot.right = rotateRight(localRoot.right);
                        ((RedBlackNode<E>) localRoot.right).isRed = false;
                        localRoot.isRed = true;
                        return rotateLeft(localRoot);
                    }
                }
                return localRoot;
            }
        }
    }
	
	public boolean add(E item)
    {
        if(root == null)
        {
            root = new RedBlackNode<>(item);
            ((RedBlackNode<E>) root).isRed = false;
            size++;
            return true;
        }
        else
        {
            root = add((RedBlackNode<E>)root,item);
            ((RedBlackNode<E>) root).isRed = false;
            return isAdded;
        }
    }
	
	/**
	 * @return false 
	 */
	protected boolean isRed()
	{
		return ((RedBlackNode<E>)root).isRed;
	}
}
