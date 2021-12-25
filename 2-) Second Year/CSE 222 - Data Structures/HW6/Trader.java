import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.NoSuchElementException;
import java.util.Scanner;

public class Trader extends AbstractUser
{
	LinkedList<Product> orders;
	
	/**
	 * This is a constructor for a user that is not registered to the system before.
	 * Automatically determines the user id, asks for a password.
	 * Saves the user into the UsersInfo.txt file.
	 * If there is already a trader with the given name, that trader is being registered. There cannot be 2 traders with the same name.
	 * @param userName is the user's name who is going to be created.
	 */
	public Trader(String userName)
	{
		super(userName);
		String check=checkID();
		if(check!=null)
		{
			System.out.println("There is already an existing trader with the current name. This trader is being logged in.");
			ID=check;
			password=retrievePassword(check);
		}
		else
			saveToFile();
		
		orders=new LinkedList<Product>();
		setOrders();
	}
	
	/**
	 * This is a constructor for a user that has been registered to the system before.
	 * Checks if the user actually was registered before.
	 * Automatically sets the information of the user. 
	 * @param userID is the ID of the user.
	 * @param userPass is the password of the user.
	 */
	public Trader(String userID, String userPass)
	{
		super(userID, userPass);
		orders=new LinkedList<Product>();
		setOrders();
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
	public Trader(String userID, String userName, String userPass, boolean check)
	{
		super(userID, userName, userPass, check);
		orders=new LinkedList<Product>();
		setOrders();
	}
	
	/**
	 * Saves the user to the UsersInfo.txt file to register the user to the system.
	 */
	private void saveToFile()
	{
		try 
		{
			FileWriter fw = new FileWriter("UsersInfo.txt", true);
			fw.write("\n"+getID()+" "+getName()+" ; "+getPass()+" "+"Trader");
			fw.close();
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
		}
	}
	
	/**
	 * Checks if there is any other trader with another ID but with the same name
	 * @return the the ID of the other trader with the same name if there is, null if there is none
	 */
	private String checkID()
	{
		try
		{
			File infoFile = new File("UsersInfo.txt");
			Scanner fileSC= new Scanner(infoFile);
			Scanner inSC= new Scanner(System.in);
			fileSC.nextLine();
			
			while(fileSC.hasNext())
			{
				String readWord=new String(fileSC.next());
				StringBuffer readWord2=new StringBuffer(fileSC.next());
				String read=fileSC.next();
				while(!read.equals(";"))
				{
					readWord2.append(" "+read);
					read=fileSC.next();
				}
				String readWord3=new String(fileSC.next());

				if(getName().equals(readWord2.toString()) && !readWord.equals(getID()))
				{
					fileSC.close();
					return readWord;
				}
				fileSC.next();
			}
			fileSC.close();	
			return null;
		}
		catch(FileNotFoundException exc)
		{
			System.out.println("The file does not exist.");
			System.exit(-1);
		}
		return null;
	}
	
	/**
	 * Retrieves the password of the trader with the given ID
	 * @param searchedID is the given ID
	 * @return the found password, return null if not found
	 */
	private String retrievePassword(String searchedID)
	{
		try
		{
			File infoFile = new File("UsersInfo.txt");
			Scanner fileSC= new Scanner(infoFile);
			Scanner inSC= new Scanner(System.in);
			fileSC.nextLine();
			
			while(fileSC.hasNext())
			{
				String readWord=new String(fileSC.next());
				if(readWord.equals(searchedID))
				{
					StringBuffer readWord2=new StringBuffer(fileSC.next());
					String read=fileSC.next();
					while(!read.equals(";"))
					{
						readWord2.append(" "+read);
						read=fileSC.next();
					}
					String readWord3=new String(fileSC.next());
					fileSC.close();
					return readWord3;
				}
				else
					fileSC.nextLine();
			}
			fileSC.close();	
			return null;
		}
		catch(FileNotFoundException exc)
		{
			System.out.println("The file does not exist.");
			System.exit(-1);
		}
		return null;
	}
	
	/**
	 * Reads the orders of the trader from Orders.csv file. 
	 * Sets the read order to the orders.
	 */
	private void setOrders()
	{
		File infoFile = new File("Orders.csv");
		if(!infoFile.exists())
			return;
		try 
		{
			FileReader pathToCsv=new FileReader("Orders.csv");
			BufferedReader csvReader = new BufferedReader(pathToCsv);
			String row;
			row=csvReader.readLine();
			while ((row = csvReader.readLine())!= null) 
			{
			    String[] data = row.split(";");
			    if(data[6].equals(getName()))
			    	orders.addLast(new Product(data[0], data[1], data[2], Double.valueOf(data[3]), Double.valueOf(data[4]), data[5], data[6], true));
			}
			
			csvReader.close();
			pathToCsv.close();
		}
		catch(FileNotFoundException exc)
		{
			System.out.println("The file does not exist.");
			System.exit(-1);
		}
		catch (IOException e) 
		{
			e.printStackTrace();
			System.exit(-1);
		}
	}
	
	/**
	 * Removes the product with the given ID from the Orders.csv file.
	 * @param removedProductID is the given ID
	 */
	private void removeOrders(String removedProductID)
	{
		try 
		{
			File inputFile=new File("Orders.csv");
			File outputFile=new File("temp.csv");
			FileReader pathToCsv=new FileReader("Orders.csv");
			BufferedReader csvReader = new BufferedReader(pathToCsv);
			String row;
			row=csvReader.readLine();
			FileWriter writer=new FileWriter(outputFile);
			boolean check=false;
			writer.append("product_name"+';'+"id"+';'+"product_category"+';'+"price"+';'+"discounted_price"+';'+"description"+';'+"trader"+';'+'\n');
			while ((row = csvReader.readLine())!= null) 
			{
			    String[] data = row.split(";");
			    if(!data[1].equals(removedProductID) || check==true)
			    	writer.append(data[0]+';'+data[1]+';'+data[2]+';'+String.valueOf(data[3])+';'+String.valueOf(data[4])+';'+data[5]+';'+data[6]+';'+'\n');
			    else
			    	check=true;
			}
			csvReader.close();
			pathToCsv.close();
			writer.close();
			inputFile.delete();
			outputFile.renameTo(inputFile);
		}
		catch(FileNotFoundException exc)
		{
			System.out.println("The file does not exist.");
			System.exit(-1);
		}
		catch (IOException e) 
		{
			e.printStackTrace();
			System.exit(-1);
		}
	}
	
	/**
	 * Edits the info of the given product
	 * It is possible to edit the name, categories, description and the prices of the product.
	 * @param editedProduct is the given product
	 */
	public void editProductInfo(Product editedProduct)
	{
		if(!editedProduct.isInStock() || !editedProduct.getTraderName().equals(name))
		{
			System.err.println("You cannot edit a prdouct info that does not belong to your shop.");
			throw new IllegalArgumentException();
		}
			
		Scanner sc=new Scanner(System.in);
		String check;
		System.out.println("You can edit the name, categories, description and prices of the product.");
		System.out.println("Enter y if you want to edit the name.");
		check=sc.next();
		sc.nextLine();
		if(check.equals("y"))
		{
			System.out.print("Enter the new name: ");
			editedProduct.setName(sc.nextLine());
		}
		System.out.println("Enter y if you want to edit the categories.");
		check=sc.next();
		sc.nextLine();
		if(check.equals("y"))
		{
			System.out.print("Enter the category name to add. If you do not want to add more categories, enter -1 ");
			Scanner sc2=new Scanner(System.in);
			String answer=sc2.nextLine();
			ArrayList<String> newCat=new ArrayList<String>();
			while(!answer.equals("-1"))
			{
				newCat.add(answer);
				answer=sc2.nextLine();
			}
			if(newCat.isEmpty())
				System.out.println("You have not added any new categories, old categories will remain.");
			else
				editedProduct.setCategories(newCat);
		}
		System.out.println("Enter y if you want to edit the description.");
		check=sc.next();
		sc.nextLine();
		if(check.equals("y"))
		{
			System.out.print("Enter the new description: ");
			editedProduct.setDescription(sc.nextLine());
		}
		System.out.println("Enter y if you want to edit the prices.");
		check=sc.next();
		sc.nextLine();
		if(check.equals("y"))
		{
			System.out.print("Enter the new price: ");
			int p1=sc.nextInt();
			sc.nextLine();
			System.out.print("Enter the new discounted price: ");
			int p2=sc.nextInt();
			sc.nextLine();
			editedProduct.setPrices(p1, p2, false);
		}
		System.out.println("Info are edited.");
	}
	
	/**
	 * Lists all the orders of the trader.
	 */
	public void listOrders()
	{
		if(orders.isEmpty())
		{
			System.out.println("There are no orders.");
			return;
		}
		Iterator<Product> listIter=orders.iterator();
		System.out.println("The Orders:");
		while(listIter.hasNext())
			System.out.println(listIter.next());
	}
	
	/**
	 * Meets the very first order in the orders list.
	 * Deletes the order from the orders.csv
	 * Deleted the product from the stock.
	 */
	public void meetOrder()
	{
		if(orders.isEmpty())
			return;
		
		Product metOrder=orders.getFirst();
		if(!metOrder.isInStock())
		{
			System.err.println("The order that is trying to be met is not in the products list of the shop. You cannot meet an order if it is not in the stock.");
			throw new NoSuchElementException();
		}
		removeOrders(orders.removeFirst().getID());
		metOrder.removeFromFile();
	}
	
	/**
	 * Meets all the orders in the orders list.
	 * Deletes the orders from the orders.csv
	 * Deleted the products from the stock.
	 */
	public void meetAllOrders()
	{
		if(orders.isEmpty())
			return;
		
		while(!orders.isEmpty())
			meetOrder();
	}
	
	/**
	 * Cancels the very first order in the orders list.
	 * Deletes the order from the orders.csv
	 */
	public void cancelOrder()
	{
		if(orders.isEmpty())
			return;
		
		removeOrders(orders.removeFirst().getID());
	}
	
	/**
	 * Cancels all the orders order in the orders list.
	 * Deletes the orders from the orders.csv
	 */
	public void cancelAllOrders()
	{
		if(orders.isEmpty())
			return;
		
		while(!orders.isEmpty())
			cancelOrder();
	}
}
