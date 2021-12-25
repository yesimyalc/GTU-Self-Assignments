
/**
 * An interface for tree classes.
 */
public interface SearchTree<E> 
{
	/** Starter method add.
	 pre: The object to insert must implement the
	 Comparable interface.
	 @param item The object being inserted
	 @return true if the object is inserted, false
	 if the object already exists in the tree
	*/
	public boolean add(E item);
	/**
	 * @param target is the element that will be searched inside the tree
	 * @return true if the element is found, false if not
	 */
	public boolean contains(E target);
	/** Starter method find.
	 pre: The target object must implement
	 the Comparable interface.
	 @param target The Comparable object being sought
	 @return The object, if found, otherwise null
	*/
	public E find(E target);
	/** Starter method delete.
	 post: The object is not in the tree.
	 @param target The object to be deleted
	 @return The object deleted from the tree
	 or null if the object was not in the tree
	 @throws ClassCastException if target does not implement
	 Comparable
	*/
	public E delete(E target);
}
