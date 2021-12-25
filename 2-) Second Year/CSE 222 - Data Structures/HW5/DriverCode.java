import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Random;

public class DriverCode {

	public static void main(String[] args) 
	{
		System.out.println("***TESTING ChainHashMap WITH SMALL TABLE***");
		System.out.println("Creating an empty ChainHashMap.");
		KWHashMap<Integer, String> chm1a=new ChainHashMap<Integer, String>(10);
		if(chm1a.isEmpty())
			System.out.println("An empty ChainHashMap is created.");
		System.out.println("Adding elements to the map.");
		chm1a.put(3, "Apple");
		chm1a.put(12, "Pear");
		chm1a.put(13, "Cherry");
		chm1a.put(25, "Banana");
		chm1a.put(23, "Melon");
		chm1a.put(51, "Pineaple");
		chm1a.put(42, "Lemon");
		if(!chm1a.isEmpty())
			System.out.println("Elements are added to the map.");
		System.out.println("Printing out the map.\n");
		chm1a.printMap();
		System.out.println("The current size of the map is: "+chm1a.size()+". The capacity is: "+chm1a.getCapacity());
		System.out.println("\nRemoving some elements from the map.");
		chm1a.remove(13);
		chm1a.remove(51);
		chm1a.remove(42);
		System.out.println("Checking if the elements are removed properly. "+"The current size of the map is: "+chm1a.size()+". The capacity is: "+chm1a.getCapacity());
		if(chm1a.get(13)==null && chm1a.get(51)==null && chm1a.get(42)==null && chm1a.get(25)!=null)
			System.out.println("Removal is done properly.");
		System.out.println("Printing out the map.\n");
		chm1a.printMap();
		
		System.out.println("\n***TESTING ChainHashMap WITH SMALL-MEDIUM SIZED TABLE***");
		System.out.println("\nAdding new elements.");
		chm1a.put(23, "Watermelon");	chm1a.put(78, "Cake");			chm1a.put(90, "Strawberry");
		chm1a.put(11, "Strawberry");	chm1a.put(79, "Cookies");		chm1a.put(92, "Strawberry");
		chm1a.put(45, "Ananas");		chm1a.put(80, "Tomatoe");		chm1a.put(91, "Melon");
		chm1a.put(106, "Grapes");		chm1a.put(81, "Vegetable");		chm1a.put(134, "Pancake");
		chm1a.put(37, "Melon");			chm1a.put(82, "Fruit");			chm1a.put(23, "Honey");
		chm1a.put(96, "Melon");			chm1a.put(83, "Berry");			chm1a.put(22, "Honey");	
		chm1a.put(45, "Orange");		chm1a.put(84, "Greyfurt");		chm1a.put(9111, "Honey");	
		chm1a.put(25, "Tangarine");		chm1a.put(85, "Avocado");		chm1a.put(111, "Honey");	
		chm1a.put(67, "Tomato");		chm1a.put(86, "Cheese");		chm1a.put(112, "Honey");	
		chm1a.put(68, "Cucumber");		chm1a.put(87, "Carrot");		chm1a.put(113, "Honey");	
		chm1a.put(69, "Olive");			chm1a.put(77, "Pickles");		chm1a.put(114, "Honey");		
		chm1a.put(70, "Peach");			chm1a.put(88, "Carrot");		chm1a.put(115, "Honey");	
		chm1a.put(71, "Blueberry");		chm1a.put(89, "Cabbage");		chm1a.put(116, "Honey");	
		chm1a.put(211, "Watermelon");	chm1a.put(432, "Cake");			chm1a.put(512, "Strawberry");
		chm1a.put(212, "Strawberry");	chm1a.put(433, "Cookies");		chm1a.put(421, "Strawberry");
		chm1a.put(345, "Ananas");		chm1a.put(434, "Tomatoe");		chm1a.put(511, "Melon");
		chm1a.put(234, "Grapes");		chm1a.put(435, "Vegetable");	chm1a.put(513, "Pancake");
		chm1a.put(345, "Melon");		chm1a.put(436, "Fruit");		chm1a.put(514, "Honey");
		chm1a.put(346, "Melon");		chm1a.put(421, "Berry");		chm1a.put(555, "Honey");	
		chm1a.put(347, "Orange");		chm1a.put(422, "Greyfurt");		chm1a.put(666, "Honey");	
		chm1a.put(348, "Tangarine");	chm1a.put(423, "Avocado");		chm1a.put(321, "Honey");	
		chm1a.put(348, "Tomato");		chm1a.put(424, "Cheese");		chm1a.put(567, "Honey");	
		chm1a.put(349, "Cucumber");		chm1a.put(425, "Carrot");		chm1a.put(789, "Honey");	
		chm1a.put(350, "Olive");		chm1a.put(426, "Pickles");		chm1a.put(906, "Honey");		
		chm1a.put(351, "Peach");		chm1a.put(412, "Carrot");		chm1a.put(368, "Honey");	
		chm1a.put(352, "Blueberry");	chm1a.put(411, "Cabbage");		chm1a.put(369, "Honey");
		System.out.println("The current size of the map is: "+chm1a.size()+". The capacity is: "+chm1a.getCapacity());
		System.out.println("Removing some elements");
		chm1a.remove(89);
		chm1a.remove(69);
		chm1a.remove(352);
		chm1a.remove(211);
		chm1a.remove(25);
		chm1a.remove(167);
		System.out.println("Checking if the elements are removed properly");
		if(chm1a.get(99)==null && chm1a.get(69)==null && chm1a.get(167)==null && chm1a.get(352)==null && chm1a.get(211)==null && chm1a.get(25)==null && chm1a.get(77)!=null)
			System.out.println("Removal is done properly. "+"The current size of the map is: "+chm1a.size()+". The capacity is: "+chm1a.getCapacity());
		
		System.out.println("\n***TESTING ChainHashMap WITH MEDIUM SIZED TABLE***");
		System.out.println("Creating an empty ChainHashMap with size 101");
		KWHashMap<Integer, Integer> chm1b=new ChainHashMap<Integer, Integer>(101);
		ArrayList<Integer>array=new ArrayList<Integer>();
		Random rand=new Random();
		int upperbound=5001;
		System.out.println("Generating 1000 numbers and inserting them into both the array and the map.");
		for(int i=0; i<1000; ++i)
		{
			Integer number=rand.nextInt(upperbound);
			if(!array.contains(number))
				array.add(number);
			chm1b.put(number, number);
		}
		System.out.println("The current size of the map is: "+chm1b.size()+". The capacity is: "+chm1b.getCapacity());
		for(int i=0; i<array.size(); ++i)
			if(chm1b.get(array.get(i))==null)
			{
				System.out.println("The addition wasnt done properly.");
				break;
			}
			else if(i==array.size()-1)
				System.out.println("The addition was done properly.");
		System.out.println("\nGenerating 300 numbers and removing them from the map.");
		for(int i=0; i<300; ++i)
		{
			Integer number=rand.nextInt(upperbound);
			if(chm1b.remove(number)==null)
				i--;
		}
		System.out.println("The new size of the map is: "+chm1b.size()+". The capacity is: "+chm1b.getCapacity());
		System.out.println("\nComparing the array and the map to see if the 300 elements are actually deleted.");
		int nullCount3=0;
		for(int i=0; i<array.size(); ++i)
			if(chm1b.get(array.get(i))==null)
				nullCount3++;
		System.out.println("There are "+nullCount3+" elements that are deleted from the map.");
		
		System.out.println("\n***TESTING ChainHashMap WITH LARGE SIZED TABLE***");
		System.out.println("Creating an empty ChainHashMap with size 101");
		KWHashMap<Integer, Integer> chm1c=new ChainHashMap<Integer, Integer>(101);
		array=new ArrayList<Integer>();
		rand=new Random();
		upperbound=50001;
		System.out.println("Generating 10000 numbers and inserting them into both the array and the map.");
		for(int i=0; i<10000; ++i)
		{
			Integer number=rand.nextInt(upperbound);
			if(!array.contains(number))
				array.add(number);
			chm1c.put(number, number);
		}
		System.out.println("The current size of the map is: "+chm1c.size()+". The capacity is: "+chm1c.getCapacity());
		for(int i=0; i<array.size(); ++i)
			if(chm1c.get(array.get(i))==null)
			{
				System.out.println("The addition wasnt done properly.");
				break;
			}
			else if(i==array.size()-1)
				System.out.println("The addition was done properly.");
		System.out.println("\nGenerating 3000 numbers and removing them from the map.");
		for(int i=0; i<3000; ++i)
		{
			Integer number=rand.nextInt(upperbound);
			if(chm1c.remove(number)==null)
				i--;
		}
		System.out.println("The new size of the map is: "+chm1c.size()+". The capacity is: "+chm1c.getCapacity());
		System.out.println("\nComparing the array and the map to see if the 3000 elements are actually deleted.");
		nullCount3=0;
		for(int i=0; i<array.size(); ++i)
			if(chm1c.get(array.get(i))==null)
				nullCount3++;
		System.out.println("There are "+nullCount3+" elements that are deleted from the map.");
		
		System.out.println("\n***TESTING ChainHashMap2 WITH SMALL TABLE***");
		System.out.println("Creating an empty ChainHashMap2.");
		KWHashMap<Integer, String> chm2a=new ChainHashMap2<Integer, String>(10);
		if(chm2a.isEmpty())
			System.out.println("An empty ChainHashMap2 is created.");
		System.out.println("Adding elements to the map.");
		chm2a.put(3, "Apple");
		chm2a.put(12, "Pear");
		chm2a.put(13, "Cherry");
		chm2a.put(25, "Banana");
		chm2a.put(23, "Melon");
		chm2a.put(51, "Pineaple");
		chm2a.put(42, "Lemon");
		if(!chm2a.isEmpty())
			System.out.println("Elements are added to the map.");
		System.out.println("Printing out the map.\n");
		chm2a.printMap();
		System.out.println("The current size of the map is: "+chm2a.size()+". The capacity is: "+chm2a.getCapacity());
		System.out.println("\nRemoving some elements from the map.");
		chm2a.remove(13);
		chm2a.remove(51);
		chm2a.remove(42);
		System.out.println("Checking if the elements are removed properly. "+"The current size of the map is: "+chm2a.size()+". The capacity is: "+chm2a.getCapacity());
		if(chm2a.get(13)==null && chm2a.get(51)==null && chm2a.get(42)==null && chm2a.get(25)!=null)
			System.out.println("Removal is done properly.");
		System.out.println("Printing out the map.\n");
		chm2a.printMap();
		
		System.out.println("\n***TESTING ChainHashMap2 WITH SMALL-MEDIUM SIZED TABLE***");
		System.out.println("\nAdding new elements.");
		chm2a.put(23, "Watermelon");	chm2a.put(78, "Cake");			chm2a.put(90, "Strawberry");
		chm2a.put(11, "Strawberry");	chm2a.put(79, "Cookies");		chm2a.put(92, "Strawberry");
		chm2a.put(45, "Ananas");		chm2a.put(80, "Tomatoe");		chm2a.put(91, "Melon");
		chm2a.put(106, "Grapes");		chm2a.put(81, "Vegetable");		chm2a.put(134, "Pancake");
		chm2a.put(37, "Melon");			chm2a.put(82, "Fruit");			chm2a.put(23, "Honey");
		chm2a.put(96, "Melon");			chm2a.put(83, "Berry");			chm2a.put(22, "Honey");	
		chm2a.put(45, "Orange");		chm2a.put(84, "Greyfurt");		chm2a.put(9111, "Honey");	
		chm2a.put(25, "Tangarine");		chm2a.put(85, "Avocado");		chm2a.put(111, "Honey");	
		chm2a.put(67, "Tomato");		chm2a.put(86, "Cheese");		chm2a.put(112, "Honey");	
		chm2a.put(68, "Cucumber");		chm2a.put(87, "Carrot");		chm2a.put(113, "Honey");	
		chm2a.put(69, "Olive");			chm2a.put(77, "Pickles");		chm2a.put(114, "Honey");		
		chm2a.put(70, "Peach");			chm2a.put(88, "Carrot");		chm2a.put(115, "Honey");	
		chm2a.put(71, "Blueberry");		chm2a.put(89, "Cabbage");		chm2a.put(116, "Honey");	
		chm2a.put(211, "Watermelon");	chm2a.put(432, "Cake");			chm2a.put(512, "Strawberry");
		chm2a.put(212, "Strawberry");	chm2a.put(433, "Cookies");		chm2a.put(421, "Strawberry");
		chm2a.put(345, "Ananas");		chm2a.put(434, "Tomatoe");		chm2a.put(511, "Melon");
		chm2a.put(234, "Grapes");		chm2a.put(435, "Vegetable");	chm2a.put(513, "Pancake");
		chm2a.put(345, "Melon");		chm2a.put(436, "Fruit");		chm2a.put(514, "Honey");
		chm2a.put(346, "Melon");		chm2a.put(421, "Berry");		chm2a.put(555, "Honey");	
		chm2a.put(347, "Orange");		chm2a.put(422, "Greyfurt");		chm2a.put(666, "Honey");	
		chm2a.put(348, "Tangarine");	chm2a.put(423, "Avocado");		chm2a.put(321, "Honey");	
		chm2a.put(348, "Tomato");		chm2a.put(424, "Cheese");		chm2a.put(567, "Honey");	
		chm2a.put(349, "Cucumber");		chm2a.put(425, "Carrot");		chm2a.put(789, "Honey");	
		chm2a.put(350, "Olive");		chm2a.put(426, "Pickles");		chm2a.put(906, "Honey");		
		chm2a.put(351, "Peach");		chm2a.put(412, "Carrot");		chm2a.put(368, "Honey");	
		chm2a.put(352, "Blueberry");	chm2a.put(411, "Cabbage");		chm2a.put(369, "Honey");
		System.out.println("The current size of the map is: "+chm2a.size()+". The capacity is: "+chm2a.getCapacity());
		System.out.println("Removing some elements");
		chm2a.remove(89);
		chm2a.remove(69);
		chm2a.remove(352);
		chm2a.remove(211);
		chm2a.remove(25);
		chm2a.remove(167);
		System.out.println("Checking if the elements are removed properly");
		if(chm2a.get(99)==null && chm2a.get(69)==null && chm2a.get(167)==null && chm2a.get(352)==null && chm2a.get(211)==null && chm2a.get(25)==null && chm2a.get(77)!=null)
			System.out.println("Removal is done properly. "+"The current size of the map is: "+chm2a.size()+". The capacity is: "+chm2a.getCapacity());
		
		System.out.println("\n***TESTING ChainHashMap2 WITH MEDIUM SIZED TABLE***");
		System.out.println("Creating an empty ChainHashMap2 with size 101");
		KWHashMap<Integer, Integer> chm2b=new ChainHashMap2<Integer, Integer>(101);
		ArrayList<Integer>array2=new ArrayList<Integer>();
		Random rand2=new Random();
		int upperbound2=5001;
		System.out.println("Generating 1000 numbers and inserting them into both the array and the map.");
		for(int i=0; i<1000; ++i)
		{
			Integer number=rand2.nextInt(upperbound2);
			if(!array2.contains(number))
				array2.add(number);
			chm2b.put(number, number);
		}
		System.out.println("The current size of the map is: "+chm2b.size()+". The capacity is: "+chm2b.getCapacity());
		for(int i=0; i<array2.size(); ++i)
			if(chm2b.get(array2.get(i))==null)
			{
				System.out.println("The addition wasnt done properly.");
				break;
			}
			else if(i==array2.size()-1)
				System.out.println("The addition was done properly.");
		System.out.println("\nGenerating 300 numbers and removing them from the map.");
		for(int i=0; i<300; ++i)
		{
			Integer number=rand2.nextInt(upperbound2);
			if(chm2b.remove(number)==null)
				i--;
		}
		System.out.println("The new size of the map is: "+chm2b.size()+". The capacity is: "+chm2b.getCapacity());
		System.out.println("\nComparing the array and the map to see if the 300 elements are actually deleted.");
		int nullCount=0;
		for(int i=0; i<array2.size(); ++i)
			if(chm2b.get(array2.get(i))==null)
				nullCount++;
		System.out.println("There are "+nullCount+" elements that are deleted from the map.");
		
		System.out.println("\n***TESTING ChainHashMap2 WITH LARGE SIZED TABLE***");
		System.out.println("Creating an empty ChainHashMap2 with size 101");
		KWHashMap<Integer, Integer> chm2c=new ChainHashMap<Integer, Integer>(101);
		array2=new ArrayList<Integer>();
		rand2=new Random();
		upperbound2=50001;
		System.out.println("Generating 10000 numbers and inserting them into both the array and the map.");
		for(int i=0; i<10000; ++i)
		{
			Integer number=rand2.nextInt(upperbound2);
			if(!array2.contains(number))
				array2.add(number);
			chm2c.put(number, number);
		}
		System.out.println("The current size of the map is: "+chm2c.size()+". The capacity is: "+chm2c.getCapacity());
		for(int i=0; i<array2.size(); ++i)
			if(chm2c.get(array2.get(i))==null)
			{
				System.out.println("The addition wasnt done properly.");
				break;
			}
			else if(i==array2.size()-1)
				System.out.println("The addition was done properly.");
		System.out.println("\nGenerating 3000 numbers and removing them from the map.");
		for(int i=0; i<3000; ++i)
		{
			Integer number=rand2.nextInt(upperbound2);
			if(chm2c.remove(number)==null)
				i--;
		}
		System.out.println("The new size of the map is: "+chm2c.size()+". The capacity is: "+chm2c.getCapacity());
		System.out.println("\nComparing the array and the map to see if the 3000 elements are actually deleted.");
		nullCount=0;
		for(int i=0; i<array2.size(); ++i)
			if(chm2c.get(array2.get(i))==null)
				nullCount++;
		System.out.println("There are "+nullCount+" elements that are deleted from the map.");
		
		System.out.println("\n***TESTING CoalescedHashMap WITH SMALL TABLE***");
		System.out.println("Creating an empty CoalescedHashMap.");
		KWHashMap<Integer, String> chm3a=new CoalescedHashMap<Integer, String>(10, 1);
		if(chm3a.isEmpty())
			System.out.println("An empty CoalescedHashMap is created.");
		System.out.println("Adding elements to the map.");
		chm3a.put(3, "Apple");
		chm3a.put(12, "Pear");
		chm3a.put(13, "Cherry");
		chm3a.put(25, "Banana");
		chm3a.put(23, "Melon");
		chm3a.put(51, "Pineaple");
		chm3a.put(42, "Lemon");
		if(!chm3a.isEmpty())
			System.out.println("Elements are added to the map.");
		System.out.println("Printing out the map.\n");
		chm3a.printMap();
		System.out.println("The current size of the map is: "+chm3a.size()+". The capacity is: "+chm3a.getCapacity());
		System.out.println("\nRemoving some elements from the map.");
		chm3a.remove(13);
		chm3a.remove(51);
		chm3a.remove(42);
		System.out.println("Checking if the elements are removed properly. "+"The current size of the map is: "+chm3a.size()+". The capacity is: "+chm3a.getCapacity());
		if(chm3a.get(13)==null && chm3a.get(51)==null && chm3a.get(42)==null && chm3a.get(25)!=null)
			System.out.println("Removal is done properly.");
		System.out.println("Printing out the map.\n");
		chm3a.printMap();
		
		System.out.println("\n***TESTING CoalescedHashMap WITH SMALL-MEDIUM SIZED TABLE***");
		KWHashMap<Integer, String> chm3c=new CoalescedHashMap<Integer, String>(11);
		System.out.println("\nAdding new elements.");
		chm3c.put(5, "Watermelon");		chm3c.put(54, "Cake");			chm3c.put(86, "Strawberry");
		chm3c.put(11, "Strawberry");	chm3c.put(56, "Cookies");		chm3c.put(91, "Strawberry");
		chm3c.put(13, "Ananas");		chm3c.put(58, "Tomatoe");		chm3c.put(92, "Strawberry");	
		chm3c.put(18, "Grapes");		chm3c.put(59, "Vegetable");		chm3c.put(93, "Strawberry");	
		chm3c.put(23, "Melon");			chm3c.put(63, "Fruit");			chm3c.put(94, "Strawberry");	
		chm3c.put(26, "Melon");			chm3c.put(64, "Berry");			chm3c.put(95, "Strawberry");	
		chm3c.put(28, "Orange");		chm3c.put(67, "Greyfurt");		chm3c.put(96, "Strawberry");			
		chm3c.put(32, "Tangarine");		chm3c.put(71, "Avocado");		chm3c.put(97, "Strawberry");		
		chm3c.put(36, "Tomato");		chm3c.put(78, "Cheese");		chm3c.put(98, "Strawberry");		
		chm3c.put(45, "Cucumber");		chm3c.put(79, "Carrot");		chm3c.put(99, "Strawberry");		
		chm3c.put(48, "Olive");			chm3c.put(81, "Pickles");		chm3c.put(100, "Strawberry");			
		chm3c.put(49, "Peach");			chm3c.put(84, "Carrot");		chm3c.put(101, "Strawberry");			
		chm3c.put(51, "Blueberry");		chm3c.put(85, "Cabbage");		chm3c.put(102, "Strawberry");	
		chm3c.put(211, "Watermelon");	chm3c.put(432, "Cake");			chm3c.put(512, "Strawberry");
		chm3c.put(212, "Strawberry");	chm3c.put(433, "Cookies");		chm3c.put(421, "Strawberry");
		chm3c.put(345, "Ananas");		chm3c.put(434, "Tomatoe");		chm3c.put(511, "Melon");
		chm3c.put(234, "Grapes");		chm3c.put(435, "Vegetable");	chm3c.put(513, "Pancake");
		chm3c.put(345, "Melon");		chm3c.put(436, "Fruit");		chm3c.put(514, "Honey");
		chm3c.put(346, "Melon");		chm3c.put(421, "Berry");		chm3c.put(555, "Honey");	
		chm3c.put(347, "Orange");		chm3c.put(422, "Greyfurt");		chm3c.put(666, "Honey");	
		chm3c.put(348, "Tangarine");	chm3c.put(423, "Avocado");		chm3c.put(321, "Honey");	
		chm3c.put(348, "Tomato");		chm3c.put(424, "Cheese");		chm3c.put(567, "Honey");	
		chm3c.put(349, "Cucumber");		chm3c.put(425, "Carrot");		chm3c.put(789, "Honey");	
		chm3c.put(350, "Olive");		chm3c.put(426, "Pickles");		chm3c.put(906, "Honey");		
		chm3c.put(351, "Peach");		chm3c.put(412, "Carrot");		chm3c.put(368, "Honey");	
		chm3c.put(352, "Blueberry");	chm3c.put(411, "Cabbage");		chm3c.put(369, "Honey");
		System.out.println("The current size of the map is: "+chm3c.size()+". The capacity is: "+chm3c.getCapacity());
		System.out.println("\nRemoving some elements");
		chm3c.remove(23);
		chm3c.remove(79);
		chm3c.remove(48);
		chm3c.remove(369);
		chm3c.remove(98);
		chm3c.remove(167);
		System.out.println("Checking if the elements are removed properly");
		if(chm3c.get(23)==null && chm3c.get(79)==null && chm3c.get(167)==null && chm3c.get(86)!=null && chm3c.get(98)==null && chm3c.get(369)==null)
			System.out.println("Removal is done properly. "+"The current size of the map is: "+chm3c.size()+". The capacity is: "+chm3c.getCapacity());
		
		System.out.println("\n***TESTING CoalescedHashMap WITH MEDIUM SIZED TABLE***");
		System.out.println("Creating an empty CoalescedHashMap with size 101");
		KWHashMap<Integer, Integer> chm3b=new CoalescedHashMap<Integer, Integer>(101);
		ArrayList<Integer>array3=new ArrayList<Integer>();
		Random rand3=new Random();
		int upperbound3=5001;
		System.out.println("Generating 1000 numbers and inserting them into both the array and the map.");
		for(int i=0; i<1000; ++i)
		{
			Integer number=rand3.nextInt(upperbound3);
			if(!array3.contains(number))
				array3.add(number);
			chm3b.put(number, number);
		}
		System.out.println("The current size of the map is: "+chm3b.size()+". The capacity is: "+chm3b.getCapacity());
		for(int i=0; i<array3.size(); ++i)
			if(chm3b.get(array3.get(i))==null)
			{
				System.out.println("The addition wasnt done properly.");
				break;
			}
			else if(i==array3.size()-1)
				System.out.println("The addition was done properly.");
		
		System.out.println("\nGenerating 300 numbers and removing them from the map.");
		for(int i=0; i<300; ++i)
		{
			Integer number=rand3.nextInt(upperbound3);
			if(chm3b.remove(number)==null)
				i--;
		}
		System.out.println("The new size of the map is: "+chm3b.size()+". The capacity is: "+chm3b.getCapacity());
		System.out.println("\nComparing the array and the map to see if the 300 elements are actually deleted.");
		int nullCount2=0;
		for(int i=0; i<array3.size(); ++i)
			if(chm3b.get(array3.get(i))==null)
				nullCount2++;
		
		System.out.println("There are "+nullCount2+" elements that are deleted from the map.");
		
		System.out.println("\n***TESTING CoalescedHashMap WITH LARGE SIZED TABLE***");
		System.out.println("Creating an empty CoalescedHashMap with size 101");
		KWHashMap<Integer, Integer> chm3d=new CoalescedHashMap<Integer, Integer>(101);
		array3=new ArrayList<Integer>();
		rand3=new Random();
		upperbound3=50001;
		System.out.println("Generating 10000 numbers and inserting them into both the array and the map.");
		for(int i=0; i<10000; ++i)
		{
			Integer number=rand3.nextInt(upperbound3);
			if(!array3.contains(number))
				array3.add(number);
			chm3d.put(number, number);
		}
		System.out.println("The current size of the map is: "+chm3d.size()+". The capacity is: "+chm3d.getCapacity());
		for(int i=0; i<array3.size(); ++i)
			if(chm3d.get(array3.get(i))==null)
			{
				System.out.println("The addition wasnt done properly.");
				break;
			}
			else if(i==array3.size()-1)
				System.out.println("The addition was done properly.");
		System.out.println("\nGenerating 3000 numbers and removing them from the map.");
		for(int i=0; i<3000; ++i)
		{
			Integer number=rand3.nextInt(upperbound3);
			if(chm3d.remove(number)==null)
				i--;
		}
		System.out.println("The new size of the map is: "+chm3d.size()+". The capacity is: "+chm3d.getCapacity());
		System.out.println("\nComparing the array and the map to see if the 3000 elements are actually deleted.");
		nullCount2=0;
		for(int i=0; i<array3.size(); ++i)
			if(chm3d.get(array3.get(i))==null)
				nullCount2++;
		System.out.println("There are "+nullCount2+" elements that are deleted from the map.");
		
		System.out.println("\n***DRIVER CODE OF IterableHashMap***");
		HashMap<Integer, String> ihm1=new IterableHashMap<Integer, String>();
		ihm1.put(1, "A");	ihm1.put(34, "I");
		ihm1.put(2, "B");	ihm1.put(21, "J");
		ihm1.put(3, "C");	ihm1.put(12, "K");
		ihm1.put(4, "D");	ihm1.put(56, "L");
		ihm1.put(5, "E");	ihm1.put(65, "M");
		ihm1.put(6, "F");	ihm1.put(13, "N");
		ihm1.put(7, "G");	ihm1.put(43, "O");
		ihm1.put(8, "H");	ihm1.put(17, "P");
		IterableHashMap<Integer, String>.MapIterator iter=(IterableHashMap<Integer, String>.MapIterator) ((IterableHashMap<Integer, String>) ihm1).iterator();
		System.out.println("Printing the keys in order.");
		while(iter.hasNext())
			System.out.println("Next: "+iter.next());
		System.out.println("");
		IterableHashMap<Integer, String>.MapIterator iter2=(IterableHashMap<Integer, String>.MapIterator) ((IterableHashMap<Integer, String>) ihm1).iterator(43);
		System.out.println("\nPrinting the keys in order with an iterator that starts right before 43");
		
		while(iter2.hasNext())
			System.out.println("Next: "+iter2.next());

		iter=(IterableHashMap<Integer, String>.MapIterator) ((IterableHashMap<Integer, String>) ihm1).iterator();
		System.out.println("\nPrinting the keys in reverse order.");
		while(iter.hasNext())
			System.out.println("Next: "+iter.previous());

		System.out.println("\nPrinting the keys in reverse order with an iterator that starts right before 21.");
		iter2=(IterableHashMap<Integer, String>.MapIterator) ((IterableHashMap<Integer, String>) ihm1).iterator(21);
		while(iter2.hasNext())
			System.out.println("Next: "+iter2.previous());
	}
}
