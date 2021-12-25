import java.util.LinkedList;

/**
 * A class that acts like an HashMap.
 * Uses chaining technique with LinkedList
 * @param <K> is the type for the keys
 * @param <V> is the type for the values
 */
public class ChainHashMap<K, V> implements KWHashMap<K,V>
{
	/**The table that holds the entries in an array*/
	private LinkedList<Entry<K, V>>[] table;
	/**Number of entries inside the map*/
	private int numKeys;
	/**Capacity of the map table*/
	private int CAPACITY;
	/**Maximum load treshold that is allowed*/
	private static final double LOAD_TRESHOLD=3.0;
	
	/** 
	 * A class that holds the information of the key-value pairs inside the map
	 * @param <K> is the type for the keys
	 * @param <V> is the type for the values
	 */
	private static class Entry<K, V>
	{
		private K key;
		private V value;
		
		/**
		 * A constructor that sets the key and value as the given keyVal and valVal
		 */
		public Entry(K keyVal, V valVal)
		{
			key=keyVal;
			value=valVal;
		}
		
		/**
		 * @return the key
		 */
		public K getKey() {return key;}
		
		/**
		 * @return the value
		 */
		public V getVal() {return value;}
		
		/**
		 * Sets the value of the entry as the given value
		 * @param newVal will be the new value
		 */
		public void setVal(V newVal)
		{
			value=newVal;
		}
		
		/**
		 * Compares the keys of the entries
		 * @return true if the keys are equal, false if not
		 */
		public boolean equals(Object other)
		{
			return key.equals(((Entry) other).getKey());
		}
	}
	
	/**
	 * A constructor that sets the CAPACITY of the map as 5
	 */
	public ChainHashMap()
	{
		CAPACITY=5;
		table=new LinkedList[CAPACITY];
		numKeys=0;
	}

	/**
	 * A constructor that sets the CAPACITY of the map as the given value.
	 * However, if the value is smaller than 5, sets the CAPACITY as 5.
	 * @param capValue
	 */
	public ChainHashMap(int capValue)
	{
		numKeys=0;
		
		if(capValue>5)
			CAPACITY=capValue;
		else
			CAPACITY=5;
		
		table=new LinkedList[CAPACITY];
	}

	/**
	 * Doubles then increments by 1 the CAPACITY of the map then reinserts all the elements one by one.
	 * Resets the numKeys.
	 */
	private void rehash()
	{
		LinkedList<Entry<K, V>>[] temp=table;
		table=new LinkedList[2*table.length+1];
		numKeys=0;
		
		for(int i=0; i<temp.length; ++i)
			if(temp[i]!=null)
				for(Entry<K, V> nextItem: temp[i])
					if(nextItem!=null)
						put(nextItem.getKey(), nextItem.getVal());
		
		CAPACITY=table.length;
	}
	
	/**
	 * @return how many entries are inside the map.
	 */
	public int size() {return numKeys;}
	/**
	 * @return the capacity of the map.
	 */
	public int getCapacity() {return CAPACITY;}
	
	/**
	 * @return true if the map has no entries inside, false if it has.
	 */
	public boolean isEmpty() 
	{
		if(size()==0)
			return true;
		else
			return false;
	}
	
	/**
	 * Searches for a key inside the map.
	 * @return the value of the key if it is found. If it is not found return null
	 */
	public V get(Object key) 
	{
		int index=key.hashCode()%table.length;
		if(index<0)
			index+=table.length;
		
		if(table[index]==null)
			return null;
		
		for(Entry<K,V> nextItem: table[index])
			if(nextItem.getKey().equals(key))
				return nextItem.getVal();
		
		return null;
	}
	
	/**
	 * Puts a new key-value pair to the map. If the key was already inside the map only changes the value of it.
	 * @return the old value of the key, if it did not have an old value returns null.
	 */
	public V put(K key, V value)
	{
		int index=key.hashCode()%table.length;
		if(index<0)
			index+=table.length;
		
		if(table[index]==null)
			table[index]=new LinkedList<Entry<K,V>>();
		
		for(Entry<K, V> nextItem: table[index])
			if(nextItem.getKey().equals(key))
			{
				V oldVal=nextItem.getVal();
				nextItem.setVal(value);
				return oldVal;
			}
		
		table[index].addFirst(new Entry<K,V>(key, value));
		numKeys++;
		if(numKeys>LOAD_TRESHOLD*table.length)
			rehash();
		
		return null;
	}
	
	/**
	 * Removes a key-value pair from the map. If the pair does not exists, does not remove anything.
	 * @return the removed value, if nothing is removed returns null.
	 */
	public V remove(Object key) 
	{
		int index=key.hashCode()%table.length;
		if(index<0)
			index+=table.length;
		
		if(table[index]==null)
			return null;
		
		for(Entry<K, V>nextItem: table[index])
			if(nextItem.key.equals(key))
			{
				V oldVal=nextItem.getVal();
				table[index].remove(nextItem);
				numKeys--;
				return oldVal;
			}
		
		return null;
	}
	
	/**
	 * Prints out the entries inside the map in an array like form.
	 */
	public void printMap()
	{
		for(int i=0; i<table.length; ++i)
			if(table[i]!=null)
			{
				for(Entry<K,V>nextItem: table[i])
					System.out.println("The entry is: "+nextItem.getKey()+", "+nextItem.getVal());
				System.out.println();
			}
	}
}
