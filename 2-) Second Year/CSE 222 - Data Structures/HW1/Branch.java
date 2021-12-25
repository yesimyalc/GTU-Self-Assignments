import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

/**
*A class that represents Branches of a company. It has it's own stock and number.
*/
public class Branch implements Store
{
	//Fields
	public static int totalBranchCount;
	private Product[] stock;
	private int branchNumber;
	
	//Constructors
	/**
	 * Constructor that creates a new branch with the following branch number.
	 * Increments the totalBranchCount everytime a new branch is created and this is used to determine the branch number.
	 */
	public Branch()
	{
		totalBranchCount++;
		branchNumber=getTotalBranchCount();
		
		//Creates a new stock for the created branch if this branch did not have a stock from before
		File fw = new File("Branch"+getNo()+"Stock.txt");
		if(!fw.exists())
			createStock();
		
		//Loads the stock to the stock array
		loadStock();
	}
	
	//Getters
	/**
	 * @return the branch number
	 */
	public int getNo() {return branchNumber;}
	
	/**
	 * @return the totalBranchCount.
	 */
	public static int getTotalBranchCount() {return totalBranchCount;}
	
	/**
	 * Returns a product from the stock with the given index.
	 * @param index is an integer that will be the index of the product that will be searched in the stock of the branch. It cannot be less than 0 or bigger than 118.
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
	 * Creates a stock if the branch did not have a stock from before.
	 * Branch stocks have 3 from all possible products at the beginning.
	 */
	private void createStock()
	{
		try
		{
			FileWriter fw = new FileWriter("Branch"+getNo()+"Stock.txt");
			fw.write("Branch "+getNo()+": Product-Amount");
			
			Product temp=new Product("Office_Chair", 1, "Black", 1);

			for(int i=1; i<=7; ++i)
				for(int j=0; j<5; ++j)
					fw.write("\nOffice_Chair "+i+" "+temp.getAvailableColour(j)+"\t3");
			
			temp=new Product("Office_Desk", 1, "Black", 1);
			for(int i=1; i<=5; ++i)
				for(int j=0; j<4; ++j)
					fw.write("\nOffice_Desk "+i+" "+temp.getAvailableColour(j)+"\t3");
			
			temp=new Product("Meeting_Table", 1, "Black", 1);
			for(int i=1; i<=10; ++i)
				for(int j=0; j<4; ++j)
					fw.write("\nMeeting_Table "+i+" "+temp.getAvailableColour(j)+"\t3");
			
			temp=new Product("Bookcase", 1, "Black", 1);
			for(int i=1; i<=12; ++i)
				for(int j=0; j<1; ++j)
					fw.write("\nBookcase "+i+" "+temp.getAvailableColour(j)+"\t3");
			
			temp=new Product("Office_Cabinet", 1, "Black", 1);
			for(int i=1; i<=12; ++i)
				for(int j=0; j<1; ++j)
					fw.write("\nOffice_Cabinet "+i+" "+temp.getAvailableColour(j)+"\t3");
			
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
	 * Loads the stock to stock array from the file branch's stock is saved.
	 */
	private void loadStock()
	{
		try
		{
			File filename = new File("Branch"+getNo()+"Stock.txt");
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
	 * Prints out the products that is inside the stock with the amounts.
	 */
	public void printStock()
	{
		for(int i=0; i<stock.length; ++i)
			System.out.println(stock[i]+"\t"+stock[i].getAmount());
	}
	
	/**
	 * Changes the amount of the product that will be given as addedProduct.
	 * @param addedProduct is the product that will be added to/remove from the stock of the branch.
	 * @param amount determines how many will be removed or added to the stock.
	 */
	public void changeStock(Product addedProduct, int amount)
	{
		int i;
		try
		{
			//Firstly changes the stock array
			for(i=0; i<stock.length; ++i)
			{
				if(stock[i].equals(addedProduct))
				{
					stock[i].changeAmount(amount);
					break;
				}
			}
			
			//Then edits the branch's store file
			File inputFile = new File("Branch"+getNo()+"Stock.txt");
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
            fw.write(String.valueOf(stock[i].getAmount()));
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
	 * Changes the number of the branch.
	 * @param newNo will be the new number of the store. If there is already a branch with the given newNo, exists because there cannot be 2 branches with the same number.
	 */
	public void changeNo(int newNo)
	{
		try
		{
			File outputFile = new File("Branch"+newNo+"Stock.txt");
			if(outputFile.exists())
			{
				System.err.print("There is already a branch with this number."+newNo);
				System.exit(-1);
			}
			File inputFile = new File("Branch"+getNo()+"Stock.txt");
			Scanner sc = new Scanner(inputFile);
	        FileWriter fw = new FileWriter(outputFile);
	        
	        fw.write(sc.nextLine());
	        while(sc.hasNextLine())
	        	fw.write("\n"+sc.nextLine());
	        
	        sc.close();
	        fw.close();
	        inputFile.delete();
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
	 * Searchs for a product in the stock of the branch.
	 * @param searchedProduct is the product that will be searched in the stock of the branch.
	 * @return the index that the product is stored in the branch, if there is at least 1 one it.
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
	 * Makes a clone of the branch.
	 */
	public Object clone() throws CloneNotSupportedException
	{
		Branch temp=(Branch)super.clone();
		
		if(stock!=null)
		{
			temp.stock=new Product[stock.length];
			for(int i=0; i<stock.length; ++i)
				temp.stock[i]=(Product)stock[i].clone();
		}
		
		return temp;
	}
}
