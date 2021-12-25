import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

/**
 * A subclass of HashMap that has a custom iterator.
 * @param <K> is the type for the keys
 * @param <V> is the type for the values
 */
public class IterableHashMap<K, V> extends HashMap<K, V>
{
	/**@return an custom iterator that starts from the first element*/
	public Iterator<K> iterator(){return new MapIterator();}
	/**@return a custom iterator that starts from the given element*/
	public Iterator<K> iterator(K key){return new MapIterator(key);}
	
	/**
	 * A custom class that allows to iterate over the elements of a HashMap
	 */
	public class MapIterator implements Iterator
	{
		/**Is the entry that can be returned from next() method*/
		Map.Entry<K, V> nextEntry;
		/**Is the entry that can be returned from previous() method*/
		Map.Entry<K, V> prevEntry;
		/**Holds all the entries that have been iterated so far*/
		HashSet<K> iterated;
		
		/**
		 * A constructor that sets the nextEntry as the first element inside the map and
		 * sets the prevEntry as the last element inside the map.
		 */
		public MapIterator()
		{
			setNextEntry();
			setPrevEntry();
			iterated=new HashSet<K>();
		}
		
		/**
		 * A constructor that sets the nextEntry as the given element inside the map and
		 * sets the prevEntry as the previous entry of this element. If the element was the first element acts like the no parameter costructor.
		 */
		public MapIterator(K key)
		{
			iterated=new HashSet<K>();
			if(!containsKey(key))
			{
				setNextEntry();
				setPrevEntry();
			}
			else 
			{
				Iterator iter= entrySet().iterator();
				Map.Entry<K, V> temp=null;
				Map.Entry<K, V> prevTemp=null;
				while(iter.hasNext())
				{
					temp=(Map.Entry<K, V>) iter.next();
					if(temp.getKey().equals(key))
					{
						nextEntry=temp;
						if(prevTemp==null)
							setPrevEntry();
						else
							prevEntry=prevTemp;
						
						break;
					}
					prevTemp=temp;
				}
			}
		}
		
		/**
		 * Sets the next entry considering the iterator will start from the first element. 
		 */
		private void setNextEntry()
		{
			if(isEmpty())
				nextEntry=null;
			else
			{
				Iterator iter= entrySet().iterator();
				nextEntry=(Map.Entry<K, V>) iter.next();
			}
		}
		
		/**
		 * Sets the previous entry considering the iterator will start from the first element.
		 */
		private void setPrevEntry()
		{
			if(isEmpty())
				prevEntry=null;
			else
			{
				Iterator iter= entrySet().iterator();
				Map.Entry<K, V> temp=null;
				while(iter.hasNext())
					temp=(Map.Entry<K, V>) iter.next();
				
				prevEntry=temp;
			}
		}
		
		/**
		 * @return True if there are still not-iterated key/s in the Map, otherwise returns False.
		 */
		public boolean hasNext() 
		{
			if(iterated.size()==size())
				return false;
			else 
				return true;
		}
	
		/**
		 * @return  the next key in the Map. It returns the first key when there is no not-iterated key in the Map.
		 */
		public K next() 
		{
			if(isEmpty())
				return null;
			else if(size()==1)
			{
				iterated.add(nextEntry.getKey());
				return nextEntry.getKey();
			}
			
			prevEntry=nextEntry;
			Iterator iter= entrySet().iterator();
			while(iter.hasNext())
			{
				Map.Entry<K, V> temp=(Map.Entry<K, V>) iter.next();
				if(temp.equals(nextEntry) && iter.hasNext())
				{
					nextEntry=(Map.Entry<K, V>)iter.next();
					break;
				}
				else if(temp.equals(nextEntry) && !iter.hasNext())
				{
					setNextEntry();
					break;
				}
			}
			
			iterated.add(prevEntry.getKey());
			return prevEntry.getKey();
		}
	
		/**
		 * @return  the last key when the iterator is at the first key.
		 */
		public K previous() 
		{
			if(isEmpty())
				return null;
			else if(size()==1)
			{
				iterated.add(nextEntry.getKey());
				return nextEntry.getKey();
			}
			
			nextEntry=prevEntry;
			Iterator iter= entrySet().iterator();
			Map.Entry<K, V> temp=null;
			Map.Entry<K, V> prevTemp=null;
			while(iter.hasNext())
			{
				temp=(Map.Entry<K, V>) iter.next();
				if(temp.equals(prevEntry) && prevTemp!=null)
				{
					prevEntry=prevTemp;
					break;
				}
				else if(temp.equals(prevEntry) && prevTemp==null)
				{
					setPrevEntry();
					break;
				}	
				prevTemp=temp;
			}
		
			iterated.add(nextEntry.getKey());
			return nextEntry.getKey();
		}

	}
}
