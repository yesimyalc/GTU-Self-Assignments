
/**
*This is the interface many of the holder classes will implement from
*@author Yeşim_Yalçın
*/
public interface Collection<E>
{
	/**Returns the number of the elements in the collection.*/
	int size();
	
	/**
	 * Ensures that this collection contains the specified element.
	 * @param e is an element which should have the same generic type.
	 */
	boolean add(E e);
	
	/**Adds all of the elements in the specified collection to this one.*/
	boolean addAll(Collection<?> c);
	
	/**Removes all of the elements from this collection.*/
	void clear();
	
	/**
	 * Returns true if this collection contains the specified element.
	 * @param e is an element which should have the same generic type.
	 */
	boolean contains(E e);
	
	/**
	 * Returns true if this collection contains all of the elements in the specified collection.
	 */
	boolean containsAll(Collection<?> c);
	
	/**Returns true if this collection contains no elements.*/
	boolean isEmpty();
	
	/**
	 * Removes a single instance of the specified element from this collection, if it is present.
	 * @param e is an element which should have the same generic type.
	 */
	boolean remove(E e);
	
	/**
	 * Removes all of this collection's elements that are also contained in the specified collection.
	 */
	boolean removeAll(Collection<?> c);
	
	/**
	 * Retains only the elements in this collection that are contained in the specified collection.
	 */
	boolean retainAll(Collection<?> c);
	
	/**Returns an iterator over the collection.*/
	Iterator<E> iterator();
}
