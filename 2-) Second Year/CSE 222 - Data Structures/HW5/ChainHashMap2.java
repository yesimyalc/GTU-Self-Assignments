import java.util.Iterator;
import java.util.LinkedList;
import java.util.TreeSet;

/**
 * A class that acts like an HashMap.
 * Uses chaining technique with TreeSet
 * @param <K> is the type for the keys
 * @param <V> is the type for the values
 */
public class ChainHashMap2<K, V> implements KWHashMap<K, V>
{
	/**The table that holds the entries in an array*/
	private TreeSet<Entry<K, V>>[] table;
	/**Number of entries inside the map*/
	private int numKeys;
	/**Capacity of the map table*/
	private int CAPACITY;
	/**Maximum load treshold that is allowed*/
	private static final double LOAD_TRESHOLD=3.0;
	
	private static class Entry<K, V> implements Comparable
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

		/**
		 * Compares the keys of the entries
		 * @return a number bigger than 0 if the current key is greater, a number smaller than 0 if the current key is smaller, 0 if they are the same.
		 */
		public int compareTo(Object o) 
		{
			return ((Comparable)key).compareTo(((ChainHashMap2.Entry) o).getKey());
		}
	}
	
	/**
	 * A constructor that sets the CAPACITY of the map as 5
	 */
	public ChainHashMap2()
	{
		CAPACITY=5;
		table=new TreeSet[CAPACITY];
		numKeys=0;
	}
	
	/**
	 * A constructor that sets the CAPACITY of the map as the given value.
	 * However, if the value is smaller than 5, sets the CAPACITY as 5.
	 * @param capValue
	 */
	public ChainHashMap2(int capValue)
	{
		if(capValue>5)
			CAPACITY=capValue;
		else
			CAPACITY=5;
		
		table=new TreeSet[CAPACITY];
		numKeys=0;
	}
	
	/**
	 * Doubles then increments by 1 the CAPACITY of the map then reinserts all the elements one by one.
	 * Resets the numKeys.
	 */
	private void rehash()
	{
		TreeSet<Entry<K, V>>[] oldTable=table;
		table=new TreeSet[2*table.length+1];
		numKeys=0;
		
		for(int i=0; i<oldTable.length; ++i)
			if(oldTable[i]!=null)
			{
				Iterator<Entry<K, V>> treeIter=(Iterator<Entry<K, V>>)oldTable[i].iterator();
				while(treeIter.hasNext())
				{
					Entry<K, V> temp=treeIter.next();
					put(temp.getKey(), temp.getVal());
				}
			}
		
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
		
		Iterator<Entry<K, V>> treeIter=(Iterator<Entry<K, V>>)table[index].iterator();
		while(treeIter.hasNext())
		{
			Entry<K, V> temp=treeIter.next();
			if(temp.getKey().equals(key))
				return temp.getVal();
		}
		
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
			table[index]=new TreeSet<Entry<K, V>>();
		
		Iterator<Entry<K, V>> treeIter=(Iterator<Entry<K, V>>)table[index].iterator();
		while(treeIter.hasNext())
		{
			Entry<K, V> temp=treeIter.next();
			V valTemp=temp.getVal();
			if(temp.getKey().equals(key))
			{
				temp.setVal(value);
				return valTemp;
			}
		}
		
		table[index].add(new Entry<K, V>(key, value));
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
		
		Iterator<Entry<K, V>> treeIter=(Iterator<Entry<K, V>>)table[index].iterator();
		while(treeIter.hasNext())
		{
			Entry<K, V> temp=treeIter.next();
			if(temp.getKey().equals(key))
			{
				table[index].remove(temp);
				numKeys--;
				return temp.getVal();
			}
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
				Iterator<Entry<K, V>> treeIter=(Iterator<Entry<K, V>>)table[i].iterator();
				while(treeIter.hasNext())
				{
					Entry<K, V>next=treeIter.next();
					System.out.println("The entry is: "+next.getKey()+", "+next.getVal());
				}
				System.out.println();
			}
	}
}
