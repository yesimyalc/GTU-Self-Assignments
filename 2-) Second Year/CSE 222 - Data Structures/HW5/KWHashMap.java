
/**
 * An interface for classes that will act like an HashMap.
 * @param <K> is the type for the keys
 * @param <V> is the type for the values
 */
public interface KWHashMap<K, V>
{
	/**
	 * Returns the value that this key is referring to.
	 * @return the value of the key or null if this does not belong to the map.
	 */
	public V get(Object key);
	
	/**
	 * Checks if the map is empty.
	 * @return true if it is empty, false if it is not.
	 */
	public boolean isEmpty();
	
	/**
	 * Adds a new value which has the given key to the map. 
	 * If the key is already inside the map, changes it's value.
	 * @return the old value of the key. If the key was not inside the map, returns null
	 */
	public V put(K key, V value);
	
	/**
	 * Removes the key from the map.
	 * @return the old value of the key. If the key was not inside the map, returns null
	 */
	public V remove(Object key);
	
	/**
	 * @return how many keys are stored inside the map
	 */
	public int size();
	
	/**
	 * Prints out the entries inside the map in an array like form.
	 */
	public void printMap();
	
	/**
	 * @return the capacity of the map
	 */
	public int getCapacity();
}
