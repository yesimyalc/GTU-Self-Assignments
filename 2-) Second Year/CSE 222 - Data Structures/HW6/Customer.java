import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.Scanner;
import java.util.TreeSet;

public class Customer extends AbstractUser
{
	/**
	 * This is a constructor for a user that is not registered to the system before.
	 * Automatically determines the user id, asks for a password.
	 * Saves the user into the UsersInfo.txt file.
	 * @param userName is the user's name who is going to be created.
	 */
	public Customer(String userName)
	{
		super(userName);
		saveToFile();
	}
	
	/**
	 * This is a constructor for a user that has been registered to the system before.
	 * Checks if the user actually was registered before.
	 * Automatically sets the information of the user. 
	 * @param userID is the ID of the user.
	 * @param userPass is the password of the user.
	 */
	public Customer(String userID, String userPass)
	{
		super(userID, userPass);
	}
	
	/**
	 * This is a constructor for a user that has been registered to the system before.
	 * Pre: Assumes the user was actually registered before does not check.
	 * Automatically sets the information of the user. 
	 * @param userID
	 * @param userName
	 * @param userPass
	 * @param check
	 */
	public Customer(String userID, String userName, String userPass, boolean check)
	{
		super(userID, userName, userPass, check);
	}
	
	/**
	 * Saves the user to the UsersInfo.txt file to register the user to the system.
	 */
	private void saveToFile()
	{
		try 
		{
			FileWriter fw = new FileWriter("UsersInfo.txt", true);
			fw.write("\n"+getID()+" "+getName()+" ; "+getPass()+" "+"Customer");
			fw.close();
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
		}
	}
	
	/**
	 * Lists all the products of the given trader.
	 * @param traderName is the given trader.
	 */
	public void listProducts(String traderName)
	{
		try 
		{
			FileReader pathToCsv=new FileReader("Products.csv");
			BufferedReader csvReader = new BufferedReader(pathToCsv);
			String row;
			row=csvReader.readLine();
			TreeSet<Product> productTree=new TreeSet<Product>();
			while ((row = csvReader.readLine())!= null) 
			{
			    String[] data = row.split(";");
			    if(data[6].equals(traderName))
			    	((TreeSet)productTree).add(new Product(data[0], data[1], data[2], Double.valueOf(data[3]), Double.valueOf(data[4]), data[5], data[6], true));

			}
			csvReader.close();
			if(!productTree.isEmpty())
			{
				Iterator<Product> iter=productTree.iterator();
				while(iter.hasNext())
					System.out.println(iter.next());
			}
			else
				System.out.println("This trader does not have any products.");
		}
		catch(FileNotFoundException exc)
		{
			System.err.print(exc);
			System.exit(-1);
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
			System.exit(-1);
		}
	}
	
	/**
	 * Makes an order from the system. The order is added to the Orders.csv file.
	 * If the product does not exists in the system an exception will be thrown and the program will exit.
	 * @param productID is the id of the product that will be ordered
	 */
	public void order(String productID)
	{
		try
		{
			Product orderedProduct=new Product(productID);
			boolean exists=true;
			File inputFile=new File("Orders.csv");
			if(!inputFile.exists())
				exists=false;
			FileWriter writer=new FileWriter("Orders.csv", true);
			if(!exists)
				writer.append("product_name"+';'+"id"+';'+"product_category"+';'+"price"+';'+"discounted_price"+';'+"description"+';'+"trader"+';'+'\n');
			writer.append(orderedProduct.getName()+';'+orderedProduct.getID()+';'+orderedProduct.getCategories()+';'+String.valueOf(orderedProduct.getPrice())+
					';'+String.valueOf(orderedProduct.getDiscountPrice())+';'+orderedProduct.getDescription()+';'+orderedProduct.getTraderName()+';'+'\n');
			writer.close();
			System.out.println("You have successfuly made your order.");
		}
		catch(FileNotFoundException exc)
		{
			System.err.print(exc);
			System.exit(-1);
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
			System.exit(-1);
		}
		
	}
	
	/**
	 * Search and products by their names. The products that contain the 
	 * search text in their name or description should be returned in the results. 
	 * The search results are shown in decreasing order of the product name. Can be sorted 
	 * the products in the search results by their prices, the percentage of the discount, or the name.
	 * The search results can be filtered by their category or the prices with respect to given upper 
	 * or lower (or both) thresholds.
	 * @param searchText is the text that will be searched in name and description of the products.
	 */
	public void search(String searchText)
	{
		try 
		{
			InsertionSort<Product> insSort=new InsertionSort<Product>();
			HeapSort<Product> heapSort=new HeapSort<Product>();
			QuickSort<Product> quickSort=new QuickSort<Product>();
			FileReader pathToCsv=new FileReader("Products.csv");
			BufferedReader csvReader = new BufferedReader(pathToCsv);
			String row;
			row=csvReader.readLine();
			ArrayList<Product> foundProducts=new ArrayList<Product>();
			while ((row = csvReader.readLine())!= null) 
			{
			    String[] data = row.split(";");
			    if(data[0].contains(searchText) || data[5].contains(searchText))
			    	foundProducts.add(new Product(data[0], data[1], data[2], Double.valueOf(data[3]), Double.valueOf(data[4]), data[5], data[6], true));

			}
			csvReader.close();
			
			if(foundProducts.isEmpty())
			{
				System.out.println("No search results are found.");
				return;
			}
			System.out.println("The search results for "+searchText+": The results are sorted in name order.");
			insSort.sort(foundProducts);
			for(int i=0; i<foundProducts.size(); ++i)
				System.out.println(foundProducts.get(i));
			
			System.out.println("\nIf you want to sort the results by price enter PRICE, if you want to sort by discount percentage enter DISCOUNT. If you "
					+ "do not want to sort enter anything else.");
			Scanner sc=new Scanner(System.in);
			String answer=sc.next();
			if(answer.equals("PRICE"))
			{
				heapSort.sort(foundProducts, new PriceComparator());
				System.out.println("\nThe search results for "+searchText+": The results are sorted in current price order.");
				for(int i=0; i<foundProducts.size(); ++i)
					System.out.println(foundProducts.get(i));
			}
			else if(answer.equals("DISCOUNT"))
			{
				quickSort.sort(foundProducts, new DiscountComparator());
				System.out.println("\nThe search results for "+searchText+": The results are sorted in discount percentage order.");
				for(int i=0; i<foundProducts.size(); ++i)
					System.out.println(foundProducts.get(i));
			}
			System.out.println("\nIf you want to filter the results by price enter PRICE, if you want to filter by categories enter CATEGORY. If you "
					+ "do not want to filter enter anything else.");
			answer=sc.next();
			if(answer.equals("PRICE"))
			{
				System.out.print("Enter minimum price: ");
				int min=sc.nextInt();
				System.out.print("Enter maximum price: ");
				int max=sc.nextInt();
				System.out.println("The filtered search results for "+searchText+":");
				for(int i=0; i<foundProducts.size(); ++i)
					if(foundProducts.get(i).isInRange(min, max))
						System.out.println(foundProducts.get(i));
			}
			else if(answer.equals("CATEGORY"))
			{
				System.out.println("Enter the category you want to filter: ");
				sc.nextLine();
				answer=sc.nextLine();
				System.out.println("The filtered search results for "+searchText+":");
				for(int i=0; i<foundProducts.size(); ++i)
					if(foundProducts.get(i).isInCategory(answer))
						System.out.println(foundProducts.get(i));
			}
			
		}
		catch(FileNotFoundException exc)
		{
			System.err.print(exc);
			System.exit(-1);
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
			System.exit(-1);
		}
	}
}
