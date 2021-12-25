
/**
*This is the interface list type of the holder classes will implement from
*@author Yeşim_Yalçın
*/
public interface List<E> extends Collection<E>
{
	/**Returns the number of the elements in the list.*/
	int size();
	
	/**
	 * Returns the element at the specified position in this list.
   	 * @throws InvalidIndex if the index is invalid.
	 */
	E get(int index) throws InvalidIndex;
	
	/**
	 * Returns the index of the first occurrence of the specified element in this list
	 * or -1 if this list does not contain the element.
	 * @param e is an element which should have the same generic type.
	 */
	int indexOf(E e);
	
	/**
	 * Appends the specified element to the end of this list.
	 * Returns true after adding.
	 * @param e is an element which should have the same generic type.
	 */
	boolean add(E e);
	
	/**
	 * Inserts the specified element at the specified position in this list.
	 * @throws InvalidIndex if the index is not valid.
	 * @param elemenet is an element which should have the same generic type.
	 */
	void add(int index, E element) throws InvalidIndex;
	
	/**
	 * Appends all of the elements in the specified collection to the end of this list
	 * in the order that they are returned by the specified collection's iterator.
	 * Returns true after adding.
	 */
	boolean addAll(Collection<?> c);
	
	/**
	 * Inserts all of the elements in the specified collection into this list at the specified position.
	 * If the index is invalid InvalidIndex exception can be caught
	 * Returns true after adding.
	 */
	boolean addAll(int index, Collection<?> c);
	
	/**Removes all of the elements from this list.*/
	void clear();
	
	/**
	 * Returns true if this list contains the specified element.
	 * @param e is an element which should have the same generic type.
	 */
	boolean contains(E e);
	
	/**
	 * Returns true if this list contains all of the elements in the specified collection.
	 */
	boolean containsAll(Collection<?> c);
	
	/**Returns true if this list contains no elements.*/
	boolean isEmpty();
	
	/**
	 * Removes the first occurrence of the specified element from this list if it is present.
	 * Returns true if it removes.
	 * @param e is an element which should have the same generic type.
	 */
	boolean remove(E e);
	
	/**
	 * Removes the element at the specified position in this list.
	 * @throws InvalidIndex if the index is invalid.
	 * Returns true after removing.
	 */
	boolean remove(int index) throws InvalidIndex;
	
	/**
	 * Removes from this list all of its elements that are contained in the specified collection.
	 * Returns true if it removes at least one element.
	 */
	boolean removeAll(Collection<?> c);
	
	/**
	 * Retains only the elements in this list that are contained in the specified collection.
	 * Returns true if it retains at least one element.
	 */
	boolean retainAll(Collection<?> c);
	
	/**Returns an iterator over the elements in this list in proper sequence.*/
	Iterator<E> iterator();
}
