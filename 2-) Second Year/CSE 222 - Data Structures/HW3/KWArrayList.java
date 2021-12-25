import java.util.Arrays;

/**
 * A container class that behaves like an ArrayList.
 * @param <E> will be the type of the objects that will be contained inside this class.
 */
public class KWArrayList<E>
{
	private E data[]=null;
	private int size=0;
	private int capacity=0;
	
	/**
	 * A constructor that creates an empty KWArrayList with 10 capacity.
	 */
	public KWArrayList()
	{
		this(10);
	}
	
	/**
	 * A constructor that creates an empty KWArrayList with capVal capacity.
	 * @param capVal will be the capacity of the list. Should be bigger than -1.
	 */
	public KWArrayList(int capVal)
	{
		try
		{
			setCap(capVal);
			data=(E[])new Object[capacity];
		}
		catch(InvalidCapacity exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
	}
	
	/**
	 * Makes a clone of the list.
	 */
	public Object clone()
	{
		KWArrayList<E> temp=new KWArrayList<E>(capacity);
		for(int i=0; i<size; ++i)
			temp.add(get(i));
		
		return temp;
	}
	
	/**
	 * Checks and sets the capacity.
	 * @param newCapVal will be the new capacity.
	 * @throws InvalidCapacity if the capacity is negative.
	 */
	private void setCap(int newCapVal)throws InvalidCapacity
	{
		if(newCapVal>=0)
			capacity=newCapVal;
		else
			throw new InvalidCapacity("There cannot be negative amount of capacity.");
	}
	
	/**
	 * Reallocates memory for the list. Capacity is doubled.
	 */
	private void reallocate()
	{
		capacity*=2;
		data=Arrays.copyOf(data, capacity);
	}
	
	/** 
	 * @param index will be the index of the element that will be returned. Should be bigger than 0, smaller than size.
	 * @return the element with the index from the list.
	 */
	public E get(int index)
	{
		if(index<0 || index>=size)
			throw new ArrayIndexOutOfBoundsException();
		
		return data[index];
	}
	
	/**
	 * @return size of the list.
	 */
	public int size() {return size;}
	
	/**
	 * Changes the element with the given index to the given element.
	 * @param index will be the index of the element that will be changed
	 * @param element will be the new data of the element with the given index
	 * @return the old data
	 * @throws InvalidIndex if the index is not valid.
	 */
	public E set(int index, E element)throws InvalidIndex
	{
		if(index<0 || index>=size)
			throw new InvalidIndex("There is no elements in this index.");
		
		E temp=data[index];
		data[index]=element;
		
		return temp;
	}
	
	/**
	 * Adds an element to the end of the list.
	 * @param element will be added to the list.
	 * @return true if added.
	 */
	public boolean add(E element)
	{
		if(size>=capacity)
			reallocate();
		
		data[size()]=element;
		size++;
		
		return true;
	}
	
	/**
	 * Adds an element to the given index of the list. Shifts elements accordingly.
	 * @param index will be the index of the added element. Should be bigger than 0, smaller than size.
	 * @param element will be added to the list.
	 * @throws InvalidIndex if the index is not valid.
	 */
	public void add(int index, E element)throws InvalidIndex
	{
		if(index<0 || index>size)
			throw new InvalidIndex("There is no elements in this index.");
		
		if(size>=capacity)
			reallocate();
		
		for(int i=size; i>index; --i)
			data[i]=data[i-1];
		
		data[index]=element;
		size++;
	}
	
	/**
	 * @param target is the element that will be searched in the list.
	 * @return the index that the element is found or -1 if not found.
	 */
	public int indexOf(E target)
	{
		for(int i=0; i<size; ++i)
			if(data[i].equals(target))
				return i;
		
		return -1;
	}
	
	/**
	 * Removes the element in the given index.
	 * @param index will be the index of the element that will be removed. Should be bigger than 0, smaller than size.
	 * @return the removed element
	 * @throws InvalidIndex if the index is not valid.
	 */
	public E remove(int index)throws InvalidIndex
	{
		if(index<0 || index>=size)
			throw new InvalidIndex("There is no elements in this index.");
		
		E temp=data[index];
		size--;
		
		for(int i=index; i<size; ++i)
			data[i]=data[i+1];
		
		return temp;
	}
}
