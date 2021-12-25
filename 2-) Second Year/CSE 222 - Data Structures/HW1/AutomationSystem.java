import java.io.File;
import java.util.Scanner;

public class AutomationSystem {

	public static void main(String[] args)
	{
		try
		{
			CompanySystem s1=new CompanySystem();
			s1.enterSystem();
			
			if(s1.getLoggedInStatus().equals("Admin"))
			{
				System.out.println("\n*******Admin Menu*******");
				System.out.println("\nCommands List:");
				System.out.println("ADDBranch: Creates a new branch for the company in the system.");
				System.out.println("REMOVEBranch: Removes a branch of the company in the system.");
				System.out.println("ADDEmployee: Adds a new employee for the company in the system.");
				System.out.println("REMOVEEmployee: Removes an employee of the company in the system.");
				System.out.println("CHECK: Checks what products should be supplied.");
				System.out.println("QUIT: Logs off.");
				System.out.println("\n*************************");
				Scanner sc=new Scanner(System.in);
				System.out.println("Enter command:");
				String command=sc.next();
				while(!command.equals("QUIT"))
				{
					if(command.equals("ADDBranch"))
					{
						Store temp=new Branch();
						s1.addBranch(temp);
						System.out.println("New branch is successfuly added.");
						System.out.println("\nEnter command:");
						command=sc.next();
					}
					else if(command.equals("REMOVEBranch"))
					{
						int no=-1;
						System.out.println("Enter the branch number you want to remove.");
						int temp=s1.getStoreAmount()-1;
						
						do
						{
							try
							{
								no=sc.nextInt();
								s1.removeBranch(no);
							}
							catch(InvalidIndex exc)
							{
								System.err.println(exc);
								System.out.println("Please enter again.");
							}
						}
						while(no<1 || no>temp);

						System.out.println("The branch is successfuly removed.");
						System.out.println("\nEnter command:");
						command=sc.next();
						
					}
					else if(command.equals("ADDEmployee"))
					{
						System.out.println("Enter the name of the employee you want to add:");
						String nameInfo=sc.next();
						System.out.println("Enter the surname of the employee you want to add:");
						String surnameInfo=sc.next();
						System.out.println("Enter the email of the employee you want to add:");
						String mailInfo=sc.next();
						System.out.println("Enter the password of the employee you want to add:");
						String passInfo=sc.next();
						BranchEmployee newEmployee=new BranchEmployee(nameInfo, surnameInfo, mailInfo, passInfo, "new");
						s1.storeEmployee(newEmployee);
						System.out.println("New employee is successfuly added.");
						System.out.println("\nEnter command:");
						command=sc.next();
	
					}
					else if(command.equals("REMOVEEmployee"))
					{
						System.out.println("Enter the name of the employee you want to remove:");
						String nameInfo=sc.next();
						System.out.println("Enter the surname of the employee you want to remove:");
						String surnameInfo=sc.next();
						System.out.println("Enter the email of the employee you want to remove:");
						String mailInfo=sc.next();
						System.out.println("Enter the password of the employee you want to remove:");
						String passInfo=sc.next();
						BranchEmployee oldEmployee=new BranchEmployee(nameInfo, surnameInfo, mailInfo, passInfo, "old");
						s1.removeEmployee(oldEmployee);
						System.out.println("The employee is successfuly removed.");
						System.out.println("\nEnter command:");
						command=sc.next();
					}
					else if(command.equals("CHECK"))
					{
						s1.querySupplyList();
						System.out.println("\nEnter command:");
						command=sc.next();
					}
					else if(!command.equals("QUIT"))
					{
						System.out.println("You have entered an invalid command. Please enter again.");
						command=sc.next();
					}
				}
			}
			else if(s1.getLoggedInStatus().equals("Employee"))
			{
				System.out.println("\n*******Employee Menu*******");
				System.out.println("\nCommands List:");
				System.out.println("STOCK: Shows the stock of a branch/online store or all.");
				System.out.println("ARRANGESales: Arrange the things related to sales.");
				System.out.println("ADDProduct: Adds a product to a branch's stock or to the online store's stock.");
				System.out.println("REMOVEProduct: Removes a product from a branch's stock or from the online store's stock.");
				System.out.println("SUPPLYProducts: Supplies the products that are needed to be supplied.");
				System.out.println("MAKESale: Makes a sale through branch.");
				System.out.println("PASTOrders: Shows the past orders of a customer.");
				System.out.println("QUIT: Logs off.");
				System.out.println("\n****************************");
				Scanner sc=new Scanner(System.in);
				System.out.println("Enter command:");
				String command=sc.next();
				while(!command.equals("QUIT"))
				{
					if(command.equals("STOCK"))
					{
						System.out.println("Do you want to see all stock or one stock of a store? Enter ALL or ONE.");
						command=sc.next();
						while(!command.equals("ONE") && !command.equals("ALL"))
						{
							System.out.println("You have entered a wrong command. Please enter again.");
							command=sc.next();
						}
						if(command.equals("ONE"))
						{
							System.out.println("If you want to see Online Store stock, enter 0. If you want to see a branch's stock, enter their branch number.");
							int no=sc.nextInt();
							while(no<0 || no>s1.getStoreAmount())
							{
								System.out.println("You have entered an invalid store no. Please enter again.");
								no=sc.nextInt();
							}
							s1.getStore(no).printStock();
						}
						else
						{
							try
							{
								int amount=0;
								for(int i=0; i<119; ++i)
								{
									for(int j=0; j<s1.getStoreAmount(); ++j)
									{
										amount+=s1.getStore(j).getProduct(i).getAmount();
									}
									System.out.println(s1.getStore(0).getProduct(i)+"\t"+amount);
									amount=0;
								}
							}
							catch(InvalidIndex exc)
							{
								System.err.print(exc);
								System.exit(-1);
							}
								
						}
						System.out.println("\nEnter command:");
						command=sc.next();
					}
					else if(command.equals("ARRANGESales"))
					{
						s1.salesMenu();
						System.out.println("All sales have been successfuly arranged.");
						System.out.println("\nEnter command:");
						command=sc.next();
					}
					else if(command.equals("ADDProduct"))
					{
						System.out.println("Enter the type of the product you want to add:");
						String typeInfo=sc.next();
						System.out.println("Enter the model of the product you want to add:");
						int modelInfo=sc.nextInt();
						System.out.println("Enter the colour of the product you want to add:");
						String colourInfo=sc.next();
						System.out.println("Enter how many you want to add:");
						int amount=sc.nextInt();
						System.out.println("Enter the store number you want to add. For Online Store enter 0.");
						int storeNo=sc.nextInt();
						while(storeNo<0 || storeNo>s1.getStoreAmount())
						{
							System.out.println("You have entered an invalid store no. Please enter again.");
							storeNo=sc.nextInt();
						}
						Product newProduct=new Product(typeInfo, modelInfo, colourInfo, amount);
						s1.getStore(storeNo).changeStock(newProduct, amount);
						System.out.println("The product has been successfuly added.");
						System.out.println("\nEnter command:");
						command=sc.next();
					
					}
					else if(command.equals("REMOVEProduct"))
					{
						System.out.println("Enter the type of the product you want to remove:");
						String typeInfo=sc.next();
						System.out.println("Enter the model of the product you want to remove:");
						int modelInfo=sc.nextInt();
						System.out.println("Enter the colour of the product you want to remove:");
						String colourInfo=sc.next();
						System.out.println("Enter how many you want to remove:");
						int amount=sc.nextInt();
						System.out.println("Enter the store number you want to add. For Online Store enter 0.");
						int storeNo=sc.nextInt();
						while(storeNo<0 || storeNo>s1.getStoreAmount())
						{
							System.out.println("You have entered an invalid store no. Please enter again.");
							storeNo=sc.nextInt();
						}
						Product newProduct=new Product(typeInfo, modelInfo, colourInfo, amount);
						s1.getStore(storeNo).changeStock(newProduct, -amount);
						System.out.println("The product has been successfuly removed.");
						System.out.println("\nEnter command:");
						command=sc.next();
					}
					else if(command.equals("SUPPLYProducts"))
					{
						if(s1.getSupplyAmount()==0)
							System.out.println("There are no products that are needed to be supplied.");
						else
						{
							for(int i=0; i<s1.getSupplyAmount(); ++i)
								s1.getStore(s1.getSupplyList(i).getStoreInfo()).changeStock(s1.getSupplyList(i), 1);
							
							File inputFile = new File("SupplyList.txt");
							if(inputFile.exists())
								inputFile.delete();
							
							System.out.println("All products are successfuly supplied.");
						}
						System.out.println("\nEnter command:");
						command=sc.next();
					}
					else if(command.equals("MAKESale"))
					{
						System.out.println("Enter the email of the customer who is shopping");
						String mailInfo=sc.next();
						int customerNo=-1;
						
						for(int i=0; i<s1.getCustomerAmount(); ++i)
						{
							if(mailInfo.equals(s1.getCustomer(i).getEmail()))
							{
								customerNo=s1.getCustomer(i).getCustomerNo();
								break;
							}
							else if(i==s1.getCustomerAmount()-1)
							{
								System.out.println("Enter the name of the customer who is shopping");
								String nameInfo=sc.next();
								System.out.println("Enter the surname of the customer who is shopping");
								String surnameInfo=sc.next();
								System.out.println("Enter the password of the customer who is shopping");
								String passInfo=sc.next();
								Customer newCustomer=new Customer(nameInfo, surnameInfo, mailInfo, passInfo);
								customerNo=newCustomer.getCustomerNo();
								s1.storeCustomer(newCustomer);
								System.out.println("The new customer is successfuly added.");
								System.out.println("The customer number is: "+newCustomer.getCustomerNo());
							}
						}
						s1.makeSale(customerNo);
						
						System.out.println("\nEnter command:");
						command=sc.next();
					}
					else if(command.equals("PASTOrders"))
					{
						System.out.println("Enter the customer no of the customer you want to see their past orders.");
						int customerNo=sc.nextInt();
						s1.seePastOrders(customerNo);
						
						System.out.println("\nEnter command:");
						command=sc.next();
					}
					else if(!command.equals("QUIT"))
					{
						System.out.println("You have entered an invalid command. Please enter again.");
						command=sc.next();
					}
				}
			}
			else
			{
				System.out.println("\n*******Customer Menu*******");
				System.out.println("\nCommands List:");
				System.out.println("SEARCH: Searchs for a product in the stores.");
				System.out.println("LIST: Shows all the products this company produces and sells.");
				System.out.println("SHOP: Starts shopping.");
				System.out.println("PASTOrders: Views past orders.");
				System.out.println("QUIT: Logs off.");
				System.out.println("\n****************************");
				Scanner sc=new Scanner(System.in);
				System.out.println("Enter command:");
				String command=sc.next();
				while(!command.equals("QUIT"))
				{
					if(command.equals("SEARCH"))
					{
						System.out.println("Enter the type of the product you want to search:");
						String typeInfo=sc.next();
						System.out.println("Enter the model of the product you want to search:");
						int modelInfo=sc.nextInt();
						System.out.println("Enter the colour of the product you want to search:");
						String colourInfo=sc.next();
						Product searchedProduct=new Product(typeInfo, modelInfo, colourInfo);
						s1.searchProduct(searchedProduct);
						System.out.println("\nEnter command:");
						command=sc.next();
					}
					else if(command.equals("LIST"))
					{
						s1.printAllProducts();
						System.out.println("\nEnter command:");
						command=sc.next();
					}
					else if(command.equals("SHOP"))
					{
						s1.shopMenu();
						System.out.println("\nEnter command:");
						command=sc.next();
					}
					else if(command.equals("PASTOrders"))
					{
						s1.seePastOrders();
						System.out.println("\nEnter command:");
						command=sc.next();
					}
					else if(!command.equals("QUIT"))
					{
						System.out.println("You have entered an invalid command. Please enter again.");
						command=sc.next();
					}
				}
			}
		
		}
		catch(InvalidIndex exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
	}
}
