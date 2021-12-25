
public class HelperMethods
{
	/**
	 * Checks if theTree is an AVLTree, RedBlackTree or a regular BinarySearchTree
	 * The result is printed on the screen.
	 */
	public static <E> void AVLOrRedBlack(BinarySearchTree<E> theTree) 
	{
		try
		{
			if(!theTree.isRed())
				System.out.println("The tree is a RedBlackTree.");
		}
		catch(NotRedBlackTreeException exc)
		{
			System.out.println(exc);
		}
	}
}
