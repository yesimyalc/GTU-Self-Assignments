import java.util.Arrays;
import java.util.Random;
import java.util.Scanner;

public class DriverCode {

	public static void main(String[] args) 
	{
		System.out.println("***MaxHeap ClASS DRIVER CODE***\n");
		System.out.println("Creating an empty MaxHeap.");
		MaxHeap<Integer> m1=new MaxHeap();
		System.out.println("Inserting 15, 7, 9, 5, 3, 5, 6 to it.");
		m1.offer(15);
		m1.offer(7);
		m1.offer(9);
		m1.offer(5);
		m1.offer(3);
		m1.offer(5);
		m1.offer(6);
		System.out.println("Printing the heap in an array form to see if the elements are inserted in a proper order.");
		m1.printArray();
		System.out.println();
		
		System.out.println("Removing 4 elements from the heap");
		m1.poll();
		m1.poll();
		m1.poll();
		m1.poll();
		System.out.println("Printing the heap in an array form to see if the elements are removed in a proper order.");
		m1.printArray();
		System.out.println();
		
		System.out.println("Searching for some elements in the heap. Will print -1 if the element is not found.");
		System.out.println("5 is in index: "+ m1.search(5));
		System.out.println("6 is in index: "+ m1.search(6));
		System.out.println("3 is in index: "+ m1.search(3));
		System.out.println("15 is in index: "+ m1.search(15));
		System.out.println();
		
		System.out.println("Creating 2 new empty heaps.");
		MaxHeap<Integer> m2=new MaxHeap();
		MaxHeap<Integer> m3=new MaxHeap();
		System.out.println("Adding 15, 7, 9 to m3 heap.");
		m3.offer(15);
		m3.offer(7);
		m3.offer(9);
		System.out.println("Adding 12, 20, 3, 11, 4, 4 to m2 heap.");
		m2.offer(12);
		m2.offer(20);
		m2.offer(3);
		m2.offer(11);
		m2.offer(4);
		m2.offer(4);
		System.out.println("Merging m2 with m3.");
		System.out.println("Note that m3 has 3 distinct elements and m2 has 5 distinct elements. However there can be "
				+ "max 7 elements in an heap. One element from m3 will not be added.");
		m2.merge(m3);
		System.out.println("Printing m2 in an array form to see if the merge is done properly.");
		m2.printArray();
		System.out.println();

		System.out.println("Removing 7th greatest element from m2. 4 Must be deleted.");
		m2.remove(7);
		m2.printArray();
		System.out.println();
		System.out.println("Removing 3rd greatest element from m2. 12 Must be deleted.");
		m2.remove(3);
		m2.printArray();
		System.out.println();
		System.out.println("Removing 1st greatest element from m2. 20 Must be deleted.");
		m2.remove(1);
		m2.printArray();
		System.out.println();
		System.out.println("Removing 4th greatest element from m2. 4 Must be deleted.");
		m2.remove(4);
		m2.printArray();
		System.out.println();
		
		if(!m2.isFull())
			System.out.println("There is not 7 elements in m2 therefore it is not full yet.");
		if(!m2.isEmpty())
			System.out.println("m2 heap has elements inside, it is not empty.");
		System.out.println("Right now m2 has 4 elements, adding some more to make it full.");
		m2.offer(32);
		m2.offer(13);
		m2.offer(2);
		m2.offer(2);
		m2.offer(13);
		m2.offer(13);
		m2.offer(7);
		m2.printArray();
		if(m2.isFull())
			System.out.println("Now m2 is full.");
		System.out.println();
		
		System.out.println("The mode of m2 is: "+m2.getMode()+" The element which has this mode is: "+m2.getModeElement());
		System.out.println();
		
		System.out.println("***BSTHeapTree ClASS DRIVER CODE***\n");
		MaxHeap<Integer> m4=new MaxHeap<Integer>();
		m4.offer(37);
		m4.offer(23);
		m4.offer(10);
		m4.offer(10);
		m4.offer(16);
		m4.offer(19);
		m4.offer(9);
		m4.offer(9);
		m4.offer(9);
		m4.offer(3);
		MaxHeap<Integer> m5=new MaxHeap<Integer>();
		m5.offer(31);
		m5.offer(15);
		m5.offer(29);
		m5.offer(13);
		m5.offer(13);
		m5.offer(13);
		m5.offer(13);
		MaxHeap<Integer> m6=new MaxHeap<Integer>();
		m6.offer(124);
		m6.offer(52);
		m6.offer(98);
		m6.offer(51);
		m6.offer(38);
		m6.offer(87);
		m6.offer(80);
		m6.offer(80);
		MaxHeap<Integer> m7=new MaxHeap<Integer>();
		m7.offer(60);
		m7.offer(57);
		m7.offer(54);
		m7.offer(39);
		m7.offer(39);
		m7.offer(43);
		m7.offer(43);
		m7.offer(43);
		System.out.println("m4, m5, m6 and m7 will be added to the BSTHeapTree that will be created. Printing out m4, m5, m6 and m6:");
		System.out.println("Note that the heaps are the same as the example in the hw pdf.");
		System.out.println("m4");
		m4.printArray();
		System.out.println("The mode of m4 is: "+m4.getMode()+" The element which has this mode is: "+m4.getModeElement());
		System.out.println("\nm5");
		m5.printArray();
		System.out.println("The mode of m5 is: "+m5.getMode()+" The element which has this mode is: "+m5.getModeElement());
		System.out.println("\nm6");
		m6.printArray();
		System.out.println("The mode of m6 is: "+m6.getMode()+" The element which has this mode is: "+m6.getModeElement());
		System.out.println("\nm7");
		m7.printArray();
		System.out.println("The mode of m7 is: "+m7.getMode()+" The element which has this mode is: "+m7.getModeElement());
		System.out.println();
		
		System.out.println("Creating an empty BSTHeapTree and adding new heaps to it.");
		BSTHeapTree<Integer> b1=new BSTHeapTree<Integer>();
		if(b1.isEmpty())
			System.out.println("Empty tree is created.");
		b1.add(m4);
		b1.add(m5);
		b1.add(m6);
		b1.add(m7);;
		if(!b1.isEmpty())
			System.out.println("The tree is not empty anymore. Elements are added.");
		System.out.println("Printing the tree in preorder form. Note that the roots of the heaps will be printed to represent the heaps.");
		System.out.println(b1.toString());
		System.out.println("\nNow printing the heaps inside of the tree in preorder form.");
		b1.printArray();
		System.out.println();
		
		System.out.println("Adding 10, 37, 29, 31, 98, 57 to b1. Note that these all exist in the tree already. Also checking some returns.");
		System.out.println("New expected amount of 10: "+b1.add(10));
		System.out.println("New expected amount of 37: "+b1.add(37));
		System.out.println("New expected amount of 29: "+b1.add(29));
		b1.add(31);
		b1.add(98);
		b1.add(57);
		System.out.println("Printing the heaps inside of the tree in preorder form to see changes.");
		b1.printArray();
		System.out.println();
		System.out.println("Adding 14, 28, 30, 41, 59. Note that these do not exist but can be added without creating new heaps in the tree. Also checking some returns.");
		System.out.println("New expected amount of 14: "+b1.add(14));
		System.out.println("New expected amount of 28: "+b1.add(28));
		b1.add(30);
		b1.add(41);
		b1.add(59);
		System.out.println("Printing the heaps inside of the tree in preorder form to see changes.");
		b1.printArray();
		System.out.println();
		System.out.println("Adding 35, 20, 36, 35, 32, 130, 135, 146, 129, 65. Note that some new heaps must be created. Also checking some returns.");
		System.out.println("New expected amount of 35: "+b1.add(35));
		System.out.println("New expected amount of 20: "+b1.add(20));
		System.out.println("New expected amount of 36: "+b1.add(36));
		System.out.println("New expected amount of 35: "+b1.add(35));
		System.out.println("New expected amount of 32: "+b1.add(32));
		b1.add(130);
		b1.add(135);
		b1.add(146);
		b1.add(129);
		b1.add(65);
		System.out.println("Printing the heaps inside of the tree in preorder form to see changes.");
		b1.printArray();
		System.out.println("\nAlso printing the tree in preorder form.");
		System.out.println(b1.toString());
		System.out.println();
		
		System.out.println("Removing 37, 29, 43, 36, 38, 31, 29, 41, 135, 87, 52. Note that these all exist in the trees and no heaps will be deleted. Also checking some returns.");
		System.out.println("New expected amount of 37: "+b1.remove(37));
		System.out.println("New expected amount of 29: "+b1.remove(29));
		System.out.println("New expected amount of 43: "+b1.remove(43));
		System.out.println("New expected amount of 36: "+b1.remove(36));
		b1.remove(38);
		b1.remove(31);
		b1.remove(29);
		b1.remove(41);
		b1.remove(135);
		b1.remove(87);
		b1.remove(52);
		System.out.println("Printing the heaps inside of the tree in preorder form to see changes.");
		b1.printArray();
		System.out.println();
		System.out.println("Removing 65, 20, 146, 130, 129, 150, 366, 1, 0, 12. Note that some heaps must be deleted also some "
				+ "elements do not exist therefore nothing will be done about them. Also checking some returns.");
		System.out.println("New expected amount of 65: "+b1.remove(65));
		System.out.println("New expected amount of 20: "+b1.remove(20));
		System.out.println("New expected amount of 146: "+b1.remove(146));
		System.out.println("New expected amount of 130: "+b1.remove(130));
		System.out.println("New expected amount of 129: "+b1.remove(129));
		b1.remove(150);
		b1.remove(366);
		b1.remove(1);
		b1.remove(0);
		b1.remove(12);
		System.out.println("Printing the heaps inside of the tree in preorder form to see changes.");
		b1.printArray();
		System.out.println("\nAlso printing the tree in preorder form.");
		System.out.println(b1.toString());
		System.out.println();
		
		System.out.println("Searching for 37, 13, 35, 57, 144, 366, 9");
		System.out.println("37 is found : "+b1.find(37)+" times.");
		System.out.println("13 is found : "+b1.find(13)+" times.");
		System.out.println("35 is found : "+b1.find(35)+" times.");
		System.out.println("57 is found : "+b1.find(57)+" times.");
		System.out.println("144 is found : "+b1.find(144)+" times.");
		System.out.println("366 is found : "+b1.find(366)+" times.");
		System.out.println("9 is found : "+b1.find(9)+" times.");
		System.out.println();
		
		String answer;
		Scanner sc=new Scanner(System.in);
		System.out.println("Do you want to continue testing? Enter y if you want to. Asking this not to have long outputs printed at once.");
		answer=sc.next();
		if(!answer.equals("y"))
			System.exit(0);
		
		System.out.println("***IMPLEMENTATION TESTING***");
		
		System.out.println("\nCreating an empty array and an empty BSTHeapTree.");
		BSTHeapTree<Integer> b2=new BSTHeapTree<Integer>();
		Integer[] treeArray=new Integer[3000];
		HelperMethods<Integer> helper=new HelperMethods<Integer>();
		Random rand=new Random();
		int upperbound=5001;
		System.out.println("Generating 3000 numbers and inserting them into both the array and the tree");
		for(int i=0; i<3000; ++i)
		{
			Integer number=rand.nextInt(upperbound);
			treeArray[i]=number;
			b2.add(number);
		}
		
		System.out.println("Sorting the array:");
		Arrays.sort(treeArray);
		
		System.out.println("Printing out all the occurrences of the numbers in both the array and the tree. Note that they should be the same.");
		int count=0;
		int j=0;
		while(j<treeArray.length)
		{
			System.out.println("The element "+treeArray[j]+": Array: "+helper.findOccurences(treeArray, treeArray[j])+", Tree: "+b2.find(treeArray[j]));
			
			int occurrence=helper.findOccurences(treeArray, treeArray[j]);
			j+=occurrence;
			
			if(j>=treeArray.length-1)
				break;
		}
		System.out.println();
		
		System.out.println("Do you want to continue testing? Enter y if you want to. Asking this not to have long outputs printed at once.");
		answer=sc.next();
		if(!answer.equals("y"))
			System.exit(0);
		
		System.out.println("Searching for 100 random elements that are supposed to be in the array and checking if the occurences are correct by "
				+ "double checking from the tree. Note that the occurences should be the same.");
		Integer[] searchSet1=new Integer[100];
		int index=0;
		while(index!=100)
		{
			Integer number=rand.nextInt(upperbound);
			if(Arrays.asList(treeArray).contains(number) && !Arrays.asList(searchSet1).contains(number))
			{
				searchSet1[index]=number;
				index++;
			}	
		}
		for(int i=0; i<searchSet1.length; ++i)
			System.out.println("The element "+searchSet1[i]+": Array: "+helper.findOccurences(treeArray, searchSet1[i])+", Tree: "+b2.find(searchSet1[i]));
		System.out.println();
		System.out.println("Searching for 10 random elements that are not supposed to be in the array and checking if the occurences are correct by "
				+ "double checking from the tree. Note that all should be 0.");
		Integer[] searchSet2=new Integer[10];
		index=0;
		while(index!=10)
		{
			Integer number=rand.nextInt(upperbound);
			if(!Arrays.asList(treeArray).contains(number) && !Arrays.asList(searchSet1).contains(number))
			{
				searchSet2[index]=number;
				index++;
			}	
		}
		for(int i=0; i<searchSet2.length; ++i)
			System.out.println("The element "+searchSet2[i]+": Array: "+helper.findOccurences(treeArray, searchSet2[i])+", Tree: "+b2.find(searchSet2[i]));
		System.out.println();
		
		System.out.println("Finding the mode of the tree and double checking with the array's mode. Note that they should be same however"
				+ "if there are more than one elements with the frequency, they might differ. However the frequency should be the same.");
		System.out.println("Mode of the Array: "+helper.findMode(treeArray)+", Tree: "+b2.find_mode());
		System.out.println("Mode frequency of the Array: "+helper.findOccurences(treeArray, helper.findMode(treeArray))+", Tree: "+b2.find(b2.find_mode()));
		System.out.println();
		
		System.out.println("Removing 10 elements that does not exist in the array from the array then checking if the occurence count is proper with the tree. "
				+ "No elements should be changed.");
		Integer[] indexArray=new Integer[10];
		for(int i=0; i<searchSet2.length; ++i)
			indexArray[i]=helper.findFirstIndex(treeArray, searchSet2[i]);
		
		Integer[] newArray=new Integer[3000];
		for(int i=0, k=0; i<treeArray.length; ++i)
			if(!Arrays.asList(indexArray).contains((Integer)i))
			{
				newArray[k]=treeArray[i];
				k++;
			}
		treeArray=newArray;
		for(int i=0; i<searchSet2.length; ++i)
			System.out.println("The element "+searchSet2[i]+": Array: "+helper.findOccurences(treeArray, searchSet2[i])+", Tree: "+b2.find(searchSet2[i]));	
		System.out.println();
		
		System.out.println("Removing 100 elements from the array then checking if the occurence count is proper with the tree. Tree must have 1 more than the array.");
		indexArray=new Integer[100];
		for(int i=0; i<searchSet1.length; ++i)
			indexArray[i]=helper.findFirstIndex(treeArray, searchSet1[i]);
		
		newArray=new Integer[2900];
		for(int i=0, k=0; i<treeArray.length; ++i)
			if(!Arrays.asList(indexArray).contains((Integer)i))
			{
				newArray[k]=treeArray[i];
				k++;
			}

		treeArray=newArray;
		for(int i=0; i<searchSet1.length; ++i)
			System.out.println("The element "+searchSet1[i]+": Array: "+helper.findOccurences(treeArray, searchSet1[i])+", Tree: "+b2.find(searchSet1[i]));
		System.out.println();
	
	}
}
