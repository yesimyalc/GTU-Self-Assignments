
public class RotatingBinarySearchTree<E> extends BinarySearchTree<E>
{
	/**
	 * Rotates one to left starting from the node
	 * @param node is the node
	 * @return the new replacement for the node
	 */
	protected Node<E> rotateLeft(Node<E> node)
	{
		Node<E> temp=node.right;
		node.right=temp.left;
		temp.left=node;
		return temp;
	}
	
	/**
	 * Rotates one to right starting from the node
	 * @param node is the node
	 * @return the new replacement for the node
	 */
	protected Node<E> rotateRight(Node<E> node)
	{
		Node<E> temp=node.left;
		node.left=temp.right;
		temp.right=node;
		return temp;
	}
}
