
import java.util.Scanner;

public class mainArrayListTest {

	public static void main(String[] args) 
	{
		try
		{
			//Creating a list that will have fruit names in it
			ArrayList<String> list1=new ArrayList<String>();
			Iterator<String> list1It=(Iterator<String>)list1.iterator();
			
			//Testing add(), size(), next(), hasNext(), get() functions
			System.out.println("***Adding Apple and Pear to list1***");
			list1.add("Apple");
			list1.add("Pear");
			//If the additions were successful the size should be 2 and they should be printed out.
			System.out.println("The size of list1 is: "+ list1.size());
			System.out.println("The element in the index 0 is: "+ list1.get(0));
			System.out.println("The element in the index 1 is: "+ list1.get(1));
			System.out.println();
			
			
			//Testing the clone() function
			System.out.println("***Creating a clone of list1 as list2***");
			ArrayList<String> list2=(ArrayList<String>) list1.clone();
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
			ArrayList<Integer> list3=new ArrayList<Integer>();
			Iterator<Integer> list3It=(Iterator<Integer>)list3.iterator();
			
			
			//Testing add(), add(index, element), clone(), remove(), next(), hasNext() functions
			list3.add(1);
			list3.add(3);
			list3.add(11);
			list3.add(3, 14);
			list3.add(1, 16);
			System.out.println("***Creating a clone of list3 as list4 and removing 11 and 1 from it***");
			ArrayList<Integer> list4=(ArrayList<Integer>) list3.clone();
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
			
			
			//Testing addAll(index, collection)
			list3.addAll(2, list4);
			list3It=(Iterator<Integer>)list3.iterator();
			System.out.println("***Printing list3***\nAt the index 2, list4 must have been added");
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
			
			
			//Testing iterator's remove(), indexOf() functions
			System.out.println("***Using the iterator's remove function***");
			list4It=(Iterator<Integer>)list4.iterator();
			Integer element=list4It.next();
			int index=list4.indexOf(element);
			list4It.remove();
			list4It=(Iterator<Integer>)list4.iterator();
			System.out.printf("***Printing list4*** \nThe element in the %d'th index must be deleted after remove()\n", index);
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
			
			
			//Testing addAll(index, collection) function with different types. It should work as long as the array type is the same.
			HashSet<String> set1=new HashSet<String>();
			set1.add("Banana");
			list1It=(Iterator<String>) list1.iterator();
			System.out.println("***Printing list1***\nBefore adding");
			while(list1It.hasNext())
				System.out.println(list1It.next());
			System.out.println();
			System.out.println("***Adding set1 to list1***");
			list1.addAll(1, set1);
			list1It=(Iterator<String>) list1.iterator();
			System.out.println("***Printing list1***\nAfter adding");
			while(list1It.hasNext())
				System.out.println(list1It.next());
			System.out.println();
			
			
			
			//Testing for the exceptions
			//get(), add(index, element), remove(index), iterator's next() functions can be tested for ArrayList
			ArrayList<String> list5=new ArrayList<String>();
			Iterator<String> list5It=(Iterator<String>)list5.iterator();
			list5.add("Flower");
			list5.add("Tree");
			list5.add("Sea");
			list5.add("Lake");
			System.out.println("Enter the index you want to reach from list5. You can enter from 0 to 3:");
			//If you enter more than 3 or less than 0, it will throw and exception.
			Scanner sc= new Scanner(System.in);
			index=sc.nextInt();
			System.out.println(list5.get(index));
			System.out.println();
			
			System.out.println("Enter the index you want to remove from list5. You can enter from 0 to 3:");
			//If you enter more than 3 or less than 0, it will throw and exception.
			index=sc.nextInt();
			list5.remove(index);
			while(list5It.hasNext())
				System.out.println(list5It.next());
			System.out.println();
			list5It=list5.iterator();
			
			System.out.println("Enter the index you want to add to list5. You can enter from 0 to 3:");
			//If you enter more than 3 or less than 0, it will throw and exception.
			index=sc.nextInt();
			list5.add(index, "Added");
			while(list5It.hasNext())
				System.out.println(list5It.next());
			System.out.println();
			list5It=list5.iterator();
			
			System.out.println("Enter how many elements you want to see. You can enter from 0 to 3:");
			//If you enter more than 3 or less than 0, it will throw and exception.
			index=sc.nextInt();
			for(int i=0; i<index; ++i)
				System.out.println(list5It.next());
			
		}
		catch(NoNextElement exc)
		{
			System.err.println(exc);
			System.exit(-1);
		}
		catch(InvalidType exc)
		{
			System.err.println(exc);
			System.exit(-1);
		}
		catch(InvalidIndex exc)
		{
			System.err.println(exc);
			System.exit(-1);
		}

	}

}
