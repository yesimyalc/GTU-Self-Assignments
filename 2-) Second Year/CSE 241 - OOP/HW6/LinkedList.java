
/**
*@author Yeşim_Yalçın
*A container class that works like a real LinkedList
*This container class can hold elements that have their own equals() function
*/
public class LinkedList<E> implements List<E>, Queue<E>
{
	private E[] theLinkedList;
	
	//Constructors
	/** Constructor that sets ArrayList size as 0*/
	public LinkedList()
	{
		this(0);
	}
	
	/**
	 * Constructor that sets the ArrayList size as the given size. 
	 * It is a private constructor to prevent null initialized elements.
	 */
	public LinkedList(int size)
	{
		theLinkedList=(E[])new Object[size];
		for(int i=0; i<size(); ++i)
			theLinkedList[i]=(E)new Object();
	}
	
	/**
	 * This method clones the LinkedList's theLinkedList field.
	 * However, the elements in the field will not be cloned, they will be directly copied.
	 */
	public Object clone()
	{
		LinkedList<E> temp=new LinkedList<E>(size());
		for(int i=0; i<size(); ++i)
			temp.theLinkedList[i]=theLinkedList[i];
		
		return temp;
	}
	
	//Getters
	/**Returns the number of the elements in the list.*/
	public int size() {return theLinkedList.length;}
	
	/**
	 * Returns the element at the specified position in this list.
	 * @param e is an element which should have the same generic type.
	 * @throws InvalidIndex if there is no element in the given index.
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
			if(theLinkedList[i]==e)
				return i;
		
		return -1;
	}
	
	/**
	 * Retrieves, but does not remove, the head of this queue.
	 * @throws NoElement if it is empty.
	 */
	public E element() throws NoElement
	{
		if(isEmpty()==true)
			throw new NoElement("There is no element in the Linked List.");
		
		return theLinkedList[0];
	}
	
	/**
	 * Retrieves and removes the head of this queue, 
	 * @throws NoElement if it is empty.
	 */
	public E poll() throws NoElement
	{
		if(isEmpty()==true)
			throw new NoElement("There is no element in the Linked List.");
		
		E temp=theLinkedList[0];
		remove(theLinkedList[0]);
		
		return temp;
	}
	
	/**
	 * Appends the specified element to the end of this list.
	 * Returns true after adding.
	 * @param e is an element which should have the same generic type.
	 */
	public boolean add(E e)
	{
		LinkedList<E> temp=(LinkedList<E>)clone();
		theLinkedList=(E[])new Object[size()+1];
		
		for(int i=0; i<temp.size(); ++i)
			theLinkedList[i]=temp.theLinkedList[i];
		
		theLinkedList[size()-1]=e;

		return true;
	}
	
	/**
	 * Inserts the specified element at the specified position in this list.
	 * @throws InvalidIndex if the index is not valid.
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
		
		LinkedList<E> temp=(LinkedList<E>)clone();
		theLinkedList=(E[])new Object[size()+1];
		
		for(int i=0; i<index; ++i)
			theLinkedList[i]=temp.theLinkedList[i];
		
		theLinkedList[index]=element;
		
		for(int i=index+1; i<size(); ++i)
			theLinkedList[i]=temp.theLinkedList[i-1];
	}
	
	/**
	 * Adds the specified element as the tail (last element) of this list.
	 * Returns true after adding
	 * @param e is an element which should have the same generic type.
	 */
	public boolean offer(E e)
	{
		add(e);
		
		return true;
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
		
		theLinkedList=(E[])new Object[0];
	}
	
	/**
	 * Returns true if this list contains the specified element.
	 * @param e is an element which should have the same generic type.
	 */
	public boolean contains(E e)
	{
		for(int i=0; i<size(); ++i)
			if(theLinkedList[i].equals(e))
				return true;

		return false;
	}
	
	/**
	 * Returns true if this list contains all of the elements
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
					if(theLinkedList[i].equals(temp))
						break;
					else if(theLinkedList[i].equals(temp)==false && i==size()-1)
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
		LinkedList<E> temp;
		int removedCount=0;
		
		if(contains(e))
		{
			temp=(LinkedList<E>)clone();
			theLinkedList=(E[])new Object[temp.size()-1];
			
			for(int j=0, k=0; j<temp.size(); ++j, ++k)
			{
				if(removedCount==0 && temp.theLinkedList[j].equals(e))
				{
					--k;
					removedCount++;
				}
				else 
					theLinkedList[k]=temp.theLinkedList[j];
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
		
		remove(theLinkedList[index]);
		
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
					if(theLinkedList[i].equals(temp)==false && cIterator.hasNext()==false)
						remove(theLinkedList[i]);
					else if(theLinkedList[i].equals(temp)==true)
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
	    	 */
	        public E next() throws NoNextElement 
	        {
	        	if(hasNext()==false)
	        		throw new NoNextElement("Error, There is no next element");
	        		
	        	return theLinkedList[currentIndex++];
	        }

	        /**Removes from the underlying collection the last element returned by this iterator.*/ 
	        public void remove() throws InvalidType
	        {
	        	throw new InvalidType("Error, cannot use remove function for queue types.");
	        }
		};
    return it;
	}
}
