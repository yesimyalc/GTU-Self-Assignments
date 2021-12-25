import java.util.ArrayList;
import java.util.Scanner;

public class DriverCode 
{
	public static void main(String[] args) throws Exception
	{
		System.out.println("***SORTING DRIVER CODE***");
		InsertionSort<Integer> is=new InsertionSort<Integer>();
		HeapSort<Integer> hs=new HeapSort<Integer>();
		QuickSort<Integer> qs=new QuickSort<Integer>();
		ArrayList<Integer> a3=new ArrayList<Integer>();
		a3.add(3);	a3.add(5);	a3.add(1);	a3.add(8);	a3.add(34);	a3.add(67);
		a3.add(23);	a3.add(45);	a3.add(90);	a3.add(11);	a3.add(2);	
		ArrayList<Integer> a6=(ArrayList<Integer>) a3.clone();
		ArrayList<Integer> a7=(ArrayList<Integer>) a3.clone();
		is.sort(a3);
		System.out.println("***Insertion Sort Test***");
		for(int i=0; i<a3.size(); i++)
			System.out.println(a3.get(i));
		System.out.println("*************************");
		hs.sort(a6, null);
		System.out.println("***Heap Sort Test***");
		for(int i=0; i<a6.size(); i++)
			System.out.println(a6.get(i));
		System.out.println("*************************");
		qs.sort(a7, null);
		System.out.println("***Quick Sort Test***");
		for(int i=0; i<a7.size(); i++)
			System.out.println(a7.get(i));
		System.out.println("*************************");
		
		System.out.println("\n***CATEGORIES DRIVER CODE***");
		System.out.println("Creating a Categories class with a proper string. Also printing it out in a string form.");
		Categories cat=new Categories("\"[\"\"Category1 >> Category2 >> Category3 >> Category4\"\"]\"");
		System.out.println("String form: "+cat.convertToText());
		if(cat.contains("Category1") && cat.contains("Category2") && cat.contains("Category3") && cat.contains("Category4"))
			System.out.println("All the categories inserted are in the class.");
		System.out.println("\nCreating a Categories class with a given ArrayList of categories. Also printing it out in a string form.");
		ArrayList<String> theList=new ArrayList<String>();
		theList.add("Category1");	theList.add("Category2");	theList.add("Category3");
		Categories cat2=new Categories(theList);
		System.out.println("String form: "+cat2.convertToText());
		if(cat2.contains("Category1") && cat2.contains("Category2") && cat2.contains("Category3") && !cat2.contains("Category4"))
			System.out.println("All the categories inserted are in the class.");
		System.out.println("Do you want to test exceptions? Enter y if you want to.");
		Scanner sc=new Scanner(System.in);
		String answer=sc.next();
		if(answer.equals("y"))
		{
			System.out.println("Creating a Categories class with a non proper string");
			Categories cat3=new Categories("sdfsdfskdhjglkdfjgl");
		}
		
		System.out.println("\n***PRODUCT DRIVER CODE***");
		System.out.println("Creating an already registered product with an ID. Also printing it out to see if the info of the product is set properly.");
		Product p1=new Product("RSFEHYE2GRC9ZHMJ");
		System.out.println(p1);
		System.out.println("\nChanging the name of p1");
		p1.setName("Acrylic Wall Shelf");
		System.out.println("Changing the categories of p1");
		p1.setCategories(theList);
		System.out.println("Changing the description of p1");
		p1.setDescription("Edited Product");
		System.out.println("Changing the prices of p1.");
		p1.setPrices(300, 100, false);
		System.out.println("\nPrinting out the new p1.");
		System.out.println(p1);
		
		System.out.println("\nCreating a new product. Also printing it out to see if the info of the product is set properly.");
		Product p2=new Product("New Product", "12345678", theList, 100, 70, "Good Product", "BigCompany");
		System.out.println(p2);
		if(p2.isInCategory("Category3") && p2.isInRange(60, 90) && p2.isInStock())
			System.out.println("The info of p2 is set properly.");
		System.out.println("\nComparing p1 and p2");
		if(p1.equals(p2))
			System.out.println("They are equal.");
		else
			System.out.println("They are not equal.");
		System.out.println("\nRemoving p2 from the stock.");
		if(p2.removeFromFile())
			System.out.println("The removal is done.");
		System.out.println("Do you want to test exceptions? Enter y if you want to.");
		answer=sc.next();
		if(answer.equals("y"))
		{
			System.out.println("Trying to create an existing product with non existing ID.");
			Product p3=new Product("slfdkjsldf");
			
		}
		
		System.out.println("\n***CUSTOMER DRIVER CODE***");
		System.out.println("Creating an already existing customer. Also printing it out to see if the info of the customer is set properly.");
		User u1=new Customer("10003108", "123456");
		System.out.println("Customer: "+u1);
		System.out.println("\nSearching for a existing trader's products.");
		((Customer)u1).listProducts("Angelfish");
		System.out.println("\nSearching for a non-existing trader's products.");
		((Customer)u1).listProducts("BigCompany2");
		System.out.println("\nOrdering a product with a proper ID");
		((Customer)u1).order("SHIEH7JEHSAGYFKQ");
		System.out.println("\nSearching for Flower key word in the shop");
		((Customer)u1).search("Flower");
		System.out.println("\nSearching for key word that does not exist anywhere in the shop");
		((Customer)u1).search("dfgdfgdlfgkdfg");
		System.out.println("Do you want to test exceptions? Enter y if you want to.");
		answer=sc.next();
		if(answer.equals("y"))
		{
			System.out.println("Trying to order a non existing product with non existing ID.");
			((Customer)u1).order("sldfnjsldfknlsdkf");
		}
		
		System.out.println("\n***TRADER DRIVER CODE***");
		System.out.println("Creating 2 already existing traders. Also printing it out to see if the info of the trader is set properly.");
		User u2=new Trader("10003107", "126563");
		System.out.println("Trader: "+u2);
		User u3=new Trader("10001630", "125086");
		System.out.println("Trader: "+u3);
		System.out.println("\nFirst trader doesnt have orders and the second one has. Printing their orders.");
		System.out.println("First Trader");
		((Trader)u2).listOrders();
		System.out.println("Second Trader");
		((Trader)u3).listOrders();
		System.out.println("\nUsing meet and cancel orders for the first trader.");
		((Trader)u2).meetOrder();
		((Trader)u2).cancelOrder();
		System.out.println("Using meet and cancel orders for the second trader.");
		((Trader)u3).meetOrder();
		((Trader)u3).cancelOrder();
		System.out.println("\nEditing a product of the first trader.");
		Product p5=new Product("STIE2KFZTQFVYZQQ");
		((Trader)u2).editProductInfo(p5);
		//putting the removed product back to be able to run the program later on with the same code. 
		ArrayList<String> list2=new ArrayList<String>();
		list2.add("Home Decor & Festive Needs");	list2.add("Wall Decor & Clocks");	list2.add("Wall Shelves");	list2.add("Transtal Wall Shelves");
		list2.add("Transtal 'Triangle Lean' Corner 9\\\" Acrylic Wall ...");
		Product p4=new Product("Transtal 'Triangle Lean' Corner 9\" Acrylic Wall Shelf", "RSFEHYDXK3ZUVRVY", list2, 665, 369, 
				"Key Features of Transtal 'Triangle Lean' Corner 9\" Acrylic Wall Shelf Suitable For: Living Room & Bedroom Wall Mountable Rust Proof,Transtal "
				+ "'Triangle Lean' Corner 9\" Acrylic Wall Shelf (Number of Shelves - 1, Clear) Price: Rs. 369 •CORNER SHELF – your corner wall space could finally "
				+ "be used to display/store your memorabilia and other items •TRANSPARENT – makes the room look bigger, translucent edges ensure better safety. "
				+ "Gives your items a floating feel. •CRYSTAL CLEAR – Made of acrylic, is absolutely clear. Does not yellow or fade when exposed to water and weather. "
				+ "•UNBREAKABLE – at 6mm thickness, is sturdy and rigid •MULTIPURPOSE – Could be used in every room of the house. Display photos in the hall, "
				+ "spices in the kitchen or cologne in the bathroom.,Specifications of Transtal 'Triangle Lean' Corner 9\" Acrylic Wall Shelf (Number of Shelves - 1, Clear) "
				+ "General Brand Transtal Suitable For Living Room & Bedroom Model Number 501.3sL9x9 Material Acrylic Model Name 'Triangle Lean' Corner 9\" Color Clear Body & "
				+ "Design Features Mount Mechanism Screw-in Number of Shelves 1 Additional Features Rust Proof Yes In the Box Sales Package 9 x 9 Inches - 'Triangle Lean' Corner "
				+ "Shelf, Screw & wallplugs", "Transtal");
		((Customer)u1).order("RSFEHYDXK3ZUVRVY");
		((Customer)u1).order("RSFEHYDXK3ZUVRVY");
		System.out.println("Do you want to test exceptions? Enter y if you want to.");
		answer=sc.next();
		if(answer.equals("y"))
		{
			System.out.println("Trying to edit a product that does not belong to the trader.");
			Product p6=new Product("RSFEHYDXK3ZUVRVY");
			((Trader)u2).editProductInfo(p6);
		}
		
		System.out.println("");
		
		ShoppingSystem theShop=new ShoppingSystem();
		theShop.showMenu();
	}

}
