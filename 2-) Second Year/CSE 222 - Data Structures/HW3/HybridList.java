import java.util.ListIterator;

/**
 * A hybrid container class that has a LinkedList inside and will store KWArrayLists.
 * @param <E> will be the type of the objects that will be contained inside this class.
 */
public class HybridList<E>
{
	private LList<KWArrayList<E>> theList;
	private static int MAX_NUMBER=12;
	
	/**
	 * Constructor that creates an empty list.
	 */
	public HybridList()
	{
		theList=new LList<KWArrayList<E>>();
	}
	
	/**
	 * @return the amount of the element is all ArrayLists.
	 */
	public int size() {return arraySize();}
	
	/**
	 * @return the amount of the ArrayLists in the list.
	 */
	public int listSize() {return theList.size();}
	
	/**
	 * @return the amount of the element is all ArrayLists.
	 */
	public int arraySize()
	{
		int count=0;
		ListIterator temp=theList.iterator();
		for(int i=0; i<listSize(); ++i)
			count+=((KWArrayList<E>)temp.next()).size();
		
		return count;
	}
	
	/**
	 * @param listIndex is the index for the arraylist that will be searched.
	 * @param arrayIndex is the index for the element that will be searched in an arraylist.
	 * @return the data in listIndex'th ArrayList and arrayIndex'th element of it.
	 * @throws ArrayIndexOutOfBoundsException if tried to reach an element with invalid index values.
	 */
	public E get(int listIndex, int arrayIndex)
	{
		if(listIndex<0 || listIndex>=listSize() || arrayIndex<0 || arrayIndex>=theList.get(listIndex).size())
			throw new ArrayIndexOutOfBoundsException();
		
		return theList.get(listIndex).get(arrayIndex);
	}
	
	/**
	 * Searches the whole linkedList as a whole.
	 * @param index is the index for the element that will be searched.
	 * @return the element that is stored in the given index.
	 * @throws ArrayIndexOutOfBoundsException if tried to reach an element with invalid index values.
	 */
	public E get(int index)
	{
		if(index<0 || index>=size())
			throw new ArrayIndexOutOfBoundsException();
		
		int counter=0;
		ListIterator temp=theList.iterator();
		while(counter<index)
		{
			counter+=((KWArrayList<E>)temp.next()).size();
			
			if(counter>index)
			{
				counter-=((KWArrayList<E>)temp.previous()).size();
				break;
			}
		}
		
		return ((KWArrayList<E>)temp.next()).get(index-counter);
	}
	
	/**
	 * Adds an element to the end of the linkedlist.
	 * @param element is the element that will be added.
	 */
	public void add(E element)
	{
		if(theList.size()==0)
			theList.addFirst(new KWArrayList<E>());
		else if(theList.get(listSize()-1).size()==MAX_NUMBER)
			theList.addLast(new KWArrayList<E>());
		
		theList.get(listSize()-1).add(element);
	}
	
	/**
	 * Removes the last element from the linkedlist.
	 */
	public void remove()
	{
		try
		{
			if(theList.size()==0)
				return;
			
			theList.get(listSize()-1).remove(theList.get(listSize()-1).size()-1);
			
			if(theList.get(listSize()-1).size()==0)
				theList.remove(listSize()-1);
		}
		catch(InvalidIndex exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
	}
	
	/**
	 * Removes the element with the given index from the linkedlist.
	 * @param index is the index of the element that will be removed. Should be smaller than size, bigger than 0.
	 * @throws ArrayIndexOutOfBoundsException if tried to reach an element with invalid index values.
	 */
	public void remove(int index)
	{
		if(index<0 || index>=size())
			throw new ArrayIndexOutOfBoundsException();
		
		try
		{
			int counter=0;
			ListIterator temp=theList.iterator();
			while(counter<index)
			{
				counter+=((KWArrayList<E>)temp.next()).size();
				
				if(counter>index)
				{
					counter-=((KWArrayList<E>)temp.previous()).size();
					break;
				}
			}
			
			((KWArrayList<E>)temp.next()).remove(index-counter);
			
			if(((KWArrayList<E>)temp.previous()).size()==0)
				theList.remove(((KWArrayList<E>)temp.next()));
		}
		catch(InvalidIndex exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}	
	}
	
	/**
	 * Removes an element from the linkedList with the given index values.
	 * @param listIndex is the index of the arrayList that the element will be removed from.
	 * @param arrayIndex is the index of the element in the arrayList.
	 * @throws ArrayIndexOutOfBoundsException if tried to reach an element with invalid index values.
	 */
	public void remove(int listIndex, int arrayIndex)
	{
		try
		{
			if(listIndex<0 || listIndex>=listSize() || arrayIndex<0 || arrayIndex>=theList.get(listIndex).size())
				throw new ArrayIndexOutOfBoundsException();
			
			theList.get(listIndex).remove(arrayIndex);
			
			if(theList.get(listIndex).size()==0)
				theList.remove(listIndex);
		}
		catch(InvalidIndex exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}	
	}
	
}
