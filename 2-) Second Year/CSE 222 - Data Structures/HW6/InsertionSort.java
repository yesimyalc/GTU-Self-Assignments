import java.util.ArrayList;

/**
 * A class that performs insertion sort.
 */
public class InsertionSort<T extends Comparable<T>>
{
	/**
	 * Sorts the given ArrayList using insertion sort algorithm
	 * @param array is the given ArrayList
	 */
	public void sort(ArrayList<T> array)
	{
		if(array.size()<2)
			return;
		
		for(int nextPos=1; nextPos<array.size(); ++nextPos)
		{
			T temp=array.get(nextPos);
			while(nextPos>0 && array.get(nextPos-1).compareTo(temp)>0)
			{
				array.set(nextPos, array.get(nextPos-1));	
				nextPos--;
			}
			array.set(nextPos, temp);
		}
	}
}
