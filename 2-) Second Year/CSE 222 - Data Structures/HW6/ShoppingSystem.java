import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Scanner;

/**
 * A class that holds the users and the information of the currently logged in user
 * Can have users register, login to the system.
 * Shows appropriate menus for the users.
 */
public class ShoppingSystem 
{
	Hashtable<String, User> users;
	User loggedInUser;
	
	/**
	 * Constructor that sets the shopping system.
	 * Users are registered, the first menu is shown.
	 */
	public ShoppingSystem()
	{
		users=new Hashtable<String, User>();
		setUsers();
		firstMenu();
	}
	
	/**
	 * Retrieves all the registered users from the UsersInfo.txt file and stores them in the users data field.
	 */
	private void setUsers()
	{
		try
		{
			File infoFile = new File("UsersInfo.txt");
			Scanner fileSC= new Scanner(infoFile);
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
				String readWord4=new String(fileSC.next());
				
				if(readWord4.equals("Customer"))
					users.put(readWord, new Customer(readWord, readWord2.toString(), readWord3, true));
				else
					users.put(readWord, new Trader(readWord, readWord2.toString(), readWord3, true));
			}
			fileSC.close();
		}
		catch(FileNotFoundException exc)
		{
			System.out.println("The file does not exist.");
			System.exit(-1);
		}
	}
	
	/**
	 * Prints out the first menu of the system.
	 * This menu lets the user to register or login.
	 * Users can login with their ID and password.
	 * Users can register with their name and determine a password. 
	 */
	private void firstMenu()
	{
		Scanner sc=new Scanner(System.in);
		System.out.println("******************************");
		System.out.print("Do you want to register or login? Enter register or login.");
		String answer1=sc.next();
		while(!answer1.equals("register") && !answer1.equals("login"))
		{
			System.out.println("You have entered an invalid answer please enter again.");
			answer1=sc.next();
		}
		
		if(answer1.equals("register"))
		{
			System.out.print("Enter your name: ");
			sc.nextLine();
			String name=sc.nextLine();
			System.out.print("Enter if you wanna register as a customer or a trader: (customer or trader) ");
			String role=sc.next();
			while(!role.equals("customer") && !role.equals("trader"))
			{
				System.out.println("You have entered an invalid answer please enter again.");
				role=sc.next();
			}
			User newUser;
			if(role.equals("customer"))
				newUser=new Customer(name);
			else
				newUser=new Trader(name);
			users.put(newUser.getID(), newUser);
			
			login(newUser.getID(), newUser.getPass());
		}
		else
		{
			System.out.print("Enter your user ID: ");
			String answer=sc.next();
			System.out.print("Enter your password: ");
			String answer2=sc.next();
			login(answer, answer2);
		}
	}
	
	/**
	 * Checks if there is a user with the given ID and the password.
	 * If there is registers the user as the loggedInUser.
	 * If there is a user with the given id but the password is wrong, asks the password again
	 * @throws IllegalArgumentexception if there is no users with the given ID 
	 * @param loginID is the given id
	 * @param loginPass is the given password
	 */
	private void login(String loginID, String loginPass)
	{
		User loginUser=users.get(loginID);
		if(loginUser==null)
		{
			System.err.print("There is no user registered with this ID");
			throw new IllegalArgumentException();
		}

		while(!loginUser.getPass().equals(loginPass))
		{
			System.out.println("You have entered a wrong password. Please enter again.");
			Scanner sc=new Scanner(System.in);
			loginPass=sc.next();
		}
		
		loggedInUser=loginUser;
		System.out.println("******************************");
	}
	
	/**
	 * If the loggedInUser is a customer shows the customer menu.
	 * If the loggedInUser is a trader shows the trader menu.
	 */
	public void showMenu()
	{
		if(loggedInUser instanceof Customer)
			showCustomerMenu();
		else
			showTraderMenu();
	}

	/**
	 * Shows the trader menu.
	 * Trader menu has LISTORDERS, EDITODERS, EDITPRODUCTS, ADDPRODUCT, REMOVEPRODUCT functionalities.
	 * LISTORDERS -> Lists all the orders from customers for the trader.
	 * EDITORDERS -> Goes into orders menu. 
	 * EDITPRODUCT -> Edits the information of a product.
	 * ADDPRODUCT -> Adds a product to the stock.
	 * REMOVEPRODUCT -> Removes a product from the stock.
	 */
	private void showTraderMenu()
	{
		Scanner sc=new Scanner(System.in);
		System.out.println("**********Trader Menu**********");
		System.out.println("Enter LISTORDERS to see the orders from customers.");
		System.out.println("Enter EDITORDERS to meet&cancel the orders from customers.");
		System.out.println("Enter EDITPRODUCT to edit the information of a product.");
		System.out.println("Enter ADDPRODUCT to add a new product.");
		System.out.println("Enter REMOVEPRODUCT to remove a product.");
		System.out.println("Enter QUIT to exit.");
		String answer="start";
		while(!answer.equals("QUIT"))
		{
			System.out.print("Enter your command: ");
			answer=sc.next();
			
			if(answer.equals("LISTORDERS"))
				((Trader)loggedInUser).listOrders();
			else if(answer.equals("EDITORDERS"))
				ordersMenu();
			else if(answer.equals("EDITPRODUCT"))
			{
				System.out.println("Enter the product ID: ");
				String productID=sc.next();
				((Trader)loggedInUser).editProductInfo(new Product(productID));
			}
			else if(answer.equals("ADDPRODUCT"))
			{
				sc.nextLine();
				System.out.print("Enter the product's name you want to add:");
				String pName=sc.nextLine();
				System.out.print("Enter the product's ID you want to add:");
				String pID=sc.next();
				sc.nextLine();
				System.out.print("Enter the category name to add. If you do not want to add more categories, enter -1 ");
				Scanner sc2=new Scanner(System.in);
				String answer2=sc2.nextLine();
				ArrayList<String> newCat=new ArrayList<String>();
				while(!answer2.equals("-1"))
				{
					newCat.add(answer2);
					answer2=sc2.nextLine();
				}
				if(newCat.isEmpty())
					newCat.add("None");
				System.out.print("Enter the product's description you want to add:");
				String desc=sc.nextLine();
				System.out.print("Enter the product's old price you want to add:");
				double pPrice=sc.nextDouble();
				System.out.print("Enter the product's discounted current price you want to add:");
				double pDPrice=sc.nextDouble();
				
				Product newProduct=new Product(pName, pID, newCat, pPrice, pDPrice, desc, loggedInUser.getName());
				System.out.println("The product is added.");
			}
			else if(answer.equals("REMOVEPRODUCT"))
			{
				System.out.println("Enter the product's ID you want to remove:");
				String productID=sc.next();
				Product removedProduct=new Product(productID);
				removedProduct.removeFromFile();
				System.out.println("The product is removed.");
			}
			else if(!answer.equals("QUIT"))
				System.out.println("You have entered an invalid command please enter again");
			System.out.println();
		}
		System.out.println("*******************************");
	}
	
	/**
	 * Shows the EDITORDERS menu. Traders can meet or cancel orders in this menu.
	 */
	private void ordersMenu()
	{
		System.out.println("**********Orders Menu**********");
		System.out.println("Enter MEET to meet the first order.");
		System.out.println("Enter MEETALL to meet all the orders");
		System.out.println("Enter CANCEL to cancel the first order.");
		System.out.println("Enter CANCELALL to cancel all the orders.");
		System.out.print("Enter your command: ");
		Scanner sc=new Scanner(System.in);
		String answer=sc.next();
		
		while(!answer.equals("MEET") && !answer.equals("MEETALL") && !answer.equals("CANCEL") && !answer.equals("CANCELALL"))
		{
			System.out.println("You have entered a wrong command. Please enter again.");
			answer=sc.next();
		}
		if(answer.equals("MEET"))
			((Trader)loggedInUser).meetOrder();
		else if(answer.equals("MEETALL"))
			((Trader)loggedInUser).meetAllOrders();
		else if(answer.equals("CANCEL"))
			((Trader)loggedInUser).cancelOrder();
		else if(answer.equals("CANCELALL"))
			((Trader)loggedInUser).cancelAllOrders();
		System.out.println("*******************************");
	}
	
	/**
	 * Shows the customer menu.
	 * This menu has SEARCH, ORDER, LIST functionalities.
	 * SEARCH -> Searches for products with a given text.
	 * ORDER -> Makes orders.
	 * LIST -> Lists the products of a specific trader.
	 */
	private void showCustomerMenu()
	{
		Scanner sc=new Scanner(System.in);
		System.out.println("**********Customer Menu**********");
		System.out.println("Enter SEARCH to search for products.");
		System.out.println("Enter ORDER to make orders.");
		System.out.println("Enter LIST to search for products of a specific trader.");
		System.out.println("Enter QUIT to exit.");
		String answer="start";
		while(!answer.equals("QUIT"))
		{
			System.out.print("Enter your command: ");
			answer=sc.next();
			
			if(answer.equals("SEARCH"))
			{
				System.out.println("What do you want to search for?");
				sc.nextLine();
				String search=sc.nextLine();
				((Customer)loggedInUser).search(search);
			}
			else if(answer.equals("ORDER"))
			{
				System.out.println("Enter the ID of the product you want to buy: ");
				String productID=sc.next();
				Product orderedProduct=new Product(productID);
				((Customer)loggedInUser).order(productID);
			}
			else if(answer.equals("LIST"))
			{
				System.out.println("Enter the trader name you want to see the products of.");
				sc.nextLine();
				String search=sc.nextLine();
				((Customer)loggedInUser).listProducts(search);
			}
			else if(!answer.equals("QUIT"))
				System.out.println("You have entered an invalid command please enter again");
			System.out.println();
		}
		System.out.println("*******************************");
	}
}
