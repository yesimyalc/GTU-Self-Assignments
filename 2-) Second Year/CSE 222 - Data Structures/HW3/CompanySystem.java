import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

/**
*A class that represents the automation system of the a company.
*It also stores the stores, admins, customers, employees of the company.
*Admins, employees and customers can use it for different purposes.
*/
public class CompanySystem 
{
	//Fields
	private LList<Store> StoreList=null;
	private KWArrayList<Admin> AdminList=null;
	private KWArrayList<Customer> CustomerList=null;
	private KWArrayList<BranchEmployee> EmployeeList=null;
	private HybridList<Product> SupplyList=null;
	private String loggedInStatus=null;
	private String loggedInEmail=null;
	
	//Constructors
	/**
	 * Constructor that sets the stores, admins, employees, customers and the supply list of a company.
	 * If this company is created for the first time, it automaticaly creates 4 branches, 1 online store.
	 * Reads the stores, admins, employees, customers and the supply list information from their files and sets them.
	 */
	public CompanySystem()
	{
		try
		{
			File inputFile = new File("BranchCount.txt");
			Scanner sc=new Scanner(inputFile);
			
			int branchCount=Integer.parseInt(sc.next());
			StoreList=new LList<Store>();
			StoreList.addFirst(new OnlineStore());
			for(int i=1; i<branchCount+1; ++i)
				StoreList.add(i, new Branch());
			
			loadAdmins();
			loadEmployees();
			loadCustomers();
			
			sc.close();
			
			inputFile = new File("SupplyList.txt");
			if(inputFile.exists())
				loadSupplyList();
			
		}
		catch (FileNotFoundException e2) 
        {
            e2.printStackTrace();
        } 
	}
	//Getters
	/**
	 * @return the loggedInStatus. Logged in status is determined according to who is using the system. Can be admin, employee or customer.
	 */
	public String getLoggedInStatus() {return loggedInStatus;}
	
	/**
	 * @return the amount of stores the company has.
	 */
	public int getStoreAmount() {return StoreList.size();}
	
	/** 
	 * @return the amount of supplies listed in the Supply List.
	 */
	public int getSupplyAmount()
	{
		if(SupplyList==null)
			return 0;
		return SupplyList.size();
	}
	
	/**
	 * @param index will be the index of the supply that will be returned from the SupplyList. It cannot be smaller than 0 or bigger than the supply amount.
	 * @return the supply at the given index from the SupplyList.
	 * @throws InvalidIndex if the index does not meet the requirements.
	 */
	public Product getSupplyList(int index)throws InvalidIndex
	{
		if(index<0 || index>SupplyList.size()-1)
			throw new InvalidIndex("There is no supply with this index.");
		
		return SupplyList.get(index);
	}
	
	/**
	 * @param index will be the index of the store that will be returned from the StoreList. It cannot be smaller than 0 or bigger than the store amount.
	 * @return the store at the given index from the StoreList.
	 * @throws InvalidIndex if the index does not meet the requirements.
	 */
	public Store getStore(int index)throws InvalidIndex
	{
		if(index<0 || index>StoreList.size()-1)
			throw new InvalidIndex("There is no store with this index.");
		
		return StoreList.get(index);
	}
	
	/**
	 * @return the amount of customers
	 */
	public int getCustomerAmount() {return CustomerList.size();}
	
	/**
	 * @param index will be the index of the customer that will be returned from the CustomerList. It cannot be smaller than 0 or bigger than the customer amount.
	 * @return the customer at the given index from the CustomerList.
	 * @throws InvalidIndex if the index does not meet the requirements.
	 */
	public Customer getCustomer(int index)throws InvalidIndex
	{
		if(index<0 || index>CustomerList.size()-1)
			throw new InvalidIndex("There is no customer with this index.");
		
		return CustomerList.get(index);
	}
	
	//Other Functions
	/**
	 * Logs into the system with the given user information.
	 * This method is to determine who is using the system and with the proper informations.
	 * If the user was registered from before but trying to enter the system with wrong information, the information will be asked again.
	 * The things that a user changes according to if they are admin, employee or a customer.
	 * Saves the email and the status(admin/costumer/employee) of the user who logs in.
	 * If the user was not registered from before, registers the user as a new customer.
	 */
	public void enterSystem()
	{
		String mailInfo;
		String nameInfo;
		String surnameInfo;
		String passInfo;
		Scanner sc=new Scanner(System.in);
		System.out.println("Enter Email:");
		mailInfo=new String(sc.next());
		
		if(isAdmin(mailInfo)!=-1)
			loggedInStatus=new String("Admin");
		else if(isEmployee(mailInfo)!=-1)
			loggedInStatus=new String("Employee");
		else if(isCustomer(mailInfo)!=-1)
			loggedInStatus=new String("Customer");
		else
		{
			System.out.println("You can register the system by entering a password, your name and surname.");
			System.out.println("Enter your password:");
			passInfo=new String(sc.next());
			System.out.println("Enter your name:");
			nameInfo=new String(sc.next());
			System.out.println("Enter your surname:");
			surnameInfo=new String(sc.next());
				
			Customer newCostumer=new Customer(nameInfo, surnameInfo, mailInfo, passInfo, "new");
			storeCustomer(newCostumer);
			loggedInStatus=new String("Customer");
			System.out.println("The registration is completed. Your customer number is: "+String.valueOf(newCostumer.getCustomerNo()));
		}
			
		System.out.println("Enter your password:");
		passInfo=new String(sc.next());
		while(loggedInStatus.equals("Admin") && !AdminList.get(isAdmin(mailInfo)).getPassword().equals(passInfo))
		{
			System.out.println("You have entered a wrong password. Try again:");
			passInfo=new String(sc.next());
		}
		while(loggedInStatus.equals("Employee") && !EmployeeList.get(isEmployee(mailInfo)).getPassword().equals(passInfo))
		{
			System.out.println("You have entered a wrong password. Try again:");
			passInfo=new String(sc.next());
		}
		while(loggedInStatus.equals("Customer") && !CustomerList.get(isCustomer(mailInfo)).getPassword().equals(passInfo))
		{
			System.out.println("You have entered a wrong password. Try again:");
			passInfo=new String(sc.next());
		}
		loggedInEmail=new String(mailInfo);
	}
	
	/**
	 * Makes a sale for a customer who comes to a branch and wants to make a purchase.
	 * The purchase can be made from a branch only, online store does not count.
	 * If the product is in the stock of the store, the purchase is done instantly. If not, it is registered as a new order for the customer.
	 * Used by the branch employees.
	 * @param no is the number of the customer that will make a purchase.
	 */
	public void makeSale(int no)
	{
		Scanner sc=new Scanner(System.in);
		System.out.println("Enter the type of the product customer wants to buy:");
		String typeInfo=sc.next();
		System.out.println("Enter the model of the product customer wants to buy:");
		int modelInfo=sc.nextInt();
		System.out.println("Enter the colour of the product customer wants to buy:");
		String colourInfo=sc.next();
		System.out.println("Enter the branch number customer wants to buy from.");
		int storeNo=sc.nextInt();
		while(storeNo<1 || storeNo>=StoreList.size())
		{
			System.out.println("There is no branch with this number, please enter again.");
			storeNo=sc.nextInt();
		}
		Product boughtProduct=new Product(typeInfo, modelInfo, colourInfo);
		boughtProduct.setStoreInfo(storeNo);
		
		for(int i=0; i<CustomerList.size(); ++i)
			if(CustomerList.get(i).getCustomerNo()==no)
			{
				if(StoreList.get(storeNo).isInStock(boughtProduct)==-1)
				{
					CustomerList.get(i).addNewOrder(boughtProduct, storeNo);
					addSupply(boughtProduct);
					System.out.println("You have successfuly done your order from Branch "+storeNo+". Your order will be delivered as soon as it gets into stock.");
				}
				else
				{
					CustomerList.get(i).addPastOrder(boughtProduct, storeNo);
					StoreList.get(storeNo).changeStock(boughtProduct, -1);
					System.out.println("You have successfuly done your purchase from Branch "+storeNo+".");
				}
			}
	}
	
	/**
	 * Makes a purchase for the customer who is logged in. The purchase can be made from a branch or the online store.
	 * If the product is in the stock of the store, the purchase is done instantly if the store is a branch. If the store is the online store,
	 * then the purchase is added as a new order.
	 * If the product is not in the stock of the store, it is asked to order from another store or the purchase is added as a new order. 
	 */
	public void shopMenu()
	{
		Scanner sc=new Scanner(System.in);
			
		System.out.println("Enter the type of the product you want to buy:");
		String typeInfo=sc.next();
		System.out.println("Enter the model of the product you want to buy:");
		int modelInfo=sc.nextInt();
		System.out.println("Enter the colour of the product you want to buy:");
		String colourInfo=sc.next();
		System.out.println("Enter the store number you want to buy from. For Online Store enter 0.");
		int storeNo=sc.nextInt();
		while(storeNo<0 || storeNo>StoreList.size()-1)
		{
			System.out.println("There is no store with this number, please enter again.");
			storeNo=sc.nextInt();
		}
		int temp=-1;
		Product boughtProduct=new Product(typeInfo, modelInfo, colourInfo);
		while(StoreList.get(storeNo).isInStock(boughtProduct)==-1 && temp!=storeNo)
		{
			temp=storeNo;
			System.out.println("\nCurrently this product is not in the stock of the store you want to buy from.");
			System.out.println("You can make your order and wait for the stock to be updated or you can purchase it from other stores.");
			System.out.println("This product can be found in these stores' stocks:");
			searchProduct(boughtProduct);
			System.out.println("\nPlease enter the store number you want to buy from again. You can also enter the same number to proceed with your order.");
			storeNo=sc.nextInt();
			
			if(temp==storeNo)
			{
				boughtProduct.setStoreInfo(storeNo);
				for(int i=0; i<CustomerList.size(); ++i)
					if(CustomerList.get(i).getEmail().equals(loggedInEmail))
						CustomerList.get(i).addNewOrder(boughtProduct, storeNo);
				System.out.println("You have successfuly done your order from Branch "+storeNo+". Your order will be delivered as soon as it gets into stock.");
			}
		}
		boughtProduct.setStoreInfo(storeNo);
		if(storeNo!=0 && temp!=storeNo)
		{
			for(int i=0; i<CustomerList.size(); ++i)
				if(CustomerList.get(i).getEmail().equals(loggedInEmail))
					CustomerList.get(i).addPastOrder(boughtProduct, storeNo);
			StoreList.get(storeNo).changeStock(boughtProduct, -1);
			System.out.println("You have successfuly done your purchase from Branch "+storeNo+".");
		}
		else if(temp!=storeNo)
		{
			for(int i=0; i<CustomerList.size(); ++i)
			{
				if(CustomerList.get(i).getEmail().equals(loggedInEmail) && CustomerList.get(i).getPhoneNumber()=="NONE")
				{
					System.out.println("You need to register your address and phone number to do shopping.");
					String phoneInfo=sc.next();
					String addressInfo=sc.next();
					CustomerList.get(i).addInfo(addressInfo, phoneInfo);
				}
			}
			
			for(int i=0; i<CustomerList.size(); ++i)
				if(CustomerList.get(i).getEmail().equals(loggedInEmail))
					CustomerList.get(i).addNewOrder(boughtProduct, storeNo);
			StoreList.get(storeNo).changeStock(boughtProduct, -1);
			System.out.println("You have successfuly done your purchase from the online store. Your purchase will be delievered as soon as possible.");
		}

	}
	
	/**
	 * Arranges all the sales. 
	 * If the product is in the stock, decreases the stock, changes the new order as a past order to show the purchase is done
	 * If the product is not in the stock, adds the product to the supply list to be able to supply it.
	 * Used by the branch employees.
	 */
	public void salesMenu()
	{
		try
		{
			for(int i=0; i<CustomerList.size(); ++i)
			{
				for(int j=0; j<CustomerList.get(i).getNewOrderCount(); ++j)
				{
					Product sale=(Product)CustomerList.get(i).getNewOrder(j).clone();
					if(StoreList.get(sale.getStoreInfo()).isInStock(sale)==-1)
						addSupply(sale);
					else
					{
						CustomerList.get(i).removeNewOrder(sale);
						CustomerList.get(i).addPastOrder(sale, sale.getStoreInfo());
						StoreList.get(sale.getStoreInfo()).changeStock(sale, -1);
						j--;
					}
				}
			}
		}
		catch(InvalidIndex exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
		catch(CloneNotSupportedException exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
	}
	
	/**
	 * Loads the admins from the AdminsInfo.txt file.
	 */
	private void loadAdmins()
	{
		try
		{
			String mailInfo;
			String nameInfo;
			String surnameInfo;
			String passInfo;
			Admin atemp;
			File inputFile = new File("AdminsInfo.txt");
			Scanner sc=new Scanner(inputFile);
			sc.nextLine();
			sc.nextLine();
			while(sc.hasNextLine())
			{
				mailInfo=new String(sc.next());
				nameInfo=new String(sc.next());
				surnameInfo=new String(sc.next());
				passInfo=new String(sc.next());
				atemp=new Admin(nameInfo, surnameInfo, mailInfo, passInfo, "old");
				storeAdmin(atemp);
			}
			sc.close();
		}
		catch (FileNotFoundException e2) 
        {
            e2.printStackTrace();
        } 
	}
	
	/**
	 * Loads the customer from the CustomersInfo.txt file.
	 */
	private void loadCustomers()
	{
		try
		{
			String mailInfo;
			String nameInfo;
			String surnameInfo;
			String passInfo;
			String phoneInfo;
			String addressInfo;
			String stemp;
			Customer ctemp;
			File inputFile = new File("CustomersInfo.txt");
			Scanner sc=new Scanner(inputFile);
			sc.nextLine();
			sc.nextLine();
			while(sc.hasNextLine())
			{
				mailInfo=new String(sc.next());
				nameInfo=new String(sc.next());
				surnameInfo=new String(sc.next());
				passInfo=new String(sc.next());
				ctemp=new Customer(nameInfo, surnameInfo, mailInfo, passInfo, "old");
				sc.next();
				stemp=new String(sc.next());
				if(!stemp.equals("PastOrders"))
				{
					phoneInfo=new String(stemp);
					addressInfo=new String(sc.next());
					ctemp.addInfo(phoneInfo, addressInfo);
				}
				storeCustomer(ctemp);
				do
				{
					if(sc.hasNextLine())
						stemp=new String(sc.next());
				}
				while(!stemp.equals("-") && sc.hasNextLine());
				
				if(sc.hasNextLine())
					sc.nextLine();
			}
			sc.close();
		}
		catch (FileNotFoundException e2) 
        {
            e2.printStackTrace();
        } 
	}
	
	/**
	 * Loads the employees from the BranchEmployeesInfo.txt file.
	 */
	private void loadEmployees()
	{
		try
		{
			String mailInfo;
			String nameInfo;
			String surnameInfo;
			String passInfo;
			BranchEmployee etemp;
			File inputFile = new File("BranchEmployeesInfo.txt");
			Scanner sc=new Scanner(inputFile);
			sc.nextLine();
			sc.nextLine();
			while(sc.hasNextLine())
			{
				mailInfo=new String(sc.next());
				nameInfo=new String(sc.next());
				surnameInfo=new String(sc.next());
				passInfo=new String(sc.next());
				etemp=new BranchEmployee(nameInfo, surnameInfo, mailInfo, passInfo, "old");
				storeEmployee(etemp);
			}
			sc.close();
		}
		catch (FileNotFoundException e2) 
        {
            e2.printStackTrace();
        } 
	}
	
	/**
	 * Loads the supplies that need to be supplied from the SupplyList.txt file.
	 */
	private void loadSupplyList()
	{
		try
		{
			File inputFile = new File("SupplyList.txt");
			Scanner sc=new Scanner(inputFile);
			String typeInfo;
			int modelInfo;
			String colourInfo;
			int storeInfo;
			Product temp;
			sc.nextLine();
			while(sc.hasNextLine())
			{
				typeInfo=new String(sc.next());
				modelInfo=Integer.parseInt(sc.next());
				colourInfo=new String(sc.next());
				temp=new Product(typeInfo, modelInfo, colourInfo);
				storeInfo=Integer.parseInt(sc.next());
				temp.setStoreInfo(storeInfo);
				storeSupply(temp);
			}
		}
		catch (FileNotFoundException e2) 
        {
            e2.printStackTrace();
        } 
	}
	
	/**
	 * Adds admins to the Admins array.
	 * @param addedAdmin is an admin that will be added to the Admins array.
	 */
	public void storeAdmin(Admin addedAdmin)
	{
		if(AdminList==null)
			AdminList=new KWArrayList<Admin>();
		else if(isAdmin(addedAdmin.getEmail())!=-1)
			return;
	
		AdminList.add(addedAdmin);
	}
	
	/**
	 * Searchs for an admin in the Admins array.
	 * @param searchedAdmin is an Admin that will be searched in the Admins array.
	 * @return the index of the Admin who is being searched in the admins array.
	 * @return -1 if the admin is not found.
	 */
	public int isAdmin(String searchedAdmin)
	{
		if(AdminList!=null)
			for(int i=0; i<AdminList.size(); ++i)
				if(AdminList.get(i).getEmail().equals(searchedAdmin))
					return i;

		return -1;
	}
	
	/**
	 * Prints out all the admins.
	 */
	public void printAdmins()
	{
		if(AdminList!=null)
			for(int i=0; i<AdminList.size(); ++i)
				System.out.println(AdminList.get(i));
	}
	
	/**
	 * Adds customers to the Customers array.
	 * @param addedCustomer is a customer that will be added to the Customers array.
	 */
	public void storeCustomer(Customer addedCustomer)
	{
		if(CustomerList==null)
			CustomerList=new KWArrayList<Customer>();
		else if(isCustomer(addedCustomer.getEmail())!=-1)
			return;

		CustomerList.add(addedCustomer);
	}
	
	/**
	 * Searchs for an customer in the Customers array.
	 * @param searchedCustomer is a Customer that will be searched in the Customers array.
	 * @return the index of the Customer who is being searched in the Customers array.
	 * @return -1 if the customer is not found.
	 */
	public int isCustomer(String addedCustomer)
	{
		if(CustomerList!=null)
			for(int i=0; i<CustomerList.size(); ++i)
				if(CustomerList.get(i).getEmail().equals(addedCustomer))
					return i;
		return -1;
	}
	
	/**
	 * Prints out all the customers.
	 */
	public void printCustomers()
	{
		if(CustomerList!=null)
			for(int i=0; i<CustomerList.size(); ++i)
				System.out.println(CustomerList.get(i));
	}
	
	/**
	 * Adds a new employee to the EmployeeList array.
	 * @param addedEmployee is a BranchEmployee who will be added to the EmployeeList array.
	 */
	public void storeEmployee(BranchEmployee addedEmployee)
	{

			if(EmployeeList==null)
				EmployeeList=new KWArrayList<BranchEmployee>();
			else if(isEmployee(addedEmployee.getEmail())!=-1)
				return;

			EmployeeList.add(addedEmployee);
	}
	
	/**
	 * Removes an employee from the EmployeeList.
	 * @param removedEmployee is a BranchEmployee who will be removed from the EmployeeList array.
	 * If the removedEmployee cannot be found, returns.
	 */
	public void removeEmployee(BranchEmployee removedEmployee)
	{
		try
		{
			if(EmployeeList==null || EmployeeList.size()==0)
			{
				System.out.println("There is no registered employee that can be removed.");
				return;
			}
			else if(isEmployee(removedEmployee.getEmail())==-1)
				return;
			else
			{
				removedEmployee.deleteEmployee();
				for(int i=0; i<EmployeeList.size(); ++i)
					if(EmployeeList.get(i).getEmail().equals(removedEmployee.getEmail()))
					{
						EmployeeList.remove(i);
						break;
					}
			}		
		}
		catch(InvalidIndex exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
	}
	
	/**
	 * Searchs for an employee in the EmployeeList array.
	 * @param searchedEmployee is an Employee that will be searched in the EmployeeList array.
	 * @return the index of the employee who is being searched in the EmployeeList array.
	 * @return -1 if the employee is not found.
	 */
	public int isEmployee(String searchedEmployee)
	{
		if(EmployeeList!=null)
			for(int i=0; i<EmployeeList.size(); ++i)
				if(EmployeeList.get(i).getEmail().equals(searchedEmployee))
					return i;
		return -1;
	}
	
	/**
	 * Prints out all the employees.
	 */
	public void printEmployees()
	{
		if(EmployeeList!=null)
			for(int i=0; i<EmployeeList.size(); ++i)
				System.out.println(EmployeeList.get(i));
	}
	
	/**
	 * Adds a new product to the SupplyList to be able to supply it later. 
	 * @param addedSupply is a Product that will be added to the SupplyList array.
	 */
	public void storeSupply(Product addedSupply)
	{
		if(SupplyList==null)
			SupplyList=new HybridList<Product>();
		
		SupplyList.add(addedSupply);
	}
	
	/**
	 * Saves the added supply to the SupplyList.txt file.
	 * @param addedSupply is a Product that will be saved to the SupplyList.txt.
	 */
	public void addSupply(Product addedSupply)
	{
		try
		{
			FileWriter fw = new FileWriter("SupplyList.txt", true);
			fw.write("\n"+addedSupply.getType()+" "+addedSupply.getModel()+" "+addedSupply.getColour()+" "+addedSupply.getStoreInfo());
			fw.close();
			
			storeSupply(addedSupply);
			System.out.println("Product has been added to the Supply List to inform the manager: "+addedSupply.getType()+" "+addedSupply.getModel()+" "+addedSupply.getColour());
		}
		catch(IOException ioe)
		{
			System.err.println("IOException: " + ioe.getMessage());
			System.exit(-1);
		}
	}
	
	/**
	 * Removes a product from the SupplyList array.
	 * @param removedSupply is a product that will be removed from the SupplyList array.
	 * If the product is not found in the SupplyList array, returns.
	 */
	public void removeSupply(Product removedSupply)
	{
		if(SupplyList==null || SupplyList.size()==0)
		{
			System.out.println("There is no registered supply that can be removed.");
			return;
		}
		else if(isSupply(removedSupply)==-1)
		{
			System.out.println("This product is not in the supply list.");
			return;
		}
		else
		{
		    String typeInfo;
		    int modelInfo;
		    int storeInfo;
		    String colourInfo;
		    
			
		    for(int i=0; i<SupplyList.size(); ++i)
				if(SupplyList.get(i).equals(removedSupply))
					SupplyList.remove(i);
		    
			try 
			{  
				int counter=0;
				File inputFile = new File("SupplyList.txt");
				File outputFile = new File("temp.txt");

			    Scanner sc = new Scanner(inputFile);
			    FileWriter fw = new FileWriter(outputFile);
			    while(sc.hasNextLine()) 
			    {
			        typeInfo=new String(sc.next());
			        modelInfo=Integer.parseInt(sc.next());
			        colourInfo=new String(sc.next());
			        storeInfo=Integer.parseInt(sc.next());
			            	
			        if(typeInfo.equals(removedSupply.getType())==true && modelInfo==removedSupply.getModel() && colourInfo.equals(removedSupply.getColour())==true && counter==0)
			        {
			           counter++;
			        }
			        else
			        {
			           fw.write("\n"+typeInfo);
			           fw.write(" "+modelInfo);
			           fw.write(" "+colourInfo);
			           fw.write(" "+storeInfo);
			        }
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
	}
	
	/**
	 * Searchs for a product in the SupplyList array.
	 * @param searchedProduct is a product that will be searched in the SupplyList array.
	 * @return the index of the product that is being searched in the SupplyList array.
	 * @return -1 if the product is not found.
	 */
	public int isSupply(Product searchedProduct)
	{
		if(SupplyList!=null)
			for(int i=0; i<SupplyList.size(); ++i)
				if(SupplyList.get(i).equals(searchedProduct))
					return i;
		return -1;
	}
	
	/**
	 * Prints out all the supplies in the Supplylist array.
	 */
	public void printSupplyList()
	{
		if(SupplyList!=null)
			for(int i=0; i<SupplyList.size(); ++i)
				System.out.println(SupplyList.get(i)+" to Store "+SupplyList.get(i).getStoreInfo());
	}
	
	/**
	 * Adds a new branch to the company.
	 * Increments the branch count in the BranchCount.txt
	 * @param addedBranch is the branch that will be added to the company.
	 */
	public void addBranch(Store addedBranch)
	{
		try
		{	
			StoreList.addLast(addedBranch);
					
			File inputFile = new File("BranchCount.txt");
			File outputFile = new File("temp.txt");

            Scanner sc = new Scanner(inputFile);
            FileWriter fw = new FileWriter(outputFile);

            fw.write(String.valueOf(StoreList.size()-1));

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
	 * Removes a branch from the StoreList. 
	 * Decreases the branch count from the BranchCount.txt.
	 * Changes all the other branchs' numbers according to the new branch count.
	 * If there was a new/past order from this store, sets them as if they were/will be done from the online store not to affect the customers' orders.
	 * Resets the SupplyList.
	 * @param branchNo is the index of the branch that will be removed from the company.
	 * @throws InvalidIndex if the branchNo does not meet the requirements.
	 */
	public void removeBranch(int branchNo)throws InvalidIndex
	{
		try
		{
			if(branchNo<1 || branchNo>StoreList.size()-1)
				throw new InvalidIndex("There is no branch with this number");
			else
			{
				File removedStock = new File("Branch"+branchNo+"Stock.txt");
				removedStock.delete();
				
				StoreList.remove(branchNo);
				for(int i=0; i<StoreList.size(); ++i)
					if(StoreList.get(i).getNo()>branchNo)
						StoreList.get(i).changeNo(StoreList.get(i).getNo()-1);
				
				Branch.totalBranchCount--;
				
				File inputFile = new File("BranchCount.txt");
				File outputFile = new File("temp.txt");

	            Scanner sc = new Scanner(inputFile);
	            FileWriter fw = new FileWriter(outputFile);

	            fw.write(String.valueOf(StoreList.size()-1));

	            sc.close();
	            fw.close();
	            sc=null;
	            System.gc();
	            inputFile.delete();
	            outputFile.renameTo(inputFile);
	            
	            File inputFile2 = new File("SupplyList.txt");
				if(inputFile2.exists())
				{
					SupplyList=null;
					inputFile2.delete();
				}
	            
				File inputFile3 = new File("CustomersInfo.txt");
				File outputFile3 = new File("temp3.txt");
	            Scanner sc3 = new Scanner(inputFile3);
	            FileWriter fw3 = new FileWriter(outputFile3);
	            
	            fw3.write(sc3.nextLine());
	            String stemp;
	            
	            while(sc3.hasNext())
	            {
	            	do
	            	{
	            		stemp=sc3.nextLine();
	            		fw3.write("\n"+stemp);
	            	}
	            	while(!stemp.equals("PastOrders"));
	            	while(!sc3.hasNext("NewOrders"))
	            	{
		            	String typeInfo=new String(sc3.next());
		            	int modelInfo=Integer.parseInt(sc3.next());
		            	String colourInfo=new String(sc3.next());
		            	int storeInfo=Integer.parseInt(sc3.next());
        	
		            	fw3.write("\n"+typeInfo);
				        fw3.write(" "+modelInfo);
				        fw3.write(" "+colourInfo);
				         
				        if(storeInfo==StoreList.size())
				        	fw3.write(" 0");
				        else
				        	fw3.write(" "+storeInfo);
	            	}
	        
			        fw3.write("\n"+sc3.next());
			        while(sc3.hasNext() && !sc3.hasNext("-"))
	            	{
		            	String typeInfo=new String(sc3.next());
		            	int modelInfo=Integer.parseInt(sc3.next());
		            	String colourInfo=new String(sc3.next());
		            	int storeInfo=Integer.parseInt(sc3.next());
		            	
		            	fw3.write("\n"+typeInfo);
				        fw3.write(" "+modelInfo);
				        fw3.write(" "+colourInfo);
				         
				        if(storeInfo==StoreList.size())
				        	fw3.write(" 0");
				        else
				        	fw3.write(" "+storeInfo);
	            	}
			        if(sc3.hasNextLine())
			        	sc3.nextLine();
	            }
	            sc3.close();
	            fw3.close();
	            sc3=null;
	            System.gc();
	            inputFile3.delete();
	            outputFile3.renameTo(inputFile3);
				
			}		
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
	 * Shows if there are any products that need to be supplied.
	 */
	public void querySupplyList()
	{
		if(SupplyList==null || SupplyList.size()==0)
		{
			System.out.println("There are no products that need to be supplied.");
		}
		else
		{
			System.out.println("These products should be supplied:");
			printSupplyList();
		}
	}
	
	/**
	 * Prints out all the products this company produces.
	 */
	public void printAllProducts()
	{
		System.out.println("All Products:");
		Product temp=new Product("Office_Chair", 1, "Black", 1);

		for(int i=1; i<=7; ++i)
			for(int j=0; j<5; ++j)
				System.out.println("Office_Chair "+i+" "+temp.getAvailableColour(j));
		
		temp=new Product("Office_Desk", 1, "Black", 1);
		for(int i=1; i<=5; ++i)
			for(int j=0; j<4; ++j)
				System.out.println("Office_Desk "+i+" "+temp.getAvailableColour(j));
		
		temp=new Product("Meeting_Table", 1, "Black", 1);
		for(int i=1; i<=10; ++i)
			for(int j=0; j<4; ++j)
				System.out.println("Meeting_Table "+i+" "+temp.getAvailableColour(j));
		
		temp=new Product("Bookcase", 1, "Black", 1);
		for(int i=1; i<=12; ++i)
			for(int j=0; j<1; ++j)
				System.out.println("Bookcase "+i+" "+temp.getAvailableColour(j));
		
		temp=new Product("Office_Cabinet", 1, "Black", 1);
		for(int i=1; i<=12; ++i)
			for(int j=0; j<1; ++j)
				System.out.println("Office_Cabinet "+i+" "+temp.getAvailableColour(j));
	}
	
	/**
	 * Serachs for a product in the stock of all the stores the company has.
	 * @param searchedProduct is the product that will be searched in the stocks of the all stores.
	 */
	public void searchProduct(Product searchedProduct)
	{
		try
		{
			for(int i=0; i<StoreList.size(); ++i)
			{
				if(StoreList.get(i).isInStock(searchedProduct)!=-1 && i!=0)
				{
					System.out.println(searchedProduct+" can be found in Branch "+StoreList.get(i).getNo()+".");
					System.out.println("There are "+(StoreList.get(i).getProduct(StoreList.get(i).isInStock(searchedProduct))).getAmount());
				}									
				else if(StoreList.get(i).isInStock(searchedProduct)!=-1)
				{
					System.out.println(searchedProduct+" can be found in the Online Store.");
					System.out.println("There are "+(StoreList.get(i).getProduct(StoreList.get(i).isInStock(searchedProduct))).getAmount());
				}
			}
		}
		catch(InvalidIndex exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
	}
	
	/**
	 * Shows the past orders of the customer who is logged in.
	 */
	public void seePastOrders()
	{
		for(int i=0; i<CustomerList.size(); ++i)
		{
			if(CustomerList.get(i).getEmail().equals(loggedInEmail))
			{
				System.out.println("Your Past Orders:");
				CustomerList.get(i).printPastOrders();
				break;
			}
		}
	}
	
	/**
	 * Shows the past orders of the customer with the given index as their customer number.
	 * @param customerNo is the number of the customer that will be searched in the CustomersList
	 */
	public void seePastOrders(int customerNo)
	{
		for(int i=0; i<CustomerList.size(); ++i)
		{
			if(CustomerList.get(i).getCustomerNo()==customerNo)
			{
				System.out.println("The Customer's Past Orders:");
				CustomerList.get(i).printPastOrders();
				break;
			}
			else if(i==CustomerList.size()-1)
				System.out.println("There is no customer with this customer number.");
		}
	}
}
