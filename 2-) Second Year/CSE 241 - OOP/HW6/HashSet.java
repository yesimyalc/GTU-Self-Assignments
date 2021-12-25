
/**
*@author Yeşim_Yalçın
*A container class that works like a real HashSet
*This container class can hold elements that have their own equals() function
*/
public class HashSet<E> implements Set<E>, Cloneable
{
	private E[] theSet;
	
	//Constructors
	/**
	 *Constructor that sets the HashSet size as 0
	 */
	public HashSet()
	{
		this(0);
	}
	
	/**
	 * Constructor that sets the HashSet size as the given size. 
	 * It is a private constructor to prevent null initialized elements.
	 */
	private HashSet(int size)
	{
		theSet=(E[])new Object[size];
		for(int i=0; i<size(); ++i)
			theSet[i]=(E)new Object();
	}
	
	/**
	 * This method clones the HashSet's theSet field.
	 * However, the elements in the field will not be cloned, they will be directly copied.
	 */
	public Object clone()
	{
		HashSet<E> temp=new HashSet<E>(size());
		
		for(int i=0; i<size(); ++i)
			temp.theSet[i]=theSet[i];

		return temp;
	}
	
	//Getters
	/**Returns the number of the elements in the set.*/
	public int size(){return theSet.length;}
	
	/**
	 * Adds the specified element to this set if it is not already present.
	 * Returns true if it manages to add the element.
	 * @param e is an element which should have the same generic type.
	 */
	public boolean add(E e) 
	{
		if(contains(e)==false)
		{
			HashSet<E> temp=(HashSet<E>)clone();
			theSet=(E[])new Object[size()+1];
			
			for(int i=0; i<temp.size(); ++i)
				theSet[i]=temp.theSet[i];
			
			theSet[size()-1]=e;
		}
		else
			return false;

		return true;
	}
	
	/**
	 * Adds all of the elements in the specified collection to this set 
	 * if they are not already present. 
	 * Returns true, if it manages to add at least one element.
	 */
	public boolean addAll(Collection<?> c)
	{
		int addedCount=0;
		
		try
		{
			Iterator<E> cIterator=(Iterator<E>) c.iterator();
			while(cIterator.hasNext())
				if(add(cIterator.next()))
					addedCount++;
			
			if(addedCount!=0)
				return true;
		}
		catch(NoNextElement exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
		
		return false;
	}
	
	/**Removes all of the elements from this set.*/
	public void clear()
	{
		if(isEmpty()==true)
			return;
		
		theSet=(E[])new Object[0];
	}
	
	/**
	 * Returns true if this set contains the specified element.
	 * @param e is an element which should have the same generic type.
	 */
	public boolean contains(E e)
	{
		for(int i=0; i<size(); ++i)
			if(theSet[i].equals(e))
				return true;

		return false;
	}
	
	/**
	 * Returns true if this set contains all of the elements
	 * in the specified collection.
	 */
	public boolean containsAll(Collection<?> c)
	{
		E temp;
		
		try
		{
			Iterator<E> cIterator=(Iterator<E>) c.iterator();
			while(cIterator.hasNext())
			{
				temp=cIterator.next();
				for(int i=0; i<size(); ++i)
				{
					if(theSet[i].equals(temp))
						break;
					else if(theSet[i].equals(temp)==false && i==size()-1)
						return false;
				}
			}
		}
		catch(NoNextElement exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
		
		return true;
	}
	
	/**Returns true if this set contains no elements.*/
	public boolean isEmpty()
	{
		if(size()!=0)
			return false;
						
		return true;
	}
	
	/**
	 * Removes a single instance of the specified element from this set, if it is present.
	 * Returns true if it removes.
	 * @param e is an element which should have the same generic type.
	 */
	public boolean remove(E e)
	{
		HashSet<E> temp;
		
		if(contains(e))
		{
			temp=(HashSet<E>)clone();
			theSet=(E[])new Object[temp.size()-1];
			
			for(int j=0, k=0; j<temp.size(); ++j, ++k)
			{
				if(temp.theSet[j].equals(e))
					--k;
				else if (temp.theSet[j].equals(e)==false)
					theSet[k]=temp.theSet[j];
			}				
			return true;
		 }
			
		return false;
	 }
	
	/**
	 * Removes all of this set's elements that are also contained in the specified collection.
	 * Returns true if it removes at least one element.
	 */
	public boolean removeAll(Collection<?> c)
	{	
		int removedCounter=0;
		Iterator<E> cIterator=(Iterator<E>) c.iterator();
		
		try
		{
			while(cIterator.hasNext())
				if(remove(cIterator.next())==true)
					removedCounter++;
			
			if(removedCounter==0)
				return false;
		}
		catch(NoNextElement exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
		
		return true;
	}
	
	/**
	 * Retains only the elements in this set that are contained in the specified collection.
	 * Returns true if it retains at least one element.
	 */
	public boolean retainAll(Collection<?> c)
	{
		Iterator<E> cIterator=(Iterator<E>) c.iterator();
		int retainedCounter=0;
		E temp;
		
		try
		{
			for(int i=0; i<size(); ++i)
			{
				while(cIterator.hasNext())
				{
					temp=(E)cIterator.next();
					if(theSet[i].equals(temp)==false && cIterator.hasNext()==false)
						remove(theSet[i]);
					else if(theSet[i].equals(temp)==true)
					{
						retainedCounter++;
						break;
					}
				}	
				cIterator=(Iterator<E>) c.iterator();
			}
			
			if(retainedCounter==0)
				return false;
		}
		catch(NoNextElement exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
		
		return true;
	}
	
	/**Returns an iterator over the elements in this set.*/
	public Iterator<E> iterator()
	{
		Iterator<E> it = new Iterator<E>() 
		{
			private int currentIndex = 0;

			/**Returns true if the iteration has more elements.*/
	        public boolean hasNext() 
	        {
	            if(currentIndex<size())
	            	return true;
	            
	            return false;
	        }

	        /**
	    	 * Returns the next element in the iteration and advances the iterator.
	    	 * @throws NoNextElement if there is no next element to return.
	    	 */
	        public E next() throws NoNextElement 
	        {
	        	if(hasNext()==false)
	        		throw new NoNextElement("Error, There is no next element");
	        		
	        	return theSet[currentIndex++];
	        }

	        /**Removes from the underlying collection the last element returned by this iterator.*/
	        public void remove()
	        {
	        	if(isEmpty() || currentIndex==0)
	        		return;
	        	
	        	HashSet<E> temp=new HashSet<E>(size());
	    		for(int i=0; i<size(); ++i)
	    			temp.theSet[i]=theSet[i];
	            
	            theSet=(E[])new Object[temp.size()-1];
	            for(int i=0, k=0; i<temp.size(); ++i, ++k)
	            {
	            	if(i!=currentIndex-1)
	            		theSet[k]=temp.theSet[i];
	            	else
	            		k--;
	            }
	        }
		};
    return it;
	}
}
