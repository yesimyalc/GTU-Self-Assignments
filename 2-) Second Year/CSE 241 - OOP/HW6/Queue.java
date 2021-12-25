
/**
*This is the interface queue type of the holder classes will implement from
*@author Yeşim_Yalçın
*/
public interface Queue<E> extends Collection<E>
{
	/**
	 * Inserts the specified element into this queue.
	 * Returns true upon success.
 	 * @param e is an element which should have the same generic type.
	 */
	boolean add(E e);
	
	/**
	 * Retrieves, but does not remove, the head of this queue.
	 * @throws NoElement if it is empty.
	 */
	E element() throws NoElement;
	
	/**
	 * Adds the specified element as the tail (last element) of this list.
	 * Returns true after adding
	 * @param e is an element which should have the same generic type.
	 */
	boolean offer(E e);
	
	/**
	 * Retrieves and removes the head of this queue, 
	 * @throws NoElement if it is empty. 
	 */
	E poll() throws NoElement;
}
