import java.io.File;
import java.util.Scanner;

public class AutomationSystem {

	public static void main(String[] args)
	{
		try
		{
			//DRIVER CODE STARTS HERE
			String answer;
			Scanner testSC=new Scanner(System.in);
			
			System.out.println("LLIST DRIVER CODE");
			System.out.println("Testing constructors.");
			//Constructors, clone(), getFirst(), size() methods are tested.
			LList<String> llist1=new LList<String>();
			System.out.println("An empty LList is created.");
			LList<Integer> llist2=new LList<Integer>(3);
			System.out.println("A LList with element 3 is created.");
			LList<Integer> llist3=(LList<Integer>) llist2.clone();
			System.out.println("A clone of llist2 is created.");
			System.out.println("llist2's size: "+llist2.size()+" llist3'size: "+llist3.size());
			System.out.println("llist2's only item: "+llist2.getFirst()+" llist3's only item: "+llist3.getFirst());
			System.out.println("The sizes and the elements are same, cloning was successful.\n");
			System.out.println("Testing addition methods.");
			//add(), addLast(), addFirst(), getLast() methods are tested.
			System.out.println("Adding Hello All to llist1");
			llist1.addFirst("Hello");
			llist1.addLast("All");
			System.out.println("llist1 elements: "+llist1.get(0)+", "+llist1.getLast());
			System.out.println("Adding Good to 1'st, Hmm to 0'th, People to 4'th index.");
			llist1.add(1, "Good");
			llist1.add(0, "Hmm");
			llist1.add(4, "People");
			for(int i=0; i<llist1.size(); ++i)
				System.out.println(llist1.get(i));
			System.out.println("All addition methods were successful.\n");
			System.out.println("Testing remove methods");
			//Both remove methods are tested.
			System.out.println("Adding 5 to last index, 2 to first index.");
			llist3.addLast(5);
			llist3.addFirst(2);
			for(int i=0; i<llist3.size(); ++i)
				System.out.println(llist3.get(i));
			System.out.println("Removing 1'st index and element 5");
			Integer int1=new Integer(5);
			llist3.remove((int)1);
			llist3.remove(int1);
			for(int i=0; i<llist3.size(); ++i)
				System.out.println(llist3.get(i));
			System.out.println("Trying to remove an element that does not exist");
			boolean b1=llist3.remove((Integer)6);
			if(b1==false)
				System.out.println("Removal is failed as expected.");
			System.out.println("All removals are done successfuly.\n");
			System.out.println("Do you want to test the exceptions? Enter yes if you want to");
			answer=testSC.next();
			if(answer.equals("yes"))
			{
				System.out.println("Trying to reach an index that does not exist. Should return null.");
				System.out.println(llist1.get(5));
				System.out.println("Trying to remove an index that does not exist. Should throw an exception.");
				llist1.remove(5);
			}
			//NOTE: All the other methods related to iterators and nodes are also tested inside these methods.
			
			System.out.println("\nKWArrayList DRIVER CODE");
			System.out.println("Testing the constructors.");
			//Constructors,clone(), size(), get(), add() methods are tested.
			KWArrayList<String> kwlist1=new KWArrayList<String>();
			KWArrayList<Integer> kwlist2=new KWArrayList<Integer>(3);
			System.out.println("Adding Hello All to kwlist1.");
			kwlist1.add("Hello");
			kwlist1.add("All");
			for(int i=0; i<kwlist1.size(); ++i)
				System.out.println(kwlist1.get(i));
			System.out.println("Cloning kwlist1 to kwlist3.");
			KWArrayList<String> kwlist3=(KWArrayList<String>) kwlist1.clone();
			for(int i=0; i<kwlist3.size(); ++i)
				System.out.println(kwlist3.get(i));
			System.out.println("Cloning and additions are successful.\n");
			System.out.println("Testing additions and set methods");
			//Add methods and set mehods are tested.
			System.out.println("Adding 3, 2, 4, 12 to random places of kwlist2.");
			kwlist2.add(3);
			kwlist2.add(0,2);
			kwlist2.add(2,4);
			kwlist2.add(1,12);
			for(int i=0; i<kwlist2.size(); ++i)
				System.out.println(kwlist2.get(i));
			System.out.println("Setting 2nd as 11, 0th as 4, 3rd as 15 of kwlist2.");
			kwlist2.set(2, 11);
			kwlist2.set(0, 4);
			kwlist2.set(3, 15);
			for(int i=0; i<kwlist2.size(); ++i)
				System.out.println(kwlist2.get(i));
			System.out.println("Additions and settings were successful.\n");
			System.out.println("Testing removal methods.");
			//remove() and indexOf() methods are tested.
			System.out.println("Removing 11 and 4 from kwlist2.");
			kwlist2.remove(kwlist2.indexOf(11));
			kwlist2.remove(kwlist2.indexOf(4));
			for(int i=0; i<kwlist2.size(); ++i)
				System.out.println(kwlist2.get(i));
			System.out.println("All removals were successful.\n");
			System.out.println("Do you want to test the exceptions? Enter yes if you want to");
			answer=testSC.next();
			if(answer.equals("yes"))
			{
				System.out.println("Enter a number between 0 and 2 to choose the test case.");
				answer=testSC.next();
				if(answer.equals("0"))
				{
					System.out.println("Trying to reach an index that does not exist. Should throw exception.");
					System.out.println(kwlist1.get(2));
				}
				else if(answer.equals("1"))
				{
					System.out.println("Trying to remove an index that does not exist. Should throw an exception.");
					kwlist2.remove(5);
				}
				else if(answer.equals("2"))
				{
					System.out.println("Trying to add to an index that does not exist. Should throw an exception.");
					kwlist1.add(5, "Why");
				}
			}
			//NOTE: All the other private methods are also tested inside these methods.
			
			System.out.println("\nHybridList DRIVER CODE");
			System.out.println("Testing the constructor and the add methods");
			//Constructor, all size methods, add method, get methods are tested.
			HybridList<String> hblist1=new HybridList<String>();
			System.out.println("Adding 14 elements to the list.");
			hblist1.add("Hello");
			hblist1.add("Every");
			hblist1.add("People");
			hblist1.add("Here");
			hblist1.add("4");
			hblist1.add("5");
			hblist1.add("6");
			hblist1.add("7");
			hblist1.add("8");
			hblist1.add("9");
			hblist1.add("10");
			hblist1.add("Last");
			hblist1.add("New");
			hblist1.add("1");
			System.out.println("The arraylist amount inside the hybridlist: "+hblist1.listSize());
			System.out.println("The element amount inside the hybridlist: "+hblist1.size());
			System.out.println("There are 2 arraylist in total so the first element of the second one should be: "+hblist1.get(1,0));
			for(int i=0; i<hblist1.arraySize(); ++i)
				System.out.println(hblist1.get(i));
			System.out.println("All the additions are done successfuly.\n");
			System.out.println("Testing the removal methods.");
			System.out.println("Removing both of the elements in the second arratlist.");
			hblist1.remove();
			hblist1.remove(1,0);
			System.out.println("Second arraylist became empty, must have been removed. ArrayList count: "+hblist1.listSize());
			System.out.println("Removing 2 more elements, new size must be 10.");
			hblist1.remove(3);
			hblist1.remove(5);
			for(int i=0; i<hblist1.arraySize(); ++i)
				System.out.println(hblist1.get(i));
			System.out.println("All the removals are done successfuly.\n");
			System.out.println("Do you want to test the exceptions? Enter yes if you want to");
			answer=testSC.next();
			if(answer.equals("yes"))
			{
				System.out.println("Enter a number between 0 and 2 to choose the test case.");
				answer=testSC.next();
				if(answer.equals("0"))
				{
					System.out.println("Trying to reach an index that does not exist. Should throw exception.");
					System.out.println(hblist1.get(12));
				}
				else if(answer.equals("1"))
				{
					System.out.println("Trying to remove an index that does not exist. Should throw an exception.");
					hblist1.remove(2,3);
				}
				else if(answer.equals("2"))
				{
					System.out.println("Trying to remove an index that does not exist. Should throw an exception.");
					hblist1.remove(10);
				}
			}
			//NOTE: All the other private methods are also tested inside these methods.
			
			System.out.println("\nAUTOMATION SYSTEM DRIVER CODE");

			System.out.println("ONLINESTORE CLASS DRIVER CODE");
			System.out.println("Testing all methods");
			System.out.println("Creating an OnlineStore, OnlineStore.txt must have been created if you are running this code for the first time.");
			OnlineStore os1=new OnlineStore();
			System.out.println("The store no of the online store: "+os1.getNo());
			System.out.println("Printing the OnlineStore's stock");
			os1.printStock();
			System.out.println("\n");
			System.out.println("Removing some amount from a product in the stock");
			os1.changeStock(new Product(), -5);
			if(os1.isInStock(new Product())==-1)
					System.out.println("The removal was done successfuly.");
			System.out.println("Adding some amount from a product in the stock");
			os1.changeStock(new Product(), 5);
			if(os1.isInStock(new Product())!=-1)
				System.out.println("The addition was done successfuly.");
			System.out.println("Creating new online stores however there can be only 1 online store in general therefore they should be the same.");
			OnlineStore os2=(OnlineStore) os1.clone();
			OnlineStore os3=new OnlineStore();
			if(os2.equals(os3))
				System.out.println("The creation was successful. All online stores are the same and they share the same stock text. "
						+ "However, creating more than 1 online stores should be avoided.");
			System.out.println("All the methods are done successfuly.\n");
			System.out.println("Do you want to test the exceptions? Enter yes if you want to");
			answer=testSC.next();
			if(answer.equals("yes"))
			{
				System.out.println("Enter a number between 0 and 1 to choose the test case.");
				answer=testSC.next();
				if(answer.equals("0"))
				{
					System.out.println("Trying to change the store no of the online store");
					os1.changeNo(3);
				}
				else if(answer.equals("1"))
				{
					System.out.println("Trying to make a product's amount negative in the stock");
					os1.changeStock(new Product(), -10);
				}
			}
			//NOTE: All the other private methods are also tested inside these methods.
			
			System.out.println("\nBRANCH CLASS DRIVER CODE");
			System.out.println("Testing all methods");
			System.out.println("Creating 4 branches. BranchXStock.txt files should be created for each if you are running this program for the first time.");
			Branch tb1=new Branch();
			Branch tb2=new Branch();
			Branch tb3=new Branch();
			Branch tb4=new Branch();
			System.out.println("The branch no of tb1: "+tb1.getNo());
			System.out.println("There are total of "+Branch.getTotalBranchCount()+" branches.");
			System.out.println("Printing the tb1's stock");
			tb1.printStock();
			System.out.println("\n");
			System.out.println("Removing some amount from a product in the stock");
			tb1.changeStock(new Product(), -3);
			if(tb1.isInStock(new Product())==-1)
					System.out.println("The removal was done successfuly.");
			System.out.println("Adding some amount from a product in the stock");
			tb1.changeStock(new Product(), 3);
			if(tb1.isInStock(new Product())!=-1)
				System.out.println("The addition was done successfuly.");
			System.out.println("Changing the no of branch 4");
			tb4.changeNo(32);
			System.out.println("The branch no of tb4: "+tb4.getNo());
			tb4.changeNo(4);
			System.out.println("All the methods are done successfuly.\n");
			System.out.println("Do you want to test the exceptions? Enter yes if you want to");
			answer=testSC.next();
			if(answer.equals("yes"))
			{
				System.out.println("Enter a number between 0 and 1 to choose the test case.");
				answer=testSC.next();
				if(answer.equals("0"))
				{
					System.out.println("Trying to change the store no of the tb2 with an already existing branch number.");
					tb2.changeNo(3);
				}
				else if(answer.equals("1"))
				{
					System.out.println("Trying to make a product's amount negative in tb1 stock");
					tb1.changeStock(new Product(), -10);
				}
			}
			//NOTE: All the other private methods are also tested inside these methods.
			
			System.out.println("\nPRODUCT CLASS DRIVER CODE");
			System.out.println("Testing all methods");
			System.out.println("Using the default constructor.");
			Product p1=new Product();
			System.out.println(p1+ " is created.");
			System.out.println("Using the second constructor with wrong information.");
			Product p2=new Product("Book",32, "RED");
			System.out.println(p2+ " is created.");
			System.out.println("Using the third constructor with wrong amount.");
			Product p3=new Product("Bookcase", 2, "Black", -5);
			System.out.println(p3+ " is created.");
			Product p4=(Product) p1.clone();
			if(p1.equals(p4))
				System.out.println("Cloning was done successfuly.");
			System.out.println("Setting a storeInfo for the product with wrong store info. Note that there are 4 branches and 1 online store at the moment.");
			p2.setStoreInfo(5);
			System.out.println("All methods are done propery.\n");
			System.out.println("Do you want to test the exceptions? Enter yes if you want to");
			answer=testSC.next();
			if(answer.equals("yes"))
			{
				System.out.println("Changing the amount of p1 with wrong amount");
				p1.changeAmount(-16);
				System.out.println("New amount of p1: "+p1.getAmount());
			}
			//NOTE: All the other private methods are also tested inside these methods.

			System.out.println("\nADMIN ClASS DRIVER CODE");
			System.out.println("Testing all methods");
			System.out.println("Creating an already existing admin.");
			Admin ta1=new Admin("Jade", "Rugged", "y.yalcin072@gmail.com", "12345y", "old");
			System.out.println("Creating a new admin with an already existing email than changing the email. Note: Enter a random email to fix it. AdminsInfo.txt should be updated.");
			Admin ta2= new Admin("Kara", "Clark", "y.yalcin072@gmail.com", "324234");
			System.out.println("Creating a copy of ta2");
			Admin ta3=(Admin) ta2.clone();
			if(ta2.equals(ta3))
				System.out.println("Copying was successful.");
			System.out.println("ta3 is: "+ta3);
			System.out.println("ta2 is: "+ta2);
			System.out.println("Deleting the new admins not to affect the system code later on.");
			ta3.deleteAdmin();
			ta2.deleteAdmin();
			System.out.println("All methods are done propery.\n");
			System.out.println("Do you want to test the exceptions? Enter yes if you want to");
			answer=testSC.next();
			if(answer.equals("yes"))
			{
				System.out.println("Trying to create a new admin as an old admin. Should throw and exception.");
				Admin ta4=new Admin("Cassidy", "Smith", "smith@gmail.com", "323423", "old");
			}
			//NOTE: All the other private methods are also tested inside these methods.
			
			System.out.println("\nBRANCHEMPLOYEE CLASS DRIVER CODE");
			System.out.println("Testing all methods");
			System.out.println("Creating an already existing employee.");
			BranchEmployee tbe1=new BranchEmployee("Menma", "Taylor", "menma03@gmail.com", "12345", "old");
			System.out.println("Creating a new employee with an already existing email than changing the email. Note: Enter a random email to fix it. BranchEmployeesInfo.txt should be updated.");
			BranchEmployee tbe2=new BranchEmployee("James", "Lock", "menma03@gmail.com", "234234");
			System.out.println("Creating a copy of tbe2");
			BranchEmployee tbe3=(BranchEmployee) tbe2.clone();
			if(tbe2.equals(tbe3))
				System.out.println("Copying was successful.");
			System.out.println("tbe2 is: "+tbe2);
			System.out.println("tbe3 is: "+tbe3);
			System.out.println("Deleting the new employees not to affect the system code later on.");
			tbe2.deleteEmployee();
			tbe3.deleteEmployee();
			System.out.println("\n");
			System.out.println("Do you want to test the exceptions? Enter yes if you want to");
			answer=testSC.next();
			if(answer.equals("yes"))
			{
				System.out.println("Trying to create a new employee as an old employee. Should throw and exception.");
				BranchEmployee ta4=new BranchEmployee("Cassidy", "Smith", "smith@gmail.com", "323423", "old");
			}
			//NOTE: All the other private methods are also tested inside these methods.
			
			System.out.println("\nCUSTOMER CLASS DRIVER CODE");
			System.out.println("Testing all methods");
			System.out.println("Creating an already existing customer.");
			Customer tc1=new Customer("Fayron", "Carol", "fayron@gmail.com", "23323", "old");
			System.out.println("Creating a new customer with an already existing email than changing the email. Note: Enter a random email to fix it. CustomersInfo.txt should be updated.");
			Customer tc2=new Customer("Lara", "Lock", "fayron@gmail.com", "234234");
			System.out.println("Creating a copy of tc2");
			Customer tc3=(Customer) tc2.clone();
			if(tc3.equals(tc2))
				System.out.println("Copying was successful.");
			System.out.println("tc1 customer no: "+tc1.getCustomerNo());
			System.out.println("tc2 customer no: "+tc2.getCustomerNo());
			System.out.println("We created tc3 too however its the clone of tc2. There should be "+tc2.getCustomerNo()+" customers in total: "+tc1.getTotalCustomerCount());
			System.out.println("Adding new and past orders to tc2");
			tc2.addNewOrder(p4, 0);
			tc2.addNewOrder(p3, 2);
			if(tc2.isNewOrder(p4)!=-1)
				System.out.println("Adding to new orders was successful.");
			tc2.addPastOrder(p4, 0);
			tc2.addPastOrder(p1, 3);
			if(tc2.isPastOrder(p1)!=-1)
				System.out.println("Adding to past orders was successful.");
			System.out.println("Printing tc2's new orders");
			tc2.printNewOrders();
			System.out.println("\n");
			System.out.println("Printing tc2's past orders");
			tc2.printPastOrders();
			System.out.println("\n");
			System.out.println("Removing a new order of tc2");
			tc2.removeNewOrder(p4);
			System.out.println("Removing a non existing new order of tc2. Will return without doing anything.");
			tc2.removeNewOrder(p1);
			System.out.println("Adding information to tc2");
			tc2.saveInfo("TaralawnCt213", "3424234234");
			System.out.println(tc2+" address: "+tc2.getAddress()+" , phone number: "+tc2.getPhoneNumber());
			System.out.println("Deleting the new customers not to affect the system code later on.");
			tc2.removeCustomer();
			tc3.removeCustomer();
			System.out.println("\n");
			System.out.println("Do you want to test the exceptions? Enter yes if you want to");
			answer=testSC.next();
			if(answer.equals("yes"))
			{
				System.out.println("Trying to create a new customer as an old customer. Should throw and exception.");
				Customer ta4=new Customer("Cassidy", "Smith", "smith@gmail.com", "323423", "old");
			}
			//NOTE: All the other private methods are also tested inside these methods.
			
			System.out.println("\nCOMPANYSYSTEM CLASS DRIVER CODE");
			//Making the totalBranchCount 0 to make the company system function properly. 
			Branch.totalBranchCount=0;
			Customer.totalCustomerCount=0;
			System.out.println("Creating the system");
			CompanySystem tcs1=new CompanySystem();
			
			System.out.println("Testing methods related to the users");
			System.out.println("Printing all the admins");
			tcs1.printAdmins();
			System.out.println("\n");
			if(tcs1.isAdmin(ta1.getEmail())!=-1)
				System.out.println(ta1+" is an admin of the system");
			System.out.println("Printing all the employees");
			tcs1.printEmployees();
			System.out.println("\n");
			if(tcs1.isEmployee(tbe1.getEmail())!=-1)
				System.out.println(tbe1+" is an employee of the system");
			System.out.println("Printing all the customers");
			tcs1.printCustomers();
			System.out.println("\n");
			if(tcs1.isCustomer(tc1.getEmail())!=-1)
				System.out.println(tc1+" is a customer of the system");
			System.out.println("Creating an new emplyee and then removing it.");
			BranchEmployee tbe4=new BranchEmployee("John", "Clark", "john@gmail.com", "324234");
			tcs1.storeEmployee(tbe4);
			System.out.println(tbe4+" is created.");
			tcs1.removeEmployee(tbe4);
			System.out.println(tbe4+" is removed.");
			System.out.println("Printing all the employees again.");
			tcs1.printEmployees();
			System.out.println("Checking the past orders of a customer that does not exist.");
			tcs1.seePastOrders(12);
			System.out.println("Checking the past orders of a customer that exists.");
			tcs1.seePastOrders(2);
			System.out.println("\n");
			
			System.out.println("Testing the methods related to the supplies");		
			System.out.println("Printing all the product the store produces and sells.");
			tcs1.printAllProducts();
			System.out.println("\n");
			System.out.println("Searching for a special product in the store");
			tcs1.searchProduct(p4);
			System.out.println("\n");
			System.out.println("Printing the supply list. It should be empty for now.");
			tcs1.printSupplyList();
			System.out.println("Querying the supply list. It should be empty.");
			tcs1.querySupplyList();
			System.out.println("Trying to remove supplies when there is nothing to remove.");
			tcs1.removeSupply(p4);
			System.out.println("Adding some supplies to the SupplyList");
			p4.setStoreInfo(1);
			p3.setStoreInfo(2);
			p2.setStoreInfo(0);
			tcs1.addSupply(p4);
			tcs1.addSupply(p3);
			tcs1.addSupply(p2);
			System.out.println("Printing the supply list. Supplies should be added.");
			tcs1.printSupplyList();
			System.out.println();
			if(tcs1.isSupply(p3)!=-1)
				System.out.println("The addition was successful.");
			System.out.println("Removing p3: "+p3);
			tcs1.removeSupply(p3);
			if(tcs1.isSupply(p3)==-1)
				System.out.println("The removal was successful.\n");
			System.out.println("Querying the supply list.");
			tcs1.querySupplyList();
			System.out.println("\n");
			
			System.out.println("Testing the methods related to branches");
			/*
			tcs1.addBranch(new Branch());
			System.out.println("A new branch is added. Now it is gonna be deleted.");
			tcs1.removeBranch(5);
			System.out.println("\n");
			*/
			//Note this part of the driver code is commented out because it affects the real menu system of the automation system. However you can delete the comments to test it once. The result of this coe piece will be in the report.
			System.out.println("Do you want to test the exceptions? Enter yes if you want to");
			answer=testSC.next();
			if(answer.equals("yes"))
			{
				System.out.println("Trying to remove a branch that does not exist. Should throw an exception.");
				tcs1.removeBranch(5);
			}
			//NOTE: All the other private methods are also tested inside these methods. However some menu methods are skipped because they require many user input to test.
			//They are being demonstrated in the real system.
			
			//Reseting some things to have the automation system work properly.
			Branch.totalBranchCount=0;
			Customer.totalCustomerCount=0;	
		
			//REAL AUTOMATION SYSTEM STARTS HERE
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
							while(no<0 || no>=s1.getStoreAmount())
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
						while(storeNo<0 || storeNo>=s1.getStoreAmount())
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
						while(storeNo<0 || storeNo>=s1.getStoreAmount())
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
		catch(InvalidAmount exc)
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
}
