import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

/**
*A class that represents the Online Store of a company. It has it's own stock and number.
*There can be only one Online Store in a company that is why if more than one Online Store is created, it will not have any affect.
*/
public class OnlineStore implements Store
{
	//Fields
	private Product[] stock;
	private int no;

	//Constructors
	/**
	 * Constructor that creates the online stock
	 * No of the online stock is set to 0.
	 * It is intended to have only 1 Online Store in the system.
	 */
	public OnlineStore()
	{
		File fw=new File("OnlineStock.txt");
		if(!fw.exists())
			createStock();
		loadStock();
		no=0;
	}
	
	//Getters
	/**
	 * @return the online store number
	 */
	public int getNo() {return no;}
	
	/**
	 * Returns a product from the stock with the given index.
	 * @param index is an integer that will be the index of the product that will be searched in the stock of the store. It cannot be less than 0 or bigger than 118.
	 * @return the product that is inside the stock with the given index value.
	 * @throws InvalidIndex if the given index does not meet the requirements.
	 */
	public Product getProduct(int index) throws InvalidIndex
	{
		if(stock==null || index<0 || index>stock.length-1)
			throw new InvalidIndex("There is not a product that is stored in this index.");
		return stock[index];
	}
	
	//Other Functions
	/**
	 * Creates a stock for the online store if it did not have one from before. 
	 * Online Stores have 5 of all products at the start.
	 */
	private void createStock()
	{
		try
		{
			FileWriter fw = new FileWriter("OnlineStock.txt");
			fw.write("Product\t\t\tAmount");
			
			Product temp=new Product("Office_Chair", 1, "Black", 1);

			for(int i=1; i<=7; ++i)
				for(int j=0; j<5; ++j)
					fw.write("\nOffice_Chair "+i+" "+temp.getAvailableColour(j)+"\t5");
			
			temp=new Product("Office_Desk", 1, "Black", 1);
			for(int i=1; i<=5; ++i)
				for(int j=0; j<4; ++j)
					fw.write("\nOffice_Desk "+i+" "+temp.getAvailableColour(j)+"\t5");
			
			temp=new Product("Meeting_Table", 1, "Black", 1);
			for(int i=1; i<=10; ++i)
				for(int j=0; j<4; ++j)
					fw.write("\nMeeting_Table "+i+" "+temp.getAvailableColour(j)+"\t5");
			
			temp=new Product("Bookcase", 1, "Black", 1);
			for(int i=1; i<=12; ++i)
				for(int j=0; j<1; ++j)
					fw.write("\nBookcase "+i+" "+temp.getAvailableColour(j)+"\t5");
			
			temp=new Product("Office_Cabinet", 1, "Black", 1);
			for(int i=1; i<=12; ++i)
				for(int j=0; j<1; ++j)
					fw.write("\nOffice_Cabinet "+i+" "+temp.getAvailableColour(j)+"\t5");
			
			fw.close();
			fw=null;
			System.gc();
		}
		catch(IOException ioe)
		{
			System.err.println("IOException: " + ioe.getMessage());
			System.exit(-1);
		}
	}
	
	/**
	 * Loads the stock from OnlineStock.txt to stock array.
	 */
	private void loadStock()
	{
		try
		{
			File filename = new File("OnlineStock.txt");
			Scanner reader=new Scanner(filename);
			
			stock=new Product[119];
			String typeInfo=new String(reader.nextLine());
			int modelInfo;
			String colourInfo;
			int amountInfo;
			for(int i=0; i<119; ++i)
			{
				typeInfo=new String(reader.next());
				modelInfo=Integer.parseInt(reader.next());
				colourInfo=new String(reader.next());
				amountInfo=Integer.parseInt(reader.next());
				
				stock[i]=new Product(typeInfo, modelInfo, colourInfo, amountInfo);
			}
			reader.close();
		}
		catch(FileNotFoundException exc)
		{
			System.out.println("This branch does not exist.");
			System.exit(-1);
		}
	}
	
	/**
	 * Prints the stock of the online store with the amounts.
	 */
	public void printStock()
	{
		for(int i=0; i<stock.length; ++i)
			System.out.println(stock[i]+"\t"+stock[i].getAmount());
	}
	
	/**
	 * Changes the amount of the product that will be given as addedProduct.
	 * @param addedProduct is the product that will be added to/remove from the stock of the store.
	 * @param amount determines how many will be removed or added to the stock.
	 */
	public void changeStock(Product addedProduct, int amount)
	{
		int i;
		try
		{
			//Firstly changes the array
			for(i=0; i<stock.length; ++i)
			{
				if(stock[i].equals(addedProduct))
				{
					stock[i].changeAmount(amount);
					break;
				}
			}
			
			//Then edits the online store's stock file.
			File inputFile = new File("OnlineStock.txt");
			File outputFile = new File("temp.txt");

            Scanner sc = new Scanner(inputFile);
            FileWriter fw = new FileWriter(outputFile);

            fw.write(sc.nextLine()+"\n");
            for(int j=0; j<i; ++j)
            	fw.write(sc.nextLine()+"\n");
            for(int j=0; j<3; ++j)
            {
            	if(j!=2)
            		fw.write(sc.next()+" ");
            	else
            		fw.write(sc.next()+"\t");
            }
            fw.write(stock[i].getAmount()+"");
            sc.next();
            
            if(sc.hasNextLine())
            {
	            fw.write(sc.nextLine());
	            for(int j=0; j<stock.length-i-1; ++j)
	            	fw.write("\n"+sc.nextLine());
            }
            
            sc.close();
            fw.close();
            sc=null;
            System.gc();
            inputFile.delete();
            outputFile.renameTo(inputFile);
		}
		catch(InvalidAmount exc)
		{
			System.err.print(exc);
			System.exit(-1);
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
	 * Online Store number can only be 0 that is why exists the program.
	 */
	public void changeNo(int newNo)
	{
		System.err.print("You cannot change Online Store's number. It is always 0.");
		System.exit(-1);
	}
	
	/**
	 * Searchs for a product in the stock of the online store.
	 * @param searchedProduct is the product that will be searched in the stock of the online store
	 * @return the index that the product is stored in the online store, if there is at least 1 one it.
	 * @return -1 if the product does not exist in the stock.
	 */
	public int isInStock(Product searchedProduct)
	{
		for(int i=0; i<stock.length; ++i)
			if(stock[i].equals(searchedProduct) && stock[i].getAmount()>0)
				return i;
		
		return -1;
	}
	
	/**
	 * Makes a copy of the online store.
	 */
	public Object clone() throws CloneNotSupportedException
	{
		OnlineStore temp=(OnlineStore)super.clone();
		
		if(stock!=null)
		{
			temp.stock=new Product[stock.length];
			for(int i=0; i<stock.length; ++i)
				temp.stock[i]=(Product)stock[i].clone();
		}
		
		return temp;
	}
}
