import java.util.ArrayList;
import java.util.Comparator;
import java.util.NoSuchElementException;

/**
 * A class that acts like a max heap but can only store maximum of 7 elements
 */
public class MaxHeap<E> implements Comparable<E>
{
	/**
	 * The elements are stored inside an arraylist in a node form
	 */
	ArrayList<Node<E>> theData;
	Comparator<E> comparator=null;
	
	/**
	 * A node class that stores the elements inside.
	 * The elements are stored inside the data field and their amount is also stored in count field.
	 */
	private static class Node<E>
	{
		private E data;
		private int count;
		
		private Node()
		{
			data=null;
			count=0;
		}
		
		private Node(E element)
		{
			data=element;
			count=1;
		}
		
		private Node(E element, int amount)
		{
			data=element;
			count=amount;
		}	
	}
	
	public MaxHeap()
	{
		theData=new ArrayList<Node<E>>();
	}
	
	public MaxHeap(Comparator<E> comp)
	{
		comparator=comp;
		theData=new ArrayList<Node<E>>();
	}
	
	public MaxHeap(E element)
	{
		theData=new ArrayList<Node<E>>();
		offer(element);
	}
	
	/**
	 * Compares left element with the right element with the comparator if exists or with their compareTo methods
	 * @return an amount bigger than 0 if left>right, smaller than 0 if left<right, 0 if they are equal
	 */
	private int compare(E left, E right)
	{
		if(comparator!=null)
			return comparator.compare(left, right);
		
		return ((Comparable<E>)left).compareTo(right);
	}
	
	/**
	 * Swaps the element inside the i'th index and j'th index of the ArrayList
	 */
	private void swap(int i, int j)
	{
		Node<E> temp=theData.get(i);
		theData.set(i, theData.get(j));
		theData.set(j, temp);
	}
	
	/**
	 * Starts swapping elements with the parents to fix the order of the heap
	 * @param childNo is the index of the element that will be used as the first child that might be swapped
	 * @return true if at least one element is swapped, false if not
	 */
	private boolean swapUp(int childNo)
	{
		boolean returnVal=false;
		
		int parent=(childNo-1)/2;
		while(parent>=0 && compare(theData.get(parent).data , theData.get(childNo).data)<0)
		{
			swap(parent, childNo);
			childNo=parent;
			parent=(childNo-1)/2;
			returnVal=true;
		}
		
		return returnVal;
	}
	
	/**
	 * Starts swapping elements with the children to fix the order of the heap
	 * @param parentNo is the index of the element that will be used as the first parent that might be swapped
	 * @return true if at least one element is swapped, false if not
	 */
	private boolean swapDown(int parentNo)
	{
		boolean returnVal=false;
		
		while(true)
		{
			int leftC=(parentNo*2)+1;
			int rightC=(parentNo*2)+2;
			
			if(leftC>=theData.size())
				break;
			
			int maxC=0;
			if(rightC<theData.size() && compare(theData.get(rightC).data, theData.get(leftC).data)>0)
				maxC=rightC;
			else
				maxC=leftC;
			
			if(compare(theData.get(parentNo).data, theData.get(maxC).data)<0)
			{
				swap(parentNo, maxC);
				parentNo=maxC;
				returnVal=true;
			}
			else
				break;
		}
		
		return returnVal;
	}
	
	/**
	 * Searches for the n'th greatest element
	 * @param greatestNo is n
	 * @return the n'th greatest element's index
	 */
	private int findNthGreatest(int greatestNo)
	{
		MaxHeap<E> temp=new MaxHeap<E>();
		temp.merge(this);
		for(int i=1; i<greatestNo; ++i)
			temp.poll();
		
		return search(temp.peek());
	}
	
	/**
	 * @return the size of the ArrayList
	 */
	public int size()
	{
		return theData.size();
	}
	
	/**
	 * @return the amount of the element with the given index
	 */
	public int getCount(int index)
	{
		if(index>=theData.size())
			throw new IllegalArgumentException();
		
		return theData.get(index).count;
	}
	
	/**
	 * @return the mode of the heap. Mode is the most repeated element's amount
	 */
	public int getMode()
	{
		int tempMode=theData.get(0).count;
		for(int i=1; i<theData.size(); ++i)
			if(theData.get(i).count>tempMode)
				tempMode=theData.get(i).count;
		
		return tempMode;
	}
	
	/**
	 * @return the element which has the mode of the heap. If there are more than one elements, the last found one is returned.
	 */
	public E getModeElement()
	{
		Node<E> tempMode=theData.get(0);
		for(int i=1; i<theData.size(); ++i)
			if(theData.get(i).count>tempMode.count)
				tempMode=theData.get(i);
		
		return tempMode.data;
	}
	
	/**
	 * @return true if heap has no elements, false if it has elements
	 */
	public boolean isEmpty()
	{
		if(theData==null || theData.size()==0)
			return true;
		
		return false;
	}
	
	/**
	 * @return true if the heap has exactly 7 elements, false if it does not
	 */
	public boolean isFull()
	{
		if(theData.size()==7)
			return true;
		
		return false;
	}
	
	/**
	 * @param element is the element that will be searched inside the heap
	 * @return true if the element is found, false if not
	 */
	public boolean contains(E element)
	{
		if(search(element)!=-1)
			return true;
		
		return false;
	}
	
	/**
	 * Prints the heap in an array form
	 */
	public void printArray()
	{
		for(int i=0; i<theData.size(); ++i)
			System.out.println(theData.get(i).data+","+theData.get(i).count);
	}
	
	/**
	 * @return the data of the very first element in the heap
	 */
	public E peek()
	{
		if(isEmpty())
			return null;
		
		return theData.get(0).data;
	}
	
	/**
	 * Merges the other heap with the current heap. If the size becomes 7, doesnt add the remaining elements.
	 * @param other is the heap that will be merged to the current heap
	 */
	public void merge(MaxHeap<E> other)
	{
		for(int i=0; i<other.theData.size(); ++i)
			offer(other.theData.get(i));
	}
	
	/**
	 * Compares another heap with re current heap.
	 * If the current heap's first element's data is bigger, return a value bigger than 0, if not returns
	 * a value smaller than 0, if equal, returns 0.
	 */
	public int compareTo(Object o) 
	{
		return compare(peek(), ((MaxHeap<E>) o).peek());
	}
	
	/**
	 * Returns the first element's data in the heap in a string form to represent the heap
	 */
	public String toString()
	{
		if(isEmpty())
			return String.format(null);
		
		return peek().toString();
	}
	
	/**
	 * Adds a new element to the heap.
	 * If the element already exists, increments its amount.
	 * If the element didnt use to exist, adds it as the last element then fixes the order of the heap.
	 * @param element is the element that will be added.
	 * @return true if adds, false if not 
	 */
	public boolean offer(E element)
	{
		int contains=search(element);
		
		if(contains!=-1)
		{
			theData.get(contains).count++;
			return true;
		}
		
		if(theData.size()==7)
			return false;
		
		theData.add(new Node<E>(element));
		int child=theData.size()-1;
		swapUp(child);
		
		return true;
	}
	
	/**
	 * Adds the given node to the heap. If it already exists, does not add.
	 * @param element is the node that will be added.
	 * @return true if adds, false if not 
	 */
	private boolean offer(Node<E> element)
	{
		int contains=search(element.data);
		
		if(contains!=-1)
		{
			theData.get(contains).count++;
			return true;
		}
		
		if(theData.size()==7)
			return false;
		
		theData.add(new Node<E>(element.data, element.count));
		int child=theData.size()-1;
		swapUp(child);
		
		return true;
	}
	
	/**
	 * Removes the first element from the heap and then fixes the order.
	 * @return the removed element's data
	 */
	public E poll()
	{
		if(isEmpty())
			return null;
		
		if(theData.get(0).count!=1)
		{
			theData.get(0).count--;
			return theData.get(0).data;
		}
		
		Node<E> temp=theData.get(theData.size()-1);
		theData.remove(theData.size()-1);
		if(theData.size()==0)
			return temp.data;
		E temp2=theData.get(0).data;
		theData.set(0, temp);
		
		int parent=0;
		swapDown(parent);
		
		return temp2;
	}
	
	/**
	 * Searches for an element in the heap
	 * @param element is the element that will be searched in the array.
	 * @return the index of the searched element
	 */
	public int search(E element)
	{
		if(isEmpty())
			return -1;
		
		if(compare(theData.get(0).data, element)<0)
			return -1;
		
		for(int i=0; i<theData.size(); ++i)
			if(compare(theData.get(i).data, element)==0)
				return i;
		
		return -1;
	}
	
	/**
	 * Removes the n'th largest element from the heap.
	 * @param largestNo is n 
	 * @return the removed element, null if nothing is removed.
	 */
	public E remove(int largestNo)
	{
		if(theData.size()<largestNo || largestNo<1)
			return null;
		
		int indexNo=findNthGreatest(largestNo);
		
		MaxHeapIterator iter=iterator(indexNo);
		E temp=(E)iter.next();
		iter.remove();
		
		return temp;
	}
	
	/**
	 * @return an iterator starting from the first element
	 */
	public MaxHeapIterator iterator()
	{
		return new MHIterator();
	}
	
	/**
	 * @return an iterator starting from the given index
	 */
	public MaxHeapIterator iterator(int index)
	{
		return new MHIterator(index);
	}
	
	/**
	 * An iterator class designed for max heap
	 */
	private class MHIterator implements MaxHeapIterator<E>
	{
		/**Holds the next item's index*/
		private int nextItem;
		/**Holds the last returned item's index*/
		private int lastReturned;
		/**Holds the current index*/
		private int index;
		
		/**Creates an iterator staring from the first element*/
		public MHIterator()
		{
			index=0;
			
			if(isEmpty())
				nextItem=-1;
			else
				nextItem=index;
			
			lastReturned=-1;

		}
		
		/**Creates an iterator starting from the indexNo'th element*/
		public MHIterator(int indexNo)
		{
			if(indexNo>theData.size())
				throw new IllegalArgumentException();
			
			index=indexNo;
			
			if(index==theData.size())
				nextItem=-1;
			else
				nextItem=index;
			
			lastReturned=-1;
		}

		/**
		 * @return true if there are more elements that can be visited, false if not
		 */
		public boolean hasNext() 
		{
			if(nextItem!=-1)
				return true;
			
			return false;
		}

		/**
		 * @return the next element's data
		 */
		public E next()
		{
			if(hasNext()==false)
				throw new NoSuchElementException();
			
			lastReturned=nextItem;
			index++;
			if(index==theData.size())
				nextItem=-1;
			else
				nextItem=index;
			
			return theData.get(lastReturned).data;
		}

		/**
		 * Sets the lastReturned element's data as the given value and it's amount as the given amount
		 * @return the old value of the element
		 */
		public E setVal(E value, int amount) 
		{
			if(lastReturned==-1)
				return null;
			
			E temp=theData.get(lastReturned).data;
			theData.set(lastReturned, new Node<E>(value, amount));
			if(swapUp(lastReturned))
				return temp;
			else
				swapDown(lastReturned);
			
			return temp;
		}
		
		/**
		 * Removes the lastReturned element from the heap
		 */
		public void remove()
		{
			if(lastReturned==-1)
				return;
			
			if(theData.get(lastReturned).count!=1)
			{
				theData.get(lastReturned).count--;
				return;
			}
			
			if(index==theData.size())
			{
				theData.remove(theData.size()-1);
				return;
			}
				
			Node<E> temp=theData.get(theData.size()-1);
			theData.remove(theData.size()-1);
			setVal(temp.data, temp.count);
		}
	}
}
