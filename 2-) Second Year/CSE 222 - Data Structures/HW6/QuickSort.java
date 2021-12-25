import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Comparator;

/**
 * A class that performs quick sort.
 */
public class QuickSort<T extends Comparable<T>> 
{
	Comparator comp;
	
	/**
	 * Sorts the given ArrayList according to the given comparator using the quick sort algorithm
	 * @param array is the given ArrayList
	 * @param pComp is the given comparator
	 */
	public void sort(ArrayList<T> array, Comparator pComp)
	{
		if(array.size()<2)
			return;
		
		comp=pComp;
		
		quickSort(array, 0, array.size()-1);
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
	
	private void quickSort(ArrayList<T> table, int first, int last)
	{
		if(first<last)
		{
			int pivIndex=partition(table, first, last);
			quickSort(table, first, pivIndex-1);
			quickSort(table, pivIndex+1, last);
		}
	}
	
	private int partition(ArrayList<T> table, int start, int end)
	{
		T pivot=table.get(start);
		int up=start;
		int down=end;
		
		do
		{
			while(up<down && compare(table.get(up), pivot)<=0)
				up++;
			while(compare(table.get(down), pivot)>0)
				down--;
			
			if(up<down)
				swap(table, up, down);
		}
		while(up<down);
		
		swap(table, start, down);
		
		return down;
	}
}
