
public class HelperMethods<E>
{
	/**
	 * Finds how many times an element exists in an array
	 * @param array is the array that will be searched in
	 * @param number is the element that will be searched
	 * @return the number of times an element exists in an array
	 */
	public int findOccurences(E[] array, E number)
	{
		int count=0;
		int start=findFirstIndex(array, number);
		
		if(start==-1)
			return 0;
		for(int i=start; i<array.length; ++i)
		{
			if(i==array.length-1 && array[i].equals(array[i-1]))
				count++;
			else if(i==array.length-1 && !array[i].equals(array[i-1]))
				break;
			else if(array[i].equals(array[i+1]))
				count++;
			else
			{
				count++;
				break;
			}
		}
		
		return count;
	}
	
	/**
	 * Finds the first index an element exists in an array
	 * @param array is the array that will be searched in
	 * @param element is the element that will be searched 
	 * @return the index
	 */
	public int findFirstIndex(E[] array, E element)
	{
		int start=-1;
		for(int i=0; i<array.length; ++i)
			if(array[i].equals(element))
			{
				start=i;
				break;
			}
		
		return start;
	}
	
	/**
	 * Finds the mode element of an array
	 * @param array is the array whose mode will be found
	 * @return the mode element
	 */
	public E findMode(E[] array)
	{
		int occurenceCount=findOccurences(array, array[0]);
		int modeTemp=occurenceCount;
		E modeETemp=array[0];
		int i=0;
		
		while(i<array.length)
		{
			i+=occurenceCount;
			
			if(i>=array.length-1)
				break;
			
			occurenceCount=findOccurences(array, array[i]);
			if(occurenceCount>modeTemp)
			{
				modeTemp=occurenceCount;
				modeETemp=array[i];
			}
		}
		
		return modeETemp;
	}
}
