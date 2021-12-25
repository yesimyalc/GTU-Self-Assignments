import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.Scanner;

public class Product implements Comparable<Product>
{
	String ID;
	Categories Category;
	String Name;
	String Description;
	double price;
	double discountPrice;
	double discount;
	String trader;
	
	/**
	 * A constructor for the products which already exist in the stock.
	 * Automatically sets the info of the product
	 * @param pID is the given ID
	 * @throws IllegalArgumentException If there is a product with the given ID in the stock, throws an exception.
	 */
	public Product(String pID)
	{
		Scanner sc=new Scanner(System.in);
		while(!checkAvailablity(pID))
		{
			System.err.println("There is not a product with the given ID.");
			throw new IllegalArgumentException();
		}
		ID=pID;
		setInfo();
		setPrices(getPrice(), getDiscountPrice(), true);
	}
	
	/**
	 * A constructor to create a new product.
	 * If there is already a product with the given ID, asks for another ID
	 * Sets the information then saves the product into the stock.
	 */
	public Product(String pName, String pID, ArrayList<String> pCategory, double pPrice, double dPrice, String pDesc, String traderName)
	{
		Scanner sc=new Scanner(System.in);
		while(checkAvailablity(pID))
		{
			System.out.println("There is already a product with the given ID, please change the ID");
			pID=sc.next();
		}
		ID=pID;
		Category=new Categories(pCategory);
		Name=pName;
		Description=pDesc;
		setPrices(pPrice, dPrice, true);
		trader=traderName;
		while(!checkTrader(traderName))
		{
			System.out.println("There is not a registered trader with this name, please enter trader name again.");
			trader=sc.nextLine();
		}
		saveToFile();
	}
	
	/**
	 * A constructor to create a new product.
	 * Pre: Does not check if there is a product with the given ID, assumes there is not.
	 * Sets the information then saves the product into the stock.
	 */
	public Product(String pName, String pID, String pCategory, double pPrice, double dPrice, String pDesc, String traderName, boolean check)
	{
		ID=pID;
		Category=new Categories(pCategory);
		Name=pName;
		Description=pDesc;
		setPrices(pPrice, dPrice, true);
		trader=traderName;
	}
	
	/**
	 * Checks if there is a trader registered with the given trader name
	 * @param searchedTrader is the given trader name
	 * @return true if there is, false if there is not.
	 */
	private boolean checkTrader(String searchedTrader)
	{
		try
		{
			File infoFile = new File("UsersInfo.txt");
			Scanner fileSC= new Scanner(infoFile);
			fileSC.nextLine();
			while(fileSC.hasNext())
			{
				fileSC.next();
				StringBuffer readWord2=new StringBuffer(fileSC.next());
				String read=fileSC.next();
				while(!read.equals(";"))
				{
					readWord2.append(" "+read);
					read=fileSC.next();
				}
				if(searchedTrader.equals(readWord2.toString()))
				{
					fileSC.close();
					return true;
				}
				fileSC.nextLine();
			}
			fileSC.close();
			return false;
		}
		catch(FileNotFoundException exc)
		{
			System.out.println("The file does not exist.");
			System.exit(-1);
		}
		return false;
	}
	
	/**
	 * Checks if there is any products with the given ID. 
	 * @param searchedID is the given ID
	 * @return true if there is, false if there is not.
	 */
	private boolean checkAvailablity(String searchedID)
	{
		try
		{
			FileReader pathToCsv=new FileReader("Products.csv");
			BufferedReader csvReader = new BufferedReader(pathToCsv);
			String row;
			row=csvReader.readLine();
			while ((row = csvReader.readLine())!= null) 
			{
			    String[] data = row.split(";");
			    if(data[1].equals(searchedID))
			    {
			    	csvReader.close();
					pathToCsv.close();
			    	return true;
			    }
			}
			csvReader.close();
			pathToCsv.close();
			return false;
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
		return false;
	}
	
	/**
	 * Retrieves the information of the product with it's ID.
	 * Sets the information.
	 */
	private void setInfo()
	{
		try
		{
			FileReader pathToCsv=new FileReader("Products.csv");
			BufferedReader csvReader = new BufferedReader(pathToCsv);
			String row;
			row=csvReader.readLine();
			while ((row = csvReader.readLine())!= null) 
			{
			    String[] data = row.split(";");
			    if(data[1].equals(getID()))
			    {
			    	Name=data[0];
			    	Category=new Categories(data[2]);
			    	price=Double.parseDouble(data[3]);
			    	discountPrice=Double.parseDouble(data[4]);
			    	Description=data[5];
			    	trader=data[6];
			    }	
			}
			csvReader.close();
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
	 * Saves the product into the Products.csv file.
	 */
	private void saveToFile()
	{
		try
		{
			FileReader pathToCsv=new FileReader("Products.csv");
			BufferedReader csvReader = new BufferedReader(pathToCsv);
			String row;
			row=csvReader.readLine();
			Queue<Product> pq=new PriorityQueue<Product>();
			while ((row = csvReader.readLine())!= null) 
			{
			    String[] data = row.split(";");
			    Product p1=new Product(data[0], data[1], data[2], Double.valueOf(data[3]), Double.valueOf(data[4]), data[5], data[6], true);
		
			    pq.add(p1);
			}
			csvReader.close();
			
			pq.add(this);
			FileWriter writer = new FileWriter("Products.csv");
			
			writer.append("product_name"+';'+"id"+';'+"product_category"+';'+"price"+';'+"discounted_price"+';'+"description"+';'+"trader"+';'+'\n');
			while(!pq.isEmpty())
			{
				Product p2=pq.poll();
				writer.append(p2.getName()+';'+p2.getID()+';'+p2.getCategories()+';'+String.valueOf(p2.getPrice())+';'+String.valueOf(p2.getDiscountPrice())+';'
						+p2.getDescription()+';'+p2.getTraderName()+';'+'\n');
			}
			writer.close();
			csvReader.close();
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
	 * Edits the Products.csv file if there is any change in the product's info.
	 */
	private void editFile()
	{
		try
		{
			File inputFile=new File("Products.csv");
			File outputFile=new File("temp.csv");
			FileReader pathToCsv=new FileReader("Products.csv");
			FileWriter writer = new FileWriter("temp.csv");
			BufferedReader csvReader = new BufferedReader(pathToCsv);
			String row;
			row=csvReader.readLine();
			writer.append("product_name"+';'+"id"+';'+"product_category"+';'+"price"+';'+"discounted_price"+';'+"description"+';'+"trader"+';'+'\n');
			while ((row = csvReader.readLine())!= null) 
			{
			    String[] data = row.split(";");
			    if(!data[1].equals(getID()))
			    	writer.append(data[0]+';'+data[1]+';'+data[2]+';'+data[3]+';'+data[4]+';'+data[5]+';'+data[6]+';'+'\n');
			    else
			    	writer.append(getName()+';'+getID()+';'+getCategories()+';'+String.valueOf(getPrice())+';'+String.valueOf(getDiscountPrice())+';'
							+getDescription()+';'+getTraderName()+';'+'\n');
			    	
			}
			csvReader.close();
			pathToCsv.close();
			writer.close();
			System.gc();
			inputFile.delete();
			outputFile.renameTo(inputFile);	
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
	 * @return the ID of the product
	 */
	public String getID() {return ID;}
	/**
	 * @return the name of the product
	 */
	public String getName() {return Name;}
	/**
	 * @return the categories of the product in text form 
	 */
	public String getCategories() {return Category.convertToText();}
	/**
	 * @return the description of the product
	 */
	public String getDescription() {return Description;}
	/**
	 * @return the price of the product
	 */
	public double getPrice() {return price;}
	/**
	 * @return current discounted price of the product
	 */
	public double getDiscountPrice() {return discountPrice;}
	/**
	 * @return the discount percentage of the product
	 */
	public double getDiscount() {return discount;}
	/**
	 * @return the trader name of the product
	 */
	public String getTraderName() {return trader;}
	
	/**
	 * Sets the name of the product as the given name. 
	 * Edits the stock file accordingly.
	 * @param newName is the given name.
	 */
	public void setName(String newName)
	{
		removeFromFile();
		Name=newName;
		saveToFile();
	}
	
	/**
	 * Sets the categories of the product as the given categories. 
	 * Edits the stock file accordingly.
	 * @param categoryList is the given categories.
	 */
	public void setCategories(ArrayList<String> categoryList)
	{
		Category=new Categories(categoryList);
		editFile();
	}
	
	/**
	 * Sets the description of the product as the given description. 
	 * Edits the stock file accordingly.
	 * @param newDescription is the given description.
	 */
	public void setDescription(String newDescription)
	{
		Description=newDescription;
		editFile();
	}
	
	/**
	 * Checks if the product is in the given category.
	 * @param searchedCategory is the given category
	 * @return true if it is, false if not.
	 */
	public boolean isInCategory(String searchedCategory)
	{
		if(Category.contains(searchedCategory))
			return true;
		else
			return false;
	}
	
	/**
	 * Checks if the product current discounter price is greater than low, smaller than high.
	 * @param low is the minimum price
	 * @param high is the maximum price
	 * @return true if it is, false if not
	 */
	public boolean isInRange(double low, double high)
	{
		if(low>high)
		{
			double temp=high;
			high=low;
			low=temp;
		}
		
		if(getDiscountPrice()>=low && getDiscountPrice()<=high)
			return true;
		else 
			return false;
	}
	
	/**
	 * Checks if the product is in the stock.
	 * @return true if it is, false if it is not.
	 */
	public boolean isInStock()
	{
		return checkAvailablity(getID());
	}
	
	/**
	 * Sets the prices of the product as the given values and calculates the discount percentage.
	 * @param newPrice will be the price of the product
	 * @param newDPrice will be the current discounted price of the product
	 * @param isFirst should be true if the prices are set for the first time, should be false if not.
	 */
	public void setPrices(double newPrice, double newDPrice, boolean isFirst)
	{
		Scanner sc=new Scanner(System.in);
		while(newPrice<=0)
		{
			System.out.println("The price cannot be equal or smaller than 0. Please enter the original price again.");
			newPrice=sc.nextInt();
		}
		price=newPrice;
		while(newDPrice<=0 || newPrice>price)
		{
			System.out.println("The discounted price cannot be equal or smaller than 0. Also it cannot be greater than the original price. "
					+ "Please enter the discounted price again.");
		}
		discountPrice=newDPrice;
		
		discount=(100*(getPrice()-getDiscountPrice()))/getPrice();
		
		if(isFirst==false)
			editFile();
	}
	
	/**
	 * Removes the product from the stock by deleting it from the Products.csv file.
	 * @return true if deleted, false if not.
	 */
	public boolean removeFromFile()
	{
		boolean returnVal=false;
		try
		{
			FileReader pathToCsv=new FileReader("Products.csv");
			BufferedReader csvReader = new BufferedReader(pathToCsv);
			String row;
			row=csvReader.readLine();
			Queue<Product> pq=new PriorityQueue<Product>();
			while ((row = csvReader.readLine())!= null) 
			{
			    String[] data = row.split(";");
			    
			    if(!data[1].equals(getID()))
			    {
				    Product p1=new Product(data[0], data[1], data[2], Double.valueOf(data[3]), Double.valueOf(data[4]), data[5], data[6], true);
				    pq.add(p1);
			    }
			    else
			    	returnVal=true;
			}
			csvReader.close();
			
			FileWriter writer = new FileWriter("Products.csv");
			
			writer.append("product_name"+';'+"id"+';'+"product_category"+';'+"price"+';'+"discounted_price"+';'+"description"+';'+"trader"+';'+'\n');
			while(!pq.isEmpty())
			{
				Product p2=pq.poll();
				writer.append(p2.getName()+';'+p2.getID()+';'+p2.getCategories()+';'+String.valueOf(p2.getPrice())+';'+String.valueOf(p2.getDiscountPrice())+';'
						+p2.getDescription()+';'+p2.getTraderName()+';'+'\n');
			}
			writer.close();
			csvReader.close();
			return returnVal;
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
		return returnVal;
	}
	
	/**
	 * @return true if the two products are equal, false if not.
	 */
	public boolean equals(Object other)
	{
		if(getID().compareTo(((Product) other).getID())==0)
			return true;
		else
			return false;
	}
	
	/**
	 * Prints out the product. 
	 */
	public String toString()
	{
		StringBuffer sb=new StringBuffer();
		sb.append("----------------------------------------");
		sb.append("\nID: "+getID()+", Name: "+getName()+", Trader: "+getTraderName());
		sb.append("\n"+Category.convertToText());
		sb.append("\nPrice: "+getDiscountPrice()+" (%+"+String.format("%.2f",getDiscount())+" discount)");
		sb.append("\n"+getDescription());
		return sb.toString();
	}
	
	/**
	 * Compares the product with another product.
	 * @return an integer greater than 0 if this.name>other.name, smaller than 0 if this.name<other.name, o if this.name=other.name.
	 */
	public int compareTo(Product other)
	{
		return getName().compareTo(((Product) other).getName());
	}

}
