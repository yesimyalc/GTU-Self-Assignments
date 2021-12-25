

public class mainHashSetTest {

	public static void main(String[] args) 
	{
		try
		{
			//Creating a set that will have fruit names in it
			HashSet<String> set1=new HashSet<String>();
			Iterator<String> set1It=(Iterator<String>)set1.iterator();
			
			//Testing add(), size(), next(), hasNext() functions
			System.out.println("***Adding Apple and Pear to set1***");
			set1.add("Apple");
			set1.add("Pear");
			//If the additions were successful the size should be 2 and they should be printed out.
			System.out.println("The size of set1: "+set1.size());
			System.out.println("***Printing set1***");
			while(set1It.hasNext())
				System.out.println(set1It.next());
			System.out.println();
			
			
			//Testing the clone() function
			System.out.println("***Creating a clone of set1 as set2***");
			HashSet<String> set2=(HashSet<String>) set1.clone();
			Iterator<String> set2It=(Iterator<String>) set2.iterator();
			System.out.println("***Printing set2***\nIt should be the same as set1");
			while(set2It.hasNext())
				System.out.println(set2It.next());
			
			
			//testing containsAll() function
			if(set1.containsAll(set2)==true)
				System.out.println("clone() function was successful, the sets are the same.");
			else
				System.out.println("clone() function was not successful, the sets are not the same.");
			System.out.println();
			
			
			//Testing add(), remove(), contains(), containsAll() functions
			System.out.println("***Adding Banana and removing Pear to&from set2***");
			set2.add("Banana");
			set2.remove("Pear");
			set2It=(Iterator<String>) set2.iterator();
			System.out.println("***Printing set2***\nBanana should have been added and Pear should have been removed");
			while(set2It.hasNext())
				System.out.println(set2It.next());
			if(set2.contains("Pear")==true)
				System.out.println("remove() function was not successful.");
			else
				System.out.println("remove() function was successful. set2 does not include Pear anymore");
			if(set1.containsAll(set2)==false)
				System.out.println("The sets are not the same anymore.");
			System.out.println();
			
			//Testing addAll()
			System.out.println("***Adding all elements from set1 to set2. If they have equal elements, they will not be added again.***");
			set2.addAll(set1);
			set2It=(Iterator<String>) set2.iterator();
			System.out.println("***Printing set2***\nIt should again have Pear");
			while(set2It.hasNext())
				System.out.println(set2It.next());
			System.out.println();
			
			
			//Testing removeAll()
			set1It=(Iterator<String>) set1.iterator();
			System.out.println("***Printing set1, These elements will be deleted from set2.***");
			while(set1It.hasNext())
				System.out.println(set1It.next());
			System.out.println("***Removing all elements of set1 from set2***");
			set2.removeAll(set1);
			set2It=(Iterator<String>) set2.iterator();
			System.out.println("***Printing set2***");
			while(set2It.hasNext())
				System.out.println(set2It.next());
			System.out.println();
			System.out.println();
			
			//Creating a set that will have numbers in it
			HashSet<Integer> set3=new HashSet<Integer>();
			Iterator<Integer> set3It=(Iterator<Integer>)set3.iterator();
			
			//Testing add(), clone(), remove(), next(), hasNext() functions
			set3.add(1);
			set3.add(3);
			set3.add(11);
			set3.add(14);
			System.out.println("***Creating a clone of set3 as set4 and removing 11 and 1 from it***");
			HashSet<Integer> set4=(HashSet<Integer>) set3.clone();
			set4.remove(11);
			set4.remove(1);
			Iterator<Integer> set4It=(Iterator<Integer>)set4.iterator();
			System.out.println("***Printing set3***");
			while(set3It.hasNext())
				System.out.println(set3It.next());
			System.out.println();
			System.out.println("***Printing set4*** \nIt should be the clone of set3 but should not have 1 and 11 after remove()");
			while(set4It.hasNext())
				System.out.println(set4It.next());
			System.out.println();
			
			
			//Testing retainAll()
			set3.retainAll(set4);
			set3It=(Iterator<Integer>)set3.iterator();
			System.out.println("***Printing set3*** \nIt should include only the same elements with set4 after retainAll()");
			while(set3It.hasNext())
				System.out.println(set3It.next());
			System.out.println();
			
			
			//Testing removeAll(), isEmpty() functions
			System.out.println("***Removing all the same elements of set4 from set3***");
			set3.removeAll(set4);
			if(set3.isEmpty()==true)
				System.out.println("removeAll() was successful because set3 and set4 had the same elements. Set3 must be empty after removeAll()");
			else
				System.out.println("removeAll() was not successful.");	
			set3It=(Iterator<Integer>)set3.iterator();
			System.out.println("***Printing set3***");
			while(set3It.hasNext())
				System.out.println(set3It.next());
			System.out.println();
			set4It=(Iterator<Integer>)set4.iterator();
			System.out.println("***Printing set4***");
			while(set4It.hasNext())
				System.out.println(set4It.next());
			System.out.println();
			
			
			//Testing iterator's remove() function
			System.out.println("***Using the iterator's remove function***");
			set4It=(Iterator<Integer>)set4.iterator();
			set4It.next();
			set4It.remove();
			set4It=(Iterator<Integer>)set4.iterator();
			System.out.println("***Printing set4*** \nThe first element must be deleted after remove()");
			while(set4It.hasNext())
				System.out.println(set4It.next());
			System.out.println();
			
			
			//Testing clear() function
			System.out.println("***Clearing set4***");
			set4.clear();
			if(set4.isEmpty()==true)
				System.out.println("clear() was successful. Set4 must be empty after removeAll()");
			else
				System.out.println("clear() was not successful.");	
			System.out.println();
			
			
			//Testing containsAll() function with different types. It should work as long as the array type is the same.
			ArrayList<String> list1=new ArrayList<String>();
			System.out.println("Creating an arrayList with the same elements as set2");
			list1.add("Banana");
			list1.add("Banana");
			if(list1.containsAll(set2))
				System.out.println("list1 contains all the elements of set2");
			else
				System.out.println("list1 does not contain all the elements of set2");
			System.out.println();
			
			
			//Testing addAll(index, collection) function with different types. It should work as long as the array type is the same.
			set1It=(Iterator<String>) set1.iterator();
			System.out.println("***Printing set1***\nBefore adding");
			while(set1It.hasNext())
				System.out.println(set1It.next());
			System.out.println();
			System.out.println("***Adding list1 to set1***");
			set1.addAll(list1);
			set1It=(Iterator<String>) set1.iterator();
			System.out.println("***Printing set1***\nAfter adding the set should have one Banana");
			while(set1It.hasNext())
				System.out.println(set1It.next());
			
		}
		catch(NoNextElement exc)
		{
			System.err.println("Error, There is no next element.");
			System.exit(-1);
		}
		catch(InvalidType exc)
		{
			System.err.println("Error, cannot use remove() function for queue types");
			System.exit(-1);
		}

	}

}
