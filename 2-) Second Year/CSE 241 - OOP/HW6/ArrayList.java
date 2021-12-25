
/**
*@author Yeşim_Yalçın
*A container class that works like a real ArrayList
*This container class can hold elements that have their own equals() function
*/
public class ArrayList<E> implements List<E>
{
	private E[] theArrayList;
	
	//Constructors
	/**
	 * Constructor that sets ArrayList size as 0
	 */
	public ArrayList()
	{
		this(0);
	}
	
	/**
	 * Constructor that sets the ArrayList size as the given size. 
	 * It is a private constructor to prevent null initialized elements.
	 */
	private ArrayList(int size)
	{
		theArrayList=(E[])new Object[size];
		for(int i=0; i<size(); ++i)
			theArrayList[i]=(E)new Object();
	}
	
	/**
	 * This method clones the ArrayList's theArrayList field.
	 * However, the elements in the field will not be cloned, they will be directly copied.
	 */
	public Object clone()
	{
		ArrayList<E> temp=new ArrayList<E>(size());
		for(int i=0; i<size(); ++i)
			temp.theArrayList[i]=theArrayList[i];
		
		return temp;
	}
	
	//Getters
	/**Returns the number of the elements in the list.*/
	public int size() {return theArrayList.length;}
	
	/**
	 * Returns the element at the specified position in this list.
	 * @throws InvalidIndex if there is no element with that index in the list.
	 */
	public E get(int index) throws InvalidIndex
	{
		if(index>=size() || index<0)
			throw new InvalidIndex("Error, There is no element with this index.");
		
		try
		{
			Iterator<E> listIterator=iterator();
			for(int i=0; i<index; ++i)
				listIterator.next();
			
			return listIterator.next();
		}
		catch(NoNextElement exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
		return (E) new Object();
	}
	
	/**
	 * Returns the index of the first occurrence of the specified element in this list
	 * or -1 if this list does not contain the element.
	 * @param e is an element which should have the same generic type.
	 */
	public int indexOf(E e)
	{
		for(int i=0; i<size(); ++i)
			if(theArrayList[i]==e)
				return i;
		
		return -1;
	}
	
	/**
	 * Appends the specified element to the end of this list.
	 * Returns true after adding.
	 * @param e is an element which should have the same generic type.
	 */
	public boolean add(E e)
	{
		ArrayList<E> temp=(ArrayList<E>)clone();
		theArrayList=(E[])new Object[size()+1];
		
		for(int i=0; i<temp.size(); ++i)
			theArrayList[i]=temp.theArrayList[i];
		
		theArrayList[size()-1]=e;

		return true;
	}
	
	/**
	 * Inserts the specified element at the specified position in this list.
	 * @throws InvalidIndex if the index is not valid.
	 * @param e is an element which should have the same generic type.
	 */
	public void add(int index, E element) throws InvalidIndex
	{
		if(index>size() || index<0)
			throw new InvalidIndex("Error, This index no does not exist.");
		
		if(index==size())
		{
			add(element);
			return;
		}
		
		ArrayList<E> temp=(ArrayList<E>)clone();
		theArrayList=(E[])new Object[size()+1];
		
		for(int i=0; i<index; ++i)
			theArrayList[i]=temp.theArrayList[i];
		
		theArrayList[index]=element;
		
		for(int i=index+1; i<size(); ++i)
			theArrayList[i]=temp.theArrayList[i-1];
		
	}
	
	/**
	 * Appends all of the elements in the specified collection to the end of this list
	 * in the order that they are returned by the specified collection's iterator.
	 * Returns true after adding.
	 */
	public boolean addAll(Collection<?> c)
	{
		try
		{
			Iterator<E> cIterator=(Iterator<E>) c.iterator();
			while(cIterator.hasNext())
				add(cIterator.next());
		}
		catch(NoNextElement exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
		
		return true;
	}
	
	/**
	 * Inserts all of the elements in the specified collection into this list at the specified position.
	 * If the index is invalid InvalidIndex exception can be caught
	 * Returns true after adding.
	 */
	public boolean addAll(int index, Collection<?> c)
	{
		Iterator<E> cIterator=(Iterator<E>) c.iterator();
		int i=0;
		
		try
		{
			while(cIterator.hasNext())
			{
				add(index+i, cIterator.next());
				++i;
			}
		}
		catch(NoNextElement exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
		catch(InvalidIndex exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
		
		return true;
	}
	
	/**Removes all of the elements from this list.*/
	public void clear()
	{
		if(isEmpty()==true)
			return;
		
		theArrayList=(E[])new Object[0];
	}
	
	/**
	 * Returns true if this list contains the specified element.
	 * @param e is an element which should have the same generic type.
	 */
	public boolean contains(E e)
	{
		for(int i=0; i<size(); ++i)
			if(theArrayList[i].equals(e))
				return true;

		return false;
	}
	
	/**
	 * Returns true if this list contains all of the elements in the specified collection.
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
					if(theArrayList[i].equals(temp))
						break;
					else if(theArrayList[i].equals(temp)==false && i==size()-1)
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
	
	/**Returns true if this list contains no elements.*/
	public boolean isEmpty()
	{
		if(size()!=0)
			return false;
						
		return true;
	}
	
	/**
	 * Removes the first occurrence of the specified element from this list if it is present.
	 * Returns true if it removes.
	 * @param e is an element which should have the same generic type.
	 */
	public boolean remove(E e)
	{
		ArrayList<E> temp;
		int removedCount=0;
		
		if(contains(e))
		{
			temp=(ArrayList<E>)clone();
			theArrayList=(E[])new Object[temp.size()-1];
			
			for(int j=0, k=0; j<temp.size(); ++j, ++k)
			{
				if(removedCount==0 && temp.theArrayList[j].equals(e))
				{
					--k;
					removedCount++;
				}
				else 
					theArrayList[k]=temp.theArrayList[j];
			}				
			return true;
		 }
			
		return false;
	}
	
	/**
	 * Removes the element at the specified position in this list.
	 * @throws InvalidIndex if the index is invalid.
	 * Returns true after removing.
	 */
	public boolean remove(int index) throws InvalidIndex
	{
		if(index>=size() || index<0)
			throw new InvalidIndex("Error, This index no does not exist.");
		
		remove(theArrayList[index]);
		
		return true;
	}
	
	/**
	 * Removes from this list all of its elements that are contained in the specified collection.
	 * Returns true if it removes at least one element.
	 */
	public boolean removeAll(Collection<?> c)
	{
		int removedCounter=0;
		Iterator<E> cIterator=(Iterator<E>) c.iterator();
		
		try
		{
			while(cIterator.hasNext())
			{
				E temp=(E)cIterator.next();
				while(contains(temp))
				{
					remove(temp);
					removedCounter++;
				}
					
			}
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
	 * Retains only the elements in this list that are contained in the specified collection.
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
					if(theArrayList[i].equals(temp)==false && cIterator.hasNext()==false)
						remove(theArrayList[i]);
					else if(theArrayList[i].equals(temp)==true)
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
	
	/**Returns an iterator over the elements in this list in proper sequence.*/
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
	         * @param e is an element which should have the same generic type.
	    	 */
	        public E next() throws NoNextElement 
	        {
	        	if(hasNext()==false)
	        		throw new NoNextElement("Error, There is no next element");
	        		
	        	return theArrayList[currentIndex++];
	        }

	        /**Removes from the underlying collection the last element returned by this iterator.*/ 
	        public void remove()
	        {
	        	if(isEmpty() || currentIndex==0)
	        		return;
	        	
	        	ArrayList<E> temp=new ArrayList<E>(size());
	    		for(int i=0; i<size(); ++i)
	    			temp.theArrayList[i]=theArrayList[i];
	            
	            theArrayList=(E[])new Object[temp.size()-1];
	            for(int i=0, k=0; i<temp.size(); ++i, ++k)
	            {
	            	if(i!=currentIndex-1)
	            		theArrayList[k]=temp.theArrayList[i];
	            	else
	            		k--;
	            }
	        }
		};
    return it;
	}
}
