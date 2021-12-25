
/**
*This is the interface set type of the holder classes will implement from
*@author Yeşim_Yalçın
*/
public interface Set<E> extends Collection<E>
{	
	/**Returns the number of the elements in the set.*/
	int size();
	
	/**
	 * Adds the specified element to this set if it is not already present.
	 * Returns true if it manages to add the element.
 	 * @param e is an element which should have the same generic type.
	 */
	boolean add(E e);
	
	 /**
	  * Adds all of the elements in the specified collection to this set if they are not already present. 
	  * Returns true, if it manages to add at least one element.
	  */
	boolean addAll(Collection<?> c);
	
	/**Removes all of the elements from this set.*/
	void clear();
	
	//Returns true if this set contains the specified element.
	boolean contains(E e);
	
	/**
	 * Returns true if this set contains all of the elements
	 * in the specified collection.
	 */
	boolean containsAll(Collection<?> c);
	
	/**Returns true if this set contains no elements.*/
	boolean isEmpty();
	
	/**
	 * Removes a single instance of the specified element from this set, if it is present.
	 * Returns true if it manages to remove.
	 * @param e is an element which should have the same generic type.
	 */
	boolean remove(E e);
	
	/**
	 * Removes all of this set's elements that are also contained in the specified collection.
	 * Returns true if it manages to remove at least one element.
	 */
	boolean removeAll(Collection<?> c);
	
	/**
	 * Retains only the elements in this set that are contained in the specified collection.
	 * Returns true if it manages to retain at least one element.
	 */
	boolean retainAll(Collection<?> c);
	
	/**Returns an iterator over the elements in this set.*/
	Iterator<E> iterator();
}
