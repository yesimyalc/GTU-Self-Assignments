
/**
 * A class that acts like an HashMap.
 * Uses open addressing with quadratic probing also uses coalesced hashing technique.
 * @param <K> is the type for the keys
 * @param <V> is the type for the values
 */
public class CoalescedHashMap<K, V> implements KWHashMap<K, V>
{
	/**The table that holds the entries in an array*/
	private Entry<K, V>[] table;
	/**Capacity of the map table*/
	private int START_CAPACITY;
	/**Maximum load treshold that is allowed*/
	private double LOAD_TRESHOLD;
	/**Number of entries inside the map*/
	private int numKeys;
	/**Number of deleted inside the map*/
	private int numDeletes;
	/**A dummy object to insert into deleted keys*/
	private final Entry<K,V> DELETED;

	private static class Entry<K, V>
	{
		K key;
		V value;
		/**The index of the following entry which has the same hash code as the current entry*/
		Integer nextIndex;
		
		/**
		 * A constructor that sets the key and value as the given keyVal and valVal.
		 * Sets the nextIndex as null.
		 */
		public Entry(K keyVal, V valVal)
		{
			key=keyVal;
			value=valVal;
			nextIndex=null;
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
		 * @return the nextIndex
		 */
		public Integer getNext() {return nextIndex;}
		
		/**
		 * Sets the value of the entry as the given value
		 * @param newVal will be the new value
		 */
		public void setVal(V newVal)
		{
			value=newVal;
		}
		
		/**
		 * Sets the nextIndex of the entry as the given newNIndexVal
		 * @param newNIndexVal will be the new nextIndex
		 */
		public void setNextIndex(Integer newNIndexVal)
		{
			nextIndex=newNIndexVal;
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
	 * A constructor that sets the CAPACITY of the map as 7
	 */
	public CoalescedHashMap()
	{
		START_CAPACITY=7;
		table=new Entry[START_CAPACITY];
		LOAD_TRESHOLD=0.48;
		numKeys=0;
		numDeletes=0;
		DELETED=new Entry<K, V>(null, null);
	}
	
	/**
	 * A constructor that sets the CAPACITY of the map as the given value.
	 * However, if the value is smaller than 7, sets the CAPACITY as 7.
	 * @param capValue
	 */
	public CoalescedHashMap(int capValue)
	{
		if(capValue>7)
			START_CAPACITY=capValue;
		else
			START_CAPACITY=7;
		
		table=new Entry[START_CAPACITY];
		LOAD_TRESHOLD=0.48;
		numKeys=0;
		numDeletes=0;
		DELETED=new Entry<K, V>(null, null);
	}
	
	/**
	 * A constructor that sets the CAPACITY of the map as the given value.
	 * However, if the value is smaller than 7, sets the CAPACITY as 7.
	 * Sets the LOAD_TRESHOLD as the given newLoad. It is not suggested to use this, this constructor was done only to test example in the hw pdf.
	 * @param capValue
	 */
	public CoalescedHashMap(int capValue, double newLoad)
	{
		if(capValue>7)
			START_CAPACITY=capValue;
		else
			START_CAPACITY=7;
		
		table=new Entry[START_CAPACITY];

		if(newLoad<0.1)
			LOAD_TRESHOLD=0.48;
		else
			LOAD_TRESHOLD=newLoad;
		
		numKeys=0;
		numDeletes=0;
		DELETED=new Entry<K, V>(null, null);
	}
	
	/**
	 * Searches for a key inside the map using quadratic probing.
	 * @param key is the searched key
	 * @return the first null index if the key is not found, the index that the key is in if it is found.
	 */
	private int find(Object key)
	{
		int index=key.hashCode()%table.length;
		if(index<0)
			index+=table.length;
		
		int k=-1;
		while(table[index]!=null)
		{
			if(table[index].getKey()!=null && table[index].getKey().equals(key))
				break;
			
			k+=2;
			index=(index+k)%table.length;
			if(index>=table.length)
				index=0;
		}
		
		return index;
	}
	
	/**
	 * Searches if there is any previous entry that is connected to the key inside the table.
	 * @param key is the key whose previous will be searched.
	 * @param keyIndex is the index of the key.
	 * @return the index of the previous entry if found, if not found returns -1
	 */
	private int findPrevious(Object key, int keyIndex)
	{
		int index=key.hashCode()%table.length;
		if(index<0)
			index+=table.length;
		
		int k=-1;
		int prevIndex=-1;
		
		while(index!=keyIndex)
		{	
			if(table[index].getKey()!=null)
			{
				Integer indexCode=table[index].getKey().hashCode()%table.length;
				Integer keyCode=key.hashCode()%table.length;
				
				if(table[index]!=null && indexCode.equals(keyCode) && index!=keyIndex)
					prevIndex=index;
			}
			
			k+=2;
			index=(index+k)%table.length;
			if(index>=table.length)
				index=0;
		}
		
		return prevIndex;
	}
	
	/**
	 * Doubles then increments by 1 the CAPACITY of the map, then finds the closest greatest prime number and makes it the capacity. 
	 * Then reinserts all the elements one by one.
	 * Resets the numKeys and numDeletes.
	 */
	private void rehash()
	{
		Entry<K, V> oldTable[]=table;
		int newIndex=2*table.length+1;
		newIndex++;
	    for (int i = 2; i <newIndex; i++) 
	    {
	        if(newIndex%i == 0) 
	        {
	            newIndex++;
	            i=2;
	         } 
	        else
	            continue;
	    }
	    
	    table=new Entry[newIndex];
	    numKeys=0;
	    numDeletes=0;
	    
	    for(int i=0; i<oldTable.length; ++i)
	    	if(oldTable[i]!=null && !oldTable[i].equals(DELETED))
	    		put(oldTable[i].getKey(), oldTable[i].getVal());
	    
	    START_CAPACITY=table.length;
	}
	
	/**
	 * Removes the element with the startIndex from the table.
	 * Assumed that that there is an element inside the startIndex.
	 * If the entry with the startIndex has no nextIndex then removes it and changes it with a DELETED entry.
	 * If the entry with the startIndex has nextIndexes then replaces the entry with the entry that has the nextIndex
	 * @param startIndex is the current index that is being examined inside the table
	 * @param prevIndex is the previous entry that is connected to the current entry
	 */
	private void remove(int startIndex, int prevIndex)
	{
		if(table[startIndex].getNext()==null)
		{
			if(prevIndex!=-1)
				table[prevIndex].setNextIndex(null);
			
			table[startIndex]=DELETED;
			numKeys--;
			numDeletes++;
			return;
		}
		else
		{
			table[startIndex].setVal(table[(int)(table[startIndex].nextIndex)].getVal());
			table[startIndex].key=table[(int)(table[startIndex].nextIndex)].getKey();
			remove((int)table[startIndex].getNext(), startIndex);
		}
	}
	
	/**
	 * @return how many entries are inside the map.
	 */
	public int size() {return numKeys;}
	/**
	 * @return the capacity of the map.
	 */
	public int getCapacity() {return START_CAPACITY;}
	
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
		int index=find(key);
		if(table[index]==null)
			return null;
		else
			return table[index].getVal();
	}

	/**
	 * Puts a new key-value pair to the map. If the key was already inside the map only changes the value of it.
	 * @return the old value of the key, if it did not have an old value returns null.
	 */
	public V put(K key, V value) 
	{
		int index=find(key);
		
		if(table[index]==null)
		{
			table[index]=new Entry<K, V>(key, value);
			numKeys++;
			
			int previous=findPrevious(key, index);
			if(previous!=-1)
				table[previous].setNextIndex(index);
			
			double loadFactor=(double)(numKeys+numDeletes)/table.length;
			if(loadFactor>LOAD_TRESHOLD)
				rehash();
			
			return null;
		}
		
		V oldVal=table[index].getVal();
		table[index].setVal(value);
		
		return oldVal;
	}

	/**
	 * Removes a key-value pair from the map. If the pair does not exists, does not remove anything.
	 * @return the removed value, if nothing is removed returns null.
	 */
	public V remove(Object key) 
	{
		int index=find(key);
		
		if(table[index]==null || table[index].equals(DELETED))
			return null;
		
		V temp=table[index].getVal();
		
		remove(index, findPrevious(key, index));
		
		return temp;	
	}
	
	/**
	 * Prints out the entries inside the map in an array like form.
	 */
	public void printMap()
	{
		for(int i=0; i<table.length; ++i)
			if(table[i]!=null && table[i].getKey()!=null)
			{
				System.out.print("The entry is at index: "+i+" -> "+table[i].getKey()+", "+table[i].getVal()+". Next is at index: "+table[i].getNext());
				if(table[i].getNext()!=null)
					System.out.print(" -> "+table[table[i].getNext()].getKey()+"\n");
				else
					System.out.println();
			}
		
	}
}
