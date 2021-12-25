import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

/**
*A class that holds the general informations and 
*customerNumber, phoneNumber, address, pastOrders, newOrders info and methods of a Customer entity.
*/
public class Customer extends User
{
	//Fields
	private int customerNumber;
	private String phoneNumber=null;
	private String address=null;
	public static int totalCustomerCount;
	private HybridList<Product> pastOrders=null;
	private HybridList<Product> newOrders=null;
	
	//Constructors
	/**
	 * Constructor that sets everything as an empty string and the status as old. It should be avoided to use this constructor.
	 */
	public Customer()
	{
		this("", "", "", "", "old");
	}
	/**
	 * @param nameInfo is a String that will be the name of the user
	 * @param surnameInfo is a String that will be the surname of the user
	 * @param mailInfo is a String that will be the email of the user
	 * @param passInfo is a String that will be the password of the user.
	 * Constructor that sets the user information as the given information. Also sets the status as new.
	 */
	public Customer(String nameInfo, String surnameInfo, String mailInfo, String passInfo)
	{
		this(nameInfo, surnameInfo, mailInfo, passInfo, "new");
		
	}
	/**
	 * @param nameInfo is a String that will be the name of the user
	 * @param surnameInfo is a String that will be the surname of the user
	 * @param mailInfo is a String that will be the email of the user
	 * @param passInfo is a String that will be the password of the user
	 * @param statusInfo is a String that will be the status of the user. It should be either new or old.
	 * Constructor that sets the user information as the given information.
	 * Increments the totalCustomerCount everytime a new costumer is created. This will be used in the determination of customer numbers.
	 */
	public Customer(String nameInfo, String surnameInfo, String mailInfo, String passInfo, String statusInfo)
	{
		super(nameInfo, surnameInfo, mailInfo, passInfo, statusInfo);
		
		totalCustomerCount++;
		customerNumber=getTotalCustomerCount();
		
		//If the status is new, the customer should be saved and if it is old, then the customer orders from the past must be loaded.
		if(getStatus().equals("new"))
			saveCustomer();
		else
			loadOrders();
	}
	
	//Getters
	/**
	 * @return the customer number of the customer.
	 */
	public int getCustomerNo(){return customerNumber;}
	
	/**
	 * @return the totalCustomerCount.
	 */
	public static int getTotalCustomerCount(){return totalCustomerCount;}
	
	/**
	 * @param orderIndex will be the index that will be used to return one of the new orders. It should be bigger than 0 and smaller than newOrders.length.
	 * @return the new order in the index orderIndex of the new orders.
	 * @throws InvalidIndex if the orderIndex does not satisfy the requirements.
	 */
	public Product getNewOrder(int orderIndex)throws InvalidIndex
	{
		if(orderIndex<0 || orderIndex>newOrders.size())
			throw new InvalidIndex("There is no order with this index.");
		
		return newOrders.get(orderIndex);
	}
	
	/**
	 * @return 0 if the newOrders is null.
	 * @return newOrders.length if newOrders is not null.
	 */
	public int getNewOrderCount()
	{
		if(newOrders!=null)
			return newOrders.size();
		
		return 0;
	}
	
	/**
	 * @return NONE if there is not a registered phone number.
	 * @return phoneNumber if there is.
	 */
	public String getPhoneNumber()
	{
		if(phoneNumber!=null)
			return phoneNumber;
		
		return String.format("NONE");
	}
	
	/**
	 * @return NONE if there is not a registered address.
	 * @return address if there is.
	 */
	public String getAddress()
	{
		if(address!=null)
			return address;
		
		return String.format("NONE");
	}
	
	//Setters
	/**
	 * This method is used when the status is new. 
	 * Checks if the email info was registered from before.
	 * Sets the email if the email info was not registered from before.
	 * @param newMail is a String that will be set as the email information.
	 */
	protected void setEmail(String newMail)
	{
		try
		{
			//Checks if the email was registered from before in CustomerInfo.txt
			File infoFile = new File("CustomersInfo.txt");
			Scanner fileSC= new Scanner(infoFile);
			Scanner inSC= new Scanner(System.in);
			String mailInput=new String(newMail);
			while(fileSC.hasNext())
			{
				String readWord=new String(fileSC.next());
				if(readWord.equals(mailInput))
				{
					//Asks for another email, if it already exists.
					System.out.println("This email already exists. Enter another email.");
					mailInput=new String(inSC.next());
					fileSC.close();	
					fileSC= new Scanner(infoFile);
				}
			}
			//Registers the proper email.
			email=new String(mailInput);
			fileSC.close();	
		}
		catch(FileNotFoundException exc)
		{
			System.out.println("The file does not exist.");
			System.exit(-1);
		}
	}
	
	//Other Functions
	/**
	 * Saves the customer info to CustomersInfo.txt.
	 */
	private void saveCustomer()
	{
		try
		{
			FileWriter fw = new FileWriter("CustomersInfo.txt", true);
			fw.write("\n-\n"+getEmail()+" "+getName()+" "+getSurname()+" "+getPassword()+" "+getCustomerNo());
			fw.write("\nPastOrders");
			fw.write("\nNewOrders");
			fw.close();
		}
		catch(IOException ioe)
		{
			System.err.println("IOException: " + ioe.getMessage());
			System.exit(-1);
		}
	}
	
	/**
	 * Writes the customer name, surname and customer number.
	 */
	public String toString()
	{
		return String.format(getName()+" "+getSurname()+" "+getCustomerNo());
	}
	
	/**
	 * Checks if the given email information was registered from before.
	 * @param mailInfo is a String that holds and email information that will be searched if it was registered from before.
	 * @throws InvalidStatus if the given email information was already registered when the status was new, or the email was not registered when the status was old.
	 */
	protected void checkEmail(String mailInfo)throws InvalidStatus
	{
		try
		{
			File infoFile = new File("CustomersInfo.txt");
			Scanner fileSC= new Scanner(infoFile);
			while(fileSC.hasNext())
			{
				String readWord=new String(fileSC.next());
				if(readWord.equals(mailInfo))
					break;
				readWord=new String(fileSC.nextLine());
			}
			if(!fileSC.hasNext())
			{
				fileSC.close();
				throw new InvalidStatus("There is not an already existing customer with this email address.");
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
	 * Deletes the employee from the BranchEmployeesInfo.txt file.
	 */
	public void removeCustomer()
	{
		try 
        {  
			File inputFile = new File("CustomersInfo.txt");
			File outputFile = new File("temp.txt");

            Scanner sc = new Scanner(inputFile);
            FileWriter fw = new FileWriter(outputFile);
            String temp;

            fw.write(sc.nextLine());
            sc.next();
            while(sc.hasNextLine())
            {
	            temp=sc.next();
	            if(!temp.equals(getEmail()))
	            {
	            	fw.write("\n-");
	            	fw.write("\n"+temp);
	            	temp=sc.nextLine();
            		fw.write(temp);
	            	while(sc.hasNextLine() && !temp.equals("-"))
	            	{
	            		temp=sc.nextLine();
	            		if(!temp.equals("-"))
	            			fw.write("\n"+temp);
	            	}
	            }
	            else
	            	while(sc.hasNextLine() && !temp.equals("-"))
	            		temp=sc.nextLine();
   	
            }
            
            sc.close();
            fw.close();
            sc=null;
            System.gc();
            inputFile.delete();
            outputFile.renameTo(inputFile);

        } 
        catch (FileNotFoundException e2) 
        {
            e2.printStackTrace();
        } 
        catch (IOException e1) 
        {
            e1.printStackTrace();
        }
	}
	
	/**
	 * Loads the orders of the costumer that were saved from before .
	 */
	private void loadOrders()
	{
		String colourInfo;
		int modelInfo;
		String Stemp;
		try
		{
			File infoFile = new File("CustomersInfo.txt");
			Scanner fileSC= new Scanner(infoFile);
			do
			{/*Intentionally left empty*/}
			while(!fileSC.next().equals(getEmail()));
			fileSC.nextLine();
			fileSC.nextLine();
			while(fileSC.hasNextLine())
			{
				Stemp=new String(fileSC.next());
				if(Stemp.equals("NewOrders"))
					break;
				modelInfo=Integer.parseInt(fileSC.next());
				colourInfo=new String(fileSC.next());
				Product temp=new Product(Stemp, modelInfo, colourInfo);
				Stemp=new String(fileSC.next());
				temp.setStoreInfo(Integer.parseInt(Stemp));
				storePastOrder(temp);
			}
			while(fileSC.hasNextLine())
			{	
				Stemp=new String(fileSC.next());
				if(Stemp.equals("-"))
					break;
				modelInfo=Integer.parseInt(fileSC.next());
				colourInfo=new String(fileSC.next());
				Product temp=new Product(Stemp, modelInfo, colourInfo);
				Stemp=new String(fileSC.next());
				temp.setStoreInfo(Integer.parseInt(Stemp));
				storeNewOrder(temp);
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
	 * Stores the given product in pastOrders array field of the class
	 * @param addedProduct is a Product that will be added as a past order of the costumer
	 */
	private void storePastOrder(Product addedProduct)
	{
		try
		{
			if(pastOrders==null)
				pastOrders=new HybridList<Product>();
				
			pastOrders.add((Product)addedProduct.clone());

		}
		catch(CloneNotSupportedException exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
	}
	
	/**
	 * Stores the given product newOrders array field of the class.
	 * @param addedProduct is a Product that will be added as a new order of the costumer.
	 */
	private void storeNewOrder(Product addedProduct)
	{
		try
		{
			if(newOrders==null)
				newOrders=new HybridList<Product>();

			newOrders.add((Product)addedProduct.clone());
		}
		catch(CloneNotSupportedException exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
	}
	
	/**
	 * Adds the given product as a new order.
	 * @param addedProduct is the product that will be added as a new order.
	 * @param storeNo is the store number that the product is ordered from.
	 */
	public void addNewOrder(Product addedProduct, int storeNo)
	{
		try
		{	
			File inputFile = new File("CustomersInfo.txt");
			File outputFile = new File("temp.txt");
	
	        Scanner sc = new Scanner(inputFile);
	        FileWriter fw = new FileWriter(outputFile);
	        String sTemp;
	        
	        fw.write(sc.nextLine());
            do 
            {
            	fw.write("\n"+sc.nextLine());
            }
            while(!sc.hasNext(getEmail()));
            
            fw.write("\n"+sc.nextLine());
            do
            {
            	sTemp=new String(sc.nextLine());
            	fw.write("\n"+sTemp);
            }
            while(!sTemp.equals("NewOrders"));
            fw.write("\n"+addedProduct.toString()+" "+storeNo);
            while(sc.hasNextLine())
            	fw.write("\n"+sc.nextLine());
            
            sc.close();
            fw.close();
            sc=null;
            System.gc();
            inputFile.delete();
            outputFile.renameTo(inputFile);
		}
		catch (FileNotFoundException e2) 
        {
            e2.printStackTrace();
        } 
        catch (IOException e1) 
        {
            e1.printStackTrace();
        }
		
		storeNewOrder(addedProduct);
	}
	
	/**
	 * Adds the given product as a past order.
	 * @param addedProduct is the product that will be added as a past order.
	 * @param storeNo is the store number that the product is ordered from.
	 */
	public void addPastOrder(Product addedProduct, int storeNo)
	{
		try
		{
			File inputFile = new File("CustomersInfo.txt");
			File outputFile = new File("temp.txt");
	
	        Scanner sc = new Scanner(inputFile);
	        FileWriter fw = new FileWriter(outputFile);

	        fw.write(sc.nextLine());
            do 
            {
            	fw.write("\n"+sc.nextLine());
            }
            while(!sc.hasNext(getEmail()));
            fw.write("\n"+sc.nextLine());
            fw.write("\n"+sc.nextLine());
            fw.write("\n"+addedProduct.toString()+" "+storeNo);
            while(sc.hasNextLine())
            	fw.write("\n"+sc.nextLine());
            
            sc.close();
            fw.close();
            sc=null;
            System.gc();
            inputFile.delete();
            outputFile.renameTo(inputFile);
		}
		catch (FileNotFoundException e2) 
        {
            e2.printStackTrace();
        } 
        catch (IOException e1) 
        {
            e1.printStackTrace();
        }
		
		storePastOrder(addedProduct);
	}
	
	/**
	 * Removes the given product from the new orders of the customer if the product exists in the new orders section. 
	 * @param removedProduct is the product that will be removed from the new orders of the customer.
	 */
	public void removeNewOrder(Product removedProduct)
	{
		try
		{
			int counter=0;

			//Firstly removes from the newOrders array.
			if(newOrders==null || newOrders.size()==0)
			{
				System.out.println("There is no registered order that can be removed.");
				return;
			}
			else if(isNewOrder(removedProduct)==-1)
				return;
			else
			{
				for(int i=0; i<newOrders.size(); ++i)
					if(newOrders.get(i).equals(removedProduct))
						newOrders.remove(i);
			}		

			//Then removes from the CustomerInfo.txt file
			File inputFile = new File("CustomersInfo.txt");
			File outputFile = new File("temp.txt");
	
	        Scanner sc = new Scanner(inputFile);
	        FileWriter fw = new FileWriter(outputFile);
	        String sTemp;
	        String typeInfo;
	        int modelInfo;
	        String colourInfo;
	        int storeInfo;
	        counter=0;
	        
	        fw.write(sc.nextLine());
            do 
            {
            	fw.write("\n"+sc.nextLine());
            }
            while(!sc.hasNext(getEmail()));
            
            fw.write("\n"+sc.nextLine());
            do
            {
            	sTemp=new String(sc.nextLine());
            	fw.write("\n"+sTemp);
            }
            while(!sTemp.equals("NewOrders"));
           
            while(sc.hasNextLine() && !sc.hasNext("-"))
            {
	            typeInfo=new String(sc.next());
	            modelInfo=Integer.parseInt(sc.next());
	            colourInfo=new String(sc.next());
	            storeInfo=Integer.parseInt(sc.next());
	            	
	            if(typeInfo.equals(removedProduct.getType())==true && modelInfo==removedProduct.getModel() && colourInfo.equals(removedProduct.getColour())==true &&counter==0)
	            {
	            	counter++;
	            }
	            else
	            {
	            	fw.write("\n"+typeInfo+" ");
	            	fw.write(" "+modelInfo);
	            	fw.write(" "+colourInfo);
	            	fw.write(" "+storeInfo);
	            }
            }
            if(sc.hasNextLine())
            	sc.nextLine();
            while(sc.hasNextLine())
            	fw.write("\n"+sc.nextLine());
            
            sc.close();
            fw.close();
            sc=null;
            System.gc();
            inputFile.delete();
            	
            outputFile.renameTo(inputFile);
	        
		}
		catch (FileNotFoundException e2) 
        {
            e2.printStackTrace();
        } 
        catch (IOException e1) 
        {
            e1.printStackTrace();
        }
	}
	
	/**
	 * Registers the address and the phoneNumber of the customer.
	 * @param addressInfo is a String that will be registered as address.
	 * @param phoneInfo is a String that will be registered as phoneNumber.
	 */
	public void addInfo(String addressInfo, String phoneInfo)
	{
		address=new String(addressInfo);
		phoneNumber=new String(phoneInfo);
	}
	
	/**
	 * Saves and registers the address and the phoneNumber of the customer to CustomersInfo.txt.
	 * @param addressInfo is a String that will be registered as address.
	 * @param phoneInfo is a String that will be registered as phoneNumber.
	 */
	public void saveInfo(String addressInfo, String phoneInfo)
	{
		addInfo(addressInfo, phoneInfo);
		
		try
		{			
			File inputFile = new File("CustomersInfo.txt");
			File outputFile = new File("temp.txt");
	
	        Scanner sc = new Scanner(inputFile);
	        FileWriter fw = new FileWriter(outputFile);
	        
	        fw.write(sc.nextLine());
            do 
            {
            	fw.write("\n"+sc.nextLine());
            }
            while(!sc.hasNext(getEmail()));
            
            fw.write("\n"+sc.nextLine());
            fw.write(" "+getPhoneNumber());
            fw.write(" "+getAddress());
            
            while(sc.hasNextLine())
            	fw.write("\n"+sc.nextLine());
            
            sc.close();
            fw.close();
            sc=null;
            System.gc();
            inputFile.delete();
            outputFile.renameTo(inputFile);
            
		}
		catch (FileNotFoundException e2) 
        {
            e2.printStackTrace();
        } 
        catch (IOException e1) 
        {
            e1.printStackTrace();
        }
	}
	
	/**
	 * Prints out all the past orders. If there is none, prints No Past Orders.
	 */
	public void printPastOrders()
	{
		if(pastOrders!=null)
			for(int i=0; i<pastOrders.size(); ++i)
				System.out.println(pastOrders.get(i));
		else
			System.out.println("No Past Orders");
	}
	
	/**
	 * Prints out all the new orders. If there is none, prints No New Orders.
	 */
	public void printNewOrders()
	{
		if(newOrders!=null)
			for(int i=0; i<newOrders.size(); ++i)
				System.out.println(newOrders.get(i));
		else
			System.out.println("No New Orders");
	}
	
	/**
	 * Makes a copy of the customer.
	 */
	public Object clone() throws CloneNotSupportedException
	{
		Customer temp=(Customer)super.clone();
		temp.status=new String("old");
		
		if(pastOrders!=null)
		{
			temp.pastOrders=new HybridList<Product>();
			for(int i=0; i<pastOrders.size(); ++i)
				temp.pastOrders.add((Product)pastOrders.get(i).clone());
		}
		if(newOrders!=null)
		{
			temp.newOrders=new HybridList<Product>();
			for(int i=0; i<newOrders.size(); ++i)
				temp.newOrders.add((Product)newOrders.get(i).clone());
		}
		
		return temp;
	}
	
	/**
	 * @param searchedProduct is the product that will be searched among the past orders
	 * @return the index of the product if it exists inside the pastOrders array.
	 * @return -1 if it does not exist.
	 */
	public int isPastOrder(Product searchedProduct)
	{
		if(pastOrders!=null)
			for(int i=0; i<pastOrders.size(); ++i)
				if(pastOrders.get(i).equals(searchedProduct))
					return i;
		return -1;
	}
	
	/**
	 * @param searchedProduct is the product that will be searched among the new orders
	 * @return the index of the product if it exists inside the newOrders array.
	 * @return -1 if it does not exist.
	 */
	public int isNewOrder(Product searchedProduct)
	{
		if(newOrders!=null)
			for(int i=0; i<newOrders.size(); ++i)
				if(newOrders.get(i).equals(searchedProduct))
					return i;
		return -1;
	}
}
