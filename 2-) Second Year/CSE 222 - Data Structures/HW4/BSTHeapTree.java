
public class BSTHeapTree<E>
{
	private BinarySearchTree<MaxHeap<E>> theData;
	
	public BSTHeapTree()
	{
		theData=new BinarySearchTree<MaxHeap<E>>();
	}
	
	/**
	 * Adds the given element to a proper position in the tree
	 * If the element already exists in the tree, increments its count
	 * If it does not exists, adds it to the first proper non full node
	 * @param newRoot is the node that will be started to search for a suitable node to add the element
	 * @param element is the element that will be added
	 * @param count is the amount of the item
	 * @return the amount of the element after addition
	 */
	private int add(BinarySearchTree<MaxHeap<E>> newRoot, E element, int count)
	{
		if(newRoot==null)
		{
			MaxHeap<E> newHeap=new MaxHeap<E>(element);
			add(newHeap);
			
			return 1;
		}
		else if(!newRoot.getRoot().isFull() || newRoot.getRoot().contains(element))
		{
			newRoot.getRoot().offer(element);
			int amount=newRoot.getRoot().getCount(newRoot.getRoot().search(element));
			return amount;
		}
		else if(((Comparable<E>)element).compareTo(newRoot.getRoot().peek())<0)
			count=add(newRoot.getLeftSubtree(), element, count);
		else
			count=add(newRoot.getRightSubtree(), element, count);
		
		return count;
	}
	
	/**
	 * Searches for an element in the tree
	 * @param newRoot is the node that will be started to search from
	 * @param element is the element that will be added
	 * @param count is the amount of the item
	 * @return how many times the element exists in the tree
	 */
	private int find(BinarySearchTree<MaxHeap<E>> newRoot, E element, int count)
	{
		if(newRoot==null)
			return count+0;
		
		int indexNo=newRoot.getRoot().search(element);
		if(indexNo!=-1)
			return count+newRoot.getRoot().getCount(indexNo);
		else if(indexNo==-1 && ((Comparable<E>)element).compareTo(newRoot.getRoot().peek())<0)
			count=find(newRoot.getLeftSubtree(), element, count);
		else
			count=find(newRoot.getRightSubtree(), element, count);
		
		return count;
	}
	
	/**
	 * Removes an element from the tree.
	 * If the element exists only once, removes it's node. If it exists more than once only decrements its count.
	 * @param newRoot is the node that will be started to search the element
	 * @param element is the element that will be removed
	 * @param count is the amount of the item
	 * @return the amount of the element after removal
	 */
	private int remove(BinarySearchTree<MaxHeap<E>> newRoot, E element, int count)
	{
		if(newRoot==null)
			return 0;
		
		int indexNo=newRoot.getRoot().search(element);
		if(indexNo!=-1)
		{
			int amount=newRoot.getRoot().getCount(indexNo);
			
			if(amount==1 && newRoot.getRoot().size()==1)
			{
				remove(newRoot.getRoot());
				return 0;
			}
			
			MaxHeapIterator iter=newRoot.getRoot().iterator(indexNo);
			E temp=(E)iter.next();
			iter.remove();
			
			return amount-1;
		}
		else if(indexNo==-1 && ((Comparable<E>)element).compareTo(newRoot.getRoot().peek())<0)
			count=remove(newRoot.getLeftSubtree(), element, count);
		else
			count=remove(newRoot.getRightSubtree(), element, count);
		
		return count;
	}
	
	/**
	 * A recursive method to check if the tree has a proper format. 
	 * Proper format meaning non leaf nodes are full. 
	 * If the format is wrong, removes elements from leaf nodes and adds to non full non leaf nodes.
	 * @param newRoot is the node that will be started to check the order
	 */
	private void fixTree(BinarySearchTree<MaxHeap<E>> newRoot)
	{
		if(newRoot==null || newRoot.isLeaf())
			return;
		while(!newRoot.getRoot().isFull())
		{
			MaxHeap<E> chosenLeaf=findLeaf(newRoot);
			E temp;
			int count;
			if(chosenLeaf.size()==1 && chosenLeaf.getCount(0)==1)
			{
				temp=chosenLeaf.peek();
				remove(chosenLeaf);
				count=1;
			}
			else
			{
				count=chosenLeaf.getCount(0);
				temp=chosenLeaf.poll();
				for(int i=0; i<count-1; ++i)
					chosenLeaf.poll();
			}
			
			for(int i=0; i<count; ++i)
				newRoot.getRoot().offer(temp);
		}
		
		fixTree(newRoot.getLeftSubtree());
		fixTree(newRoot.getRightSubtree());
	}
	
	/**
	 * Finds a leaf node of a tree starting to search from the given newRoot
	 * @param newRoot is the node that will be started to search from
	 * @return the found leaf node
	 */
	private MaxHeap<E> findLeaf(BinarySearchTree<MaxHeap<E>> newRoot)
	{
		if(newRoot.isLeaf())
			return newRoot.getRoot();
		
		MaxHeap<E> leafHeap=null;
		if(newRoot.getLeftSubtree()!=null)
			leafHeap=findLeaf(newRoot.getLeftSubtree());
		else
			leafHeap=findLeaf(newRoot.getRightSubtree());

		return leafHeap;
	}
	
	/**
	 * Searches for the element that has the mode in the tree. If more than one elements have the mode, the last found one is returned
	 * @param newRoot is the node that will be started to search from 
	 * @param tempModeE is the current element that has the mode element
	 * @param tempMode is the current mode value
	 * @return the new found element that has the mode
	 */
	private E find_mode(BinarySearchTree<MaxHeap<E>> newRoot, E tempModeE, Integer tempMode)
	{
		if(newRoot==null)
			return tempModeE;
		
		if(newRoot.getRoot().getMode()>tempMode)
		{
			tempMode=newRoot.getRoot().getMode();
			tempModeE=newRoot.getRoot().getModeElement();
		}
		
		tempModeE=find_mode(newRoot.getLeftSubtree(), tempModeE, tempMode);
		tempMode=find(tempModeE);
		tempModeE=find_mode(newRoot.getRightSubtree(), tempModeE, tempMode);
		tempMode=find(tempModeE);
		
		return tempModeE;
	}
	
	/**
	 * Prints out all the elements in the tree in an array form using preorder format. 
	 * @param newRoot is the node that will be started to print from
	 */
	private void printArray(BinarySearchTree<MaxHeap<E>> newRoot)
	{
		if(newRoot==null)
			return;
		
		newRoot.getRoot().printArray();
		System.out.println();
		
		printArray(newRoot.getLeftSubtree());
		printArray(newRoot.getRightSubtree());
	}
	
	/**
	 * @return true if the tree is empty, false if not
	 */
	public boolean isEmpty()
	{
		if(theData==null || theData.isEmpty())
			return true;
		
		return false;
	}
	
	/**
	 * Converts the tree into a string using preorder format.
	 */
	public String toString()
	{
		return theData.toString();
	}
	
	/**
	 * Starting method for the recursive printArray method. 
	 * Prints the tree in an array format
	 */
	public void printArray()
	{
		if(isEmpty())
			return;
		
		printArray(theData);
	}
	
	/**
	 * Starting method for the recursive add method.
	 * Adds the given element into the tree
	 * @param element is the element that will be added
	 * @return is the new amount of the element after addition
	 */
	public int add(E element)
	{
		if(isEmpty())
		{
			MaxHeap<E> newHeap=new MaxHeap<E>(element);
			add(newHeap);
			
			return 1;
		}
		
		return add(theData, element, 0);	
	}
	
	/**
	 * Adds a new MaxHeap to the tree
	 * @param element
	 * @return
	 */
	public boolean add(MaxHeap<E> element)
	{
		return theData.add(element);
	}
	
	/**
	 * Starting method for the recursive remove method
	 * Removes the given element from the tree.
	 * @param element is the element that will be removed
	 * @return the new amount of the element after the removal
	 */
	public int remove(E element)
	{
		if(isEmpty())
			return 0;
		
		int result=remove(theData, element, 0);
		fixTree(theData);

		return result;
	}
	
	/**
	 * Removes a MaxHeap from the tree
	 * @param element is the MaxHeap that will be removed
	 * @return the removed MaxHeap
	 */
	public MaxHeap<E> remove(MaxHeap<E> element)
	{
		return theData.delete(element);
	}
	
	/**
	 * Starting method for the recursive find method.
	 * Searches for an element in the tree
	 * @param element is the element that will be searched
	 * @return how many times the element exists in the tree
	 */
	public int find(E element)
	{
		if(isEmpty())
			return 0;
		
		return find(theData, element, 0);
	}
	
	/**
	 * Starting method for the recursive find_mode method
	 * Searches for the mode element of the tree
	 * @return the found mode element
	 */
	public E find_mode()
	{
		if(isEmpty())
			return null;
		else if(theData.getLeftSubtree()==null && theData.getRightSubtree()==null)
			return theData.getRoot().getModeElement();
		
		E modeElement=theData.getRoot().getModeElement();
		Integer mode=new Integer(theData.getRoot().getMode());
		modeElement=find_mode(theData, modeElement, mode);
		
		return modeElement;
	}
}
