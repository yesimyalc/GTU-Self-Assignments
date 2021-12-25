import java.io.Serializable;

/**
 * A class that acts like a tree.
 */
public class Tree<E>
{
	protected Node<E> root;
	protected int size;
	
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
	
	public Tree()
	{
		root=null;
		size=0;
	}
	
	/** Constructs a new binary tree with data in its root leftTree 
	 as its left subtree and rightTree as its right subtree.
	*/
	public Tree(E data, BinarySearchTree<E> leftTree, BinarySearchTree<E> rightTree)
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
	
	protected Tree(Node<E> item)
	{
		root=item;
	}
	
	/**
	 * @return the data of the first element inside the tree
	 */
	public E getRoot()
	{
		if (root != null) 
	        return root.data;
	    else
	        return null;
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
	 * 
	 * @return the size
	 */
	public int size() {return size;}
	
	/** Determine whether this tree is a leaf.
	 @return true if the root has no children
	*/
	public boolean isLeaf()
	{
		return (root.left == null && root.right == null);
	}
	
	/** Return the left subtree. 
	 @return The left subtree or null if either the root or
	 the left subtree is null 
	*/
	public Tree<E> getLeftSubtree()
	{
		if (root != null && root.left != null) 
			 return new Tree<>(root.left);
		 else
			 return null;
	}
	
	/** Return the right subtree. 
	 @return The right subtree or null if either the root or
	 the right subtree is null 
	*/
	public Tree<E> getRightSubtree()
	{
		 if (root != null && root.right != null) 
			 return new Tree<>(root.right);
		 else
			 return null;
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
	
	/**
	 * Prints the tree inOrder form.
	 */
	private void inOrderPrint(Node<E> node)
	{
		if(node==null)
			return;
		else
		{
			inOrderPrint(node.left);
			System.out.println(node.toString());
			inOrderPrint(node.right);
		}
	}
	
	/**
	 * Prints the tree inOrder form.
	 */
	public void inOrderPrint()
	{
		inOrderPrint(root);
	}
}
