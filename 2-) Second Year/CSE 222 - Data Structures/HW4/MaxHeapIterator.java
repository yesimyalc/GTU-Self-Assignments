import java.util.Iterator;

/**
 * An interface class for max heap's iterator. Adds a setVal method
 */
public interface MaxHeapIterator<E> extends Iterator<E>
{
	/**
	 * Sets the lastReturned element as the passed parameter. Also the amount of the element can be updated
	 * @param value is the new element
	 * @param amount is the new amount of the element
	 * @return the old value of the element
	 */
	public E setVal(E value, int amount);
}
