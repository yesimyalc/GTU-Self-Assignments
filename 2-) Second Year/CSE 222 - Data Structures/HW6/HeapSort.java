import java.util.ArrayList;
import java.util.Comparator;

/**
 * A class that performs heap sort.
 */
public class HeapSort<T extends Comparable<T>>
{
	Comparator comp;
	
	/**
	 * Sorts the given ArrayList according to the given comparator using the heap sort algorithm
	 * @param array is the given ArrayList
	 * @param pComp is the given comparator
	 */
	public void sort(ArrayList<T> array, Comparator pComp)
	{
		if(array.size()<2)
			return;
		
		comp=pComp;
		
		buildHeap(array);
		shrinkHeap(array);
	}
	
	/**
	 * Compares the given elements.
	 * If the comparator is null uses the compareTo methods of the elements.
	 * If not null, uses comparator to compare
	 * @return an integer greater than 0 if left>right, smaller than 0 if left<right, o if left=right.
	 */
	private int compare(T left, T right)
	{
		if(comp==null)
			return left.compareTo(right);
		else
			return comp.compare(left,right);
	}
	
	/**
	 * Swaps the elements inside the given entries of the given arraylist
	 */
	private void swap(ArrayList<T> table, int pIndex, int cIndex)
	{
		T temp=table.get(pIndex);
		table.set(pIndex, table.get(cIndex));
		table.set(cIndex, temp);
	}
	
	private void buildHeap(ArrayList<T> table)
	{
		int n=1;
		while(n<table.size())
		{
			n++;
			int child=n-1;
			int parent=(child-1)/2;
			while(parent>=0 && compare(table.get(parent), table.get(child))<0)
			{
				swap(table, parent, child);
				child=parent;
				parent=(child-1)/2;
			}
		}
	}
	
	private void shrinkHeap(ArrayList<T> table)
	{
		int n=table.size();
		while(n>0)
		{
			n--;
			int parent=0;
			swap(table, 0, n);
			
			while(true)
			{
				int leftC=(parent*2)+1;
				int rightC=leftC+1;
				
				if(leftC>=n)
					break;
				int maxC=leftC;
				if(rightC<n && compare(table.get(leftC), table.get(rightC))<0)
					maxC=rightC;
				
				if(compare(table.get(parent), table.get(maxC))<0)
				{
					swap(table, parent, maxC);
					parent=maxC;
				}
				else
					break;
			}
		}
	}
}
