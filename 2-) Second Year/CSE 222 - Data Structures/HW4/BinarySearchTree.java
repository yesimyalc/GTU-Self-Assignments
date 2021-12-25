import java.io.Serializable;

public class BinarySearchTree<E> 
{
	 // Data Fields
	/** The root of the binary tree */
	protected Node<E> root;
	 /** Return value from the public add method. */
	 protected boolean addReturn;
	 /** Return value from the public delete method. */
	 protected E deleteReturn;
	 
	/** Class to encapsulate a tree node. */
	protected static class Node<E> implements Serializable 
	{
		 // Data Fields
		 /** The information stored in this node. */
		 protected E data;
		 /** Reference to the left child. */
		 protected Node<E> left;
		 /** Reference to the right child. */
		 protected Node<E> right;
		 // Constructors
		/** Construct a node with given data and no children. 
		 @param data The data to store in this node
		 */
		 public Node(E data) 
		{
			this.data = data;
			left = null;
			right = null;
		}
		 
		 /**
		  * Returns the data field in string form
		  */
		public String toString()
		{
			return data.toString();
		}
	}
	
	public BinarySearchTree() 
	{ 
		 root = null; 
	}
	
	protected BinarySearchTree(Node<E> root) 
	{
		 this.root = root;
	}
	
	/** Constructs a new binary tree with data in its root leftTree 
	 as its left subtree and rightTree as its right subtree.
	*/
	public BinarySearchTree(E data, BinarySearchTree<E> leftTree, BinarySearchTree<E> rightTree) 
	{
		 root = new Node<>(data);
		 if (leftTree != null) 
			 root.left = leftTree.root;
		 else 
			 root.left = null; 
		 
		 if (rightTree != null) 
			 root.right = rightTree.root;
		 else 
			 root.right = null;
	}
	
	/**
	 * @return true if empty, false if not 
	 */
	public boolean isEmpty()
	{
		if(root==null)
			return true;
		
		return false;
	}
	
	/**
	 * @return the data of the first element inside the tree
	 */
	public E getRoot()
	{
		return root.data;
	}
	
	/** Return the left subtree. 
	 @return The left subtree or null if either the root or
	 the left subtree is null 
	*/
	public BinarySearchTree<E> getLeftSubtree() 
	{
		 if (root != null && root.left != null) 
			 return new BinarySearchTree<>(root.left);
		 else
			 return null;
	}
	
	/** Return the right subtree. 
	 @return The right subtree or null if either the root or
	 the right subtree is null 
	*/
	public BinarySearchTree<E> getRightSubtree() 
	{
		 if (root != null && root.right != null) 
			 return new BinarySearchTree<>(root.right);
		 else
			 return null;
	}
	
	/** Determine whether this tree is a leaf.
	 @return true if the root has no children
	*/
	public boolean isLeaf() 
	{
		return (root.left == null && root.right == null);
	}
	
	/**
	 * @param target is the element that will be searched inside the tree
	 * @return true if the element is found, false if not
	 */
	public boolean contains(E target)
	{
		if(find(target)!=null)
			return true;
		
		return false;
	}
	
	 /** Starter method find.
	 pre: The target object must implement
	 the Comparable interface.
	 @param target The Comparable object being sought
	 @return The object, if found, otherwise null
	*/
	public E find(E target) 
	{
		return find(root, target);
	}
	
	/** Recursive find method.
	 @param localRoot The local subtree's root
	 @param target The object being sought
	 @return The object, if found, otherwise null
	*/
	private E find(Node<E> localRoot, E target) 
	{
		if (localRoot == null)
			return null;
		// Compare the target with the data field at the root.
		int compResult = ((Comparable<E>)target).compareTo(localRoot.data);
		
		if (compResult == 0)
			return localRoot.data;
		else if (compResult < 0)
			return find(localRoot.left, target);
		else
			return find(localRoot.right, target);
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
		root = add(root, item);
		
		return addReturn;
	}
	
	/** Recursive add method.
	 post: The data field addReturn is set true if the item is added to
	 the tree, false if the item is already in the tree.
	 @param localRoot The local root of the subtree
	 @param item The object to be inserted
	 @return The new local root that now contains the
	 inserted item
	*/
	private Node<E> add(Node<E> localRoot, E item) 
	{
		 if (localRoot == null) 
		 {
			 // item is not in the tree Ã¢â‚¬â€� insert it.
			 addReturn = true;
			 return new Node<>(item);
		 } 
		 else if (((Comparable<E>)item).compareTo(localRoot.data) == 0) 
		 {
			 // item is equal to localRoot.data
			 addReturn = false;
			 return localRoot;
		 } 
		 else if (((Comparable<E>)item).compareTo(localRoot.data) < 0)
		 {
			 // item is less than localRoot.data
			 localRoot.left = add(localRoot.left, item);
			 return localRoot;
		 } 
		 else 
		 {
			 // item is greater than localRoot.data
			 localRoot.right = add(localRoot.right, item);
			 return localRoot;
		 }
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
		 root = delete(root, target);
		 return deleteReturn;
	}
	
	/** Recursive delete method.
	 post: The item is not in the tree;
	 deleteReturn is equal to the deleted item
	 as it was stored in the tree or null
	 if the item was not found.
	 @param localRoot The root of the current subtree
	 @param item The item to be deleted
	 @return The modified local root that does not contain
	 the item
	*/
	private Node<E> delete(Node<E> localRoot, E item) 
	{
		 if (localRoot == null) 
		 {
			 // item is not in the tree.
			 deleteReturn = null;
			 return localRoot;
		 }
		 // Search for item to delete.
		 int compResult = ((Comparable<E>)item).compareTo(localRoot.data);
		 if (compResult < 0) 
		 {
			 // item is smaller than localRoot.data.
			 localRoot.left = delete(localRoot.left, item);
			 return localRoot;
		 } 
		 else if (compResult > 0) 
		 {
			 // item is larger than localRoot.data.
			 localRoot.right = delete(localRoot.right, item);
			 return localRoot;
		 } 
		 else 
		 {
			 // item is at local root.
			 deleteReturn = localRoot.data;
			 if (localRoot.left == null) 
			 {
				 // If there is no left child, return right child
				 // which can also be null.
				 return localRoot.right;
			 } 
			else if (localRoot.right == null) 
			{
				 // If there is no right child, return left child.
				 return localRoot.left;
			} 
			else 
			{
				 // Node being deleted has 2 children, replace the data
				 // with inorder predecessor. 
				 if (localRoot.left.right == null) 
				 {
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
					 localRoot.data = findLargestChild(localRoot.left);
					 return localRoot;
				 }
			 }
		 }
	}
	
	/** Find the node that is the
	 inorder predecessor and replace it
	 with its left child (if any).
	 post: The inorder predecessor is removed from the tree.
	 @param parent The parent of possible inorder
	 predecessor (ip)
	 @return The data in the ip
	*/
	private E findLargestChild(Node<E> parent) 
	{
		 // If the right child has no right child, it is
		 // the inorder predecessor.
		 if (parent.right.right == null) 
		 {
			 E returnValue = parent.right.data;
			 parent.right = parent.right.left;
			 return returnValue;
		 } 
		 else 
		 {
			 return findLargestChild(parent.right);
		 }
	}
	
	/** Converts the tree into a string
	 Performs a preorder traversal.
	 @param node The local root
	 @param depth The depth
	 @param sb The StringBuilder to save the output
	*/
	private void toString(Node<E> node, int depth, StringBuilder sb) 
	{
		 for (int i = 1; i < depth; i++) 
			 sb.append(" ");
		 
		 if (node == null) 
			 sb.append("null\n");
		 else 
		 {
			 sb.append(node.toString());
			 sb.append("\n");
			 toString(node.left, depth + 1, sb);
			 toString(node.right, depth + 1, sb);
		 }
	}
	
	/**
	 * Starter method of the recursive toString method
	 * Returns a string version of the tree
	 */
	public String toString() 
	{
		 StringBuilder sb = new StringBuilder();
		 toString(root, 1, sb);
		 return sb.toString();
	}
	
}