import java.util.Scanner;


public class mainLinkedListTest 
{
	public static void main(String args[])
	{
		try
		{
			//Creating a list that will have fruit names in it
			LinkedList<String> list1=new LinkedList<String>();
			Iterator<String> list1It=(Iterator<String>)list1.iterator();
			
			//Testing add(), size(), next(), hasNext(), get(), element(), offer() functions
			System.out.println("***Adding Apple and Pear to list1***");
			list1.add("Apple");
			list1.offer("Pear");
			//If the additions were successful the size should be 2 and they should be printed out.
			System.out.println("The size of list1 is: "+ list1.size());
			System.out.println("The element in the index 0 is: "+ list1.element());
			System.out.println("The element in the index 1 is: "+ list1.get(1));
			System.out.println();
			
			
			//Testing the clone() function
			System.out.println("***Creating a clone of list1 as list2***");
			LinkedList<String> list2=(LinkedList<String>) list1.clone();
			Iterator<String> list2It=(Iterator<String>) list2.iterator();
			System.out.println("***Printing list2***\nIt should be the same as list1");
			while(list2It.hasNext())
				System.out.println(list2It.next());
			
			
			//testing containsAll() function
			if(list1.containsAll(list2)==true)
				System.out.println("clone() function was successful, the lists are the same.");
			else
				System.out.println("clone() function was not successful, the lists are not the same.");
			System.out.println();
			
			
			//Testing add(), remove(), contains(), containsAll() functions
			System.out.println("***Adding Banana and removing Pear to&from list2***");
			list2.add("Banana");
			list2.remove("Pear");
			list2It=(Iterator<String>) list2.iterator();
			System.out.println("***Printing list2***\nBanana should have been added and Pear should have been removed");
			while(list2It.hasNext())
				System.out.println(list2It.next());
			if(list2.contains("Pear")==true)
				System.out.println("remove() function was not successful.");
			else
				System.out.println("remove() function was successful. list2 does not include Pear anymore");
			if(list1.containsAll(list2)==false)
				System.out.println("The lists are not the same anymore.");
			System.out.println();
			
			
			//Testing addAll()
			System.out.println("***Adding all elements from list1 to list2.***");
			list2.addAll(list1);
			list2It=(Iterator<String>) list2.iterator();
			System.out.println("***Printing list2***\nIt should again have Pear");
			while(list2It.hasNext())
				System.out.println(list2It.next());
			System.out.println();
			
			
			//Testing removeAll()
			list1It=(Iterator<String>) list1.iterator();
			System.out.println("***Printing list1, These elements will be deleted from list2.***");
			while(list1It.hasNext())
				System.out.println(list1It.next());
			System.out.println("***Removing all elements of list1 from list2***");
			list2.removeAll(list1);
			list2It=(Iterator<String>) list2.iterator();
			System.out.println("***Printing list2***");
			while(list2It.hasNext())
				System.out.println(list2It.next());
			System.out.println();
			System.out.println();
		
			
			//Creating a set that will have numbers in it
			LinkedList<Integer> list3=new LinkedList<Integer>();
			Iterator<Integer> list3It=(Iterator<Integer>)list3.iterator();
			
			
			//Testing add(), add(index, element), clone(), remove(), next(), hasNext() functions
			list3.add(1);
			list3.add(3);
			list3.add(11);
			list3.add(3, 14);
			list3.add(1, 16);
			System.out.println("***Creating a clone of list3 as list4 and removing 11 and 1 from it***");
			LinkedList<Integer> list4=(LinkedList<Integer>) list3.clone();
			list4.remove((Integer)11);
			list4.remove((Integer)1);
			Iterator<Integer> list4It=(Iterator<Integer>)list4.iterator();
			System.out.println("***Printing list3***");
			while(list3It.hasNext())
				System.out.println(list3It.next());
			System.out.println();
			System.out.println("***Printing list4*** \nIt should be the clone of list3 but should not have 1 and 11 after remove()");
			while(list4It.hasNext())
				System.out.println(list4It.next());
			System.out.println();
			
			
			//Testing addAll(index, collection), remove(), poll() functions
			list3.addAll(2, list4);
			list3It=(Iterator<Integer>)list3.iterator();
			System.out.println("***Printing list3***\nAt the index 2, list4 must have been added");
			while(list3It.hasNext())
				System.out.println(list3It.next());
			System.out.println();
			System.out.println("***Deleting 16 from list3***");
			list3.remove((Integer)16);
			System.out.println(list3.poll()+" will also be deleted from list3.");
			list3It=(Iterator<Integer>)list3.iterator();
			System.out.println("***Printing list3***\nOnly one 16 and the first element which was 1 should have been removed.");
			while(list3It.hasNext())
				System.out.println(list3It.next());
			System.out.println();
			
			
			//Testing retainAll()
			list3.retainAll(list4);
			list3It=(Iterator<Integer>)list3.iterator();
			System.out.println("***Printing list3*** \nIt should include only the same elements with list4 after retainAll()");
			while(list3It.hasNext())
				System.out.println(list3It.next());
			System.out.println();
			
			
			//Testing removeAll(), isEmpty() functions
			System.out.println("***Removing all the same elements of list4 from list3***");
			list3.removeAll(list4);
			if(list3.isEmpty()==true)
				System.out.println("removeAll() was successful because list3 and list4 had the same elements. list3 must be empty after removeAll()");
			else
				System.out.println("removeAll() was not successful.");	
			list3It=(Iterator<Integer>)list3.iterator();
			System.out.println("***Printing list3***");
			while(list3It.hasNext())
				System.out.println(list3It.next());
			System.out.println();
			list4It=(Iterator<Integer>)list4.iterator();
			System.out.println("***Printing list4***");
			while(list4It.hasNext())
				System.out.println(list4It.next());
			System.out.println();
			
			
			//Testing retaimAll() function with different types. It should work as long as the array type is the same.
			ArrayList<Integer> alist1=new ArrayList<Integer>();
			System.out.println("***Creating an arrayList with 1,3,2,45***");
			alist1.add(1);
			alist1.add(3);
			alist1.add(2);
			alist1.add(45);
			System.out.println("***Saving only the elements that are the same with alist1***");
			list4.retainAll(alist1);
			list4It=(Iterator<Integer>)list4.iterator();
			System.out.println("***Printing list4***\nIt should have 3 only");
			while(list4It.hasNext())
				System.out.println(list4It.next());
			System.out.println();
			
			//Testing clear() function
			System.out.println("***Clearing list4***");
			list4.clear();
			if(list4.isEmpty()==true)
				System.out.println("clear() was successful. list4 must be empty after removeAll()");
			else
				System.out.println("clear() was not successful.");	
			System.out.println();
			
			
			
			//Testing for the exceptions
			//poll() and offer() functions can be tested the same way
			//This should throw an exception because list4 was cleared.
			System.out.println("poll function will be tried to used on empty list4");
			list4.poll();
			
		}
		catch(NoNextElement exc)
		{
			System.err.println(exc);
			System.exit(-1);
		}
		catch(InvalidIndex exc)
		{
			System.err.println(exc);
			System.exit(-1);
		}
		catch(NoElement exc)
		{
			System.err.println(exc);
			System.exit(-1);
		}
	}
}
