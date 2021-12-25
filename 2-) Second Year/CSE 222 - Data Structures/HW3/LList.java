import java.util.ListIterator;
import java.util.NoSuchElementException;

/**
 * A container class that behaves like a LinkedList.
 * @param <E> will be the type of the objects that will be contained inside this class.
 */
public class LList<E>
{
	/**
	 *A helper node class that will be used to hold the objects inside the LList class.
	 * @param <E> will be the type of the objects that will be contained inside this class.
	 */
	private static class Node<E>
	{
		E data;
		Node<E> next;
		Node<E> previous;
		
		/**
		 * A constructor that adds the given item to the data field of the class.
		 * There will not be next or previous nodes for this data therefore they will be set to null.
		 * @param item will be the stored data.
		 */
		private Node(E item)
		{
			data=item;
			next=null;
			previous=null;
		}
		
		/**
		 *  A constructor that adds the given item to the data field of the class and sets the next node of it.
		 *  There will not be previous nodes for this data therefore it will be set to null.
		 * @param item will be the stored data.
		 * @param nextItem will be the next node of this node.
		 */
		private Node(E item, Node<E> nextItem)
		{
			data=item;
			next=nextItem;
			previous=null;
			nextItem.previous=this;
		}
		
		/**
		 * A constructor that sets the data, next and previous fields of the node as the given values.
		 * @param item will be the stored data.
		 * @param nextItem will be the next node of this node.
		 * @param pastItem will be the previous node of this node.
		 */
		private Node(E item, Node<E> nextItem, Node<E> pastItem)
		{
			data=item;
			next=nextItem;
			previous=pastItem;
			nextItem.previous=this;
			pastItem.next=this;
		}
	}
	
	private Node<E> head;
	private Node<E> tail;
	private int size=0;
	
	/**
	 * A constructor that creates an empty LList.
	 */
	public LList()
	{
		head=null;
		tail=null;
	}
	
	/**
	 * A constructor that creates a LList starting with the given item.
	 * @param item will be the first element of the LList.
	 */
	public LList(E item)
	{
		Node<E> newNode=new Node(item);
		head=newNode;
		tail=newNode;
		size++;
	}
	
	/**
	 * Makes a clone of the LList.
	 */
	public Object clone()
	{
		LList<E> temp=new LList<E>();
		for(int i=0; i<size; ++i)
			temp.addLast(get(i));
		
		return temp;
	}
	
	/**
	 * @return the size of the LList.
	 */
	public int size() {return size;}
	
	/**
	 * @return the data of the first element(head) of the LList. 
	 */
	public E getFirst() {return head.data;}
	
	/**
	 * @return the data of the last element(tail) of the LList.
	 */
	public E getLast() {return tail.data;}
	
	/**
	 * @param index will be the index of the element that will be searched. It should be bigger than 0, smaller than size.
	 * @return the data of the element with the given index.
	 */
	public E get(int index)
	{
		if(iterator(index).nextItem==null)
			return null;
		
		return iterator(index).nextItem.data;
	}
	
	/**
	 * Adds the given object to the given index of the LList. 
	 * @param index will be the index of the element that will be added. It should be bigger than 0, smaller than size.
	 * @param obj is the element that will be added to the LList.
	 */
	public void add(int index, E obj)
	{
		iterator(index).add(obj);
	}
	
	/**
	 * Adds the given element to the start of the LList.
	 * @param obj is the element that will be added to the LList.
	 */
	public void addFirst(E obj)
	{
		iterator().add(obj);
	}
	
	/**
	 * Adds the given element to the end of the LList.
	 * @param obj is the element that will be added to the LList.
	 */
	public void addLast(E obj)
	{
		iterator(size()).add(obj);
	}
	
	/**
	 * Removes the first appearance of the given object from the list if it exists.
	 * @param obj is the element that will be removed from the list.
	 * @return true if removed, false if not.
	 */
	public boolean remove(E obj)
	{
		ListIter iter=iterator();
		
		while(iter.hasNext())
		{
			if(iter.nextItem.data.equals(obj))
			{
				iter.remove();
				return true;
			}
			else
				iter.next();
		}
		
		return false;
	}
	
	/**
	 * Removes the element with the given index from the LList.
	 * @param index is the index of the element that will be removed. Should be bigger than 0, smaller than size.
	 * @return true if removes.
	 * @throws InvalidIndex if the index is not valid.
	 */
	public boolean remove(int index) throws InvalidIndex
	{
		if(index<0 || index>=size())
			throw new InvalidIndex("There is no elements with this index.");
		
		ListIter iter=iterator(index);
		iter.remove();
		
		return true;
	}
	
	/**
	 * @return an iterator starting from the beginning.
	 */
	public ListIter iterator() {return new ListIter();}
	
	/**
	 * @param i will be the index that will be started from. Should be bigger than 0, smaller than size.
	 * @return an iterator starting from the i'th index.
	 */
	public ListIter iterator(int i) 
	{
		ListIter iter=null;
		try
		{
			iter=new ListIter(i);
		}
		catch(InvalidIndex exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
		return iter;
	}
	
	/**
	 * A helper class to iterate over the elements of LList.
	 */
	private class ListIter implements ListIterator<E>
	{
		private Node<E> nextItem;
		private Node<E> lastReturned;
		private int index;
		
		/**
		 * Constructor that creates an iterator starting from the beginning.
		 */
		public ListIter()
		{
			nextItem=head;
			lastReturned=null;
			index=0;
		}
		
		/**
		 * Constructor that creates an iterator starting from the indexNo'th index.
		 * @param indexNo will be the index that the iterator will start from. Should be bigger than 0,smaller than size.
		 * @throws InvalidIndex if the indexNo is not valid.
		 */
		public ListIter(int indexNo)throws InvalidIndex
		{
			if(indexNo<0 || indexNo>size())
				throw new InvalidIndex("There is no elements with this index no.");
			
			index=indexNo;
			lastReturned=null;
			
			if(index==size())
				nextItem=null;
			else
			{
				nextItem=head;
				for(int i=0; i<index; ++i)
					nextItem=nextItem.next;
			}
		}
		
		/**
		 * Adds the object to the LList.
		 */
		public void add(E obj)
		{
			Node<E> newNode=new Node<E>(obj);
			
			if(head==null)
			{
				head=newNode;
				tail=newNode;
			}
			else if(index==0)
			{
				head.previous=newNode;
				head=newNode;
			}
			else if(index==size())
			{
				tail.next=newNode;
				newNode.previous=tail;
				tail=newNode;
			}
			else
			{
				nextItem.previous.next=newNode;
				newNode.previous=nextItem.previous;
				nextItem.previous=newNode;
			}
			
			newNode.next=nextItem;
			index++;
			size++;
		}
		
		/**
		 * @return true if LList still has element, false if not.
		 */
		public boolean hasNext()
		{
			if(nextItem!=null)
				return true;
			
			return false;
		}
		
		/**
		 * @return true if LList has previous elements, false if not.
		 */
		public boolean hasPrevious()
		{
			return(nextItem==null && size!=0) || nextItem.previous!=null;
		}
		
		/**
		 * @return the next element in the LList.
		 * @throws NoSuchElementException if there in no next
		 */
		public E next()
		{
			if(hasNext()==false)
				throw new NoSuchElementException();
			
			lastReturned=nextItem;
			nextItem=nextItem.next;
			index++;
			
			return lastReturned.data;
		}
		
		/**
		 * @return the index of the element that will be returned next.
		 */
		public int nextIndex() {return index;}
		
		/**
		 * @return the previous element in the LList.
		 * @throws NoSuchElementException if there in no previous.
		 */
		public E previous()
		{
			if(hasPrevious()==false)
				throw new NoSuchElementException();
			
			if(nextItem==null)
				nextItem=tail;
			else
				nextItem=nextItem.previous;
			lastReturned=nextItem;
			index--;
			
			return lastReturned.data;
		}
		
		/**
		 * @return the previous element's index in the LList.
		 */
		public int previousIndex() {return index-1;}
		
		/**
		 * Removes an element from the LList.
		 * @throws NoSuchElementException if the list is empty or the iterator is at the end of the list.
		 */
		public void remove()
		{
			if(index==size())
				throw new NoSuchElementException();
			else if(head==null)
				throw new NoSuchElementException();
			else if(head==tail)
			{
				head=null;
				tail=null;
			}
			else if(index==0)
			{
				head=head.next;
				head.previous=null;
			}
			else if(index==size()-1)
			{
				tail=tail.previous;
				tail.next=null;
			}
			else
			{
				nextItem.previous.next=nextItem.next;
				nextItem.next.previous=nextItem.previous;
			}
			
			nextItem=nextItem.next;
			size--;
		}
		
		public void set(E obj) {nextItem.data=obj;}
	}
}
