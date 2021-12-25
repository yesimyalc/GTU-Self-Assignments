
/**
*This is the interface for the iterators that will be used for holders
*@author Yeşim_Yalçın
*/
public interface Iterator<T>
{
	/**
	 *Returns true if the iteration has more elements.
	 */
	boolean hasNext();
	
	/**
	 * Returns the next element in the iteration and advances the iterator.
	 * @throws NoNextElement if there is no next element to return.
	 */
	T next() throws NoNextElement;
	
	/**
	 * Removes from the underlying collection the last element returned by this iterator. 
	 * @throws InvalidType if used with queue types.
	 */
	void remove() throws InvalidType;
}
