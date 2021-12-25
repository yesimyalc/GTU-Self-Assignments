import java.util.Arrays;
import java.util.Scanner;

/**
*A class that holds the type, model, colour, amount, storeInfo and methods of a Product entity.
*Possible product types are Office_Chair, Office_Desk, Meeting_Table, Bookcase and Office_Cabinet
*Colour options in order: Black, White, Red, Blue, Green.
*Office_Chair have 7 models and have 5 colours.
*Office_Desk have 5 models and 4 colours.
*Meeting_Table has 10 models and 4 colours.
*Bookcase has 12 models and 1 colour.
*Office_Cabinet have 12 models and 1 colour.
*amount and storeInfo are specially set for products that will be used in store stocks.
*If storeInfo is set to -1, it means it is not a product that belongs to any store's stock.
*/
public class Product implements Cloneable
{
	//Fields
	private final String types[]= {"Office_Chair", "Office_Desk", "Meeting_Table", "Bookcase", "Office_Cabinet"};
	private int availableModels;
	private String colours[];

	private String type=null;
	private int model;
	private String colour=null;
	private int amount;
	private int storeInfo=-1;
	
	//Constructors
	/**
	 * Constuctor that sets the product as a Office_Chair 1 Red as default. Also sets the amount as 1
	 */
	public Product()
	{
		this("Office_Chair", 1, "Black", 1);
	}
	/**
	 * Constructor that sets the informations of the product as the given values.
	 * It sets the amount as 1.
	 * @param typeInfo is a String that will be the type of the product.
	 * @param modelInfo is an integer that will be the model of the product 
	 * @param colourInfo is a String that will be the colour of the product.
	 */
	public Product(String typeInfo, int modelInfo, String colourInfo)
	{
		this(typeInfo, modelInfo, colourInfo, 1);
	}
	/**
	 * Constructor that sets the informations of the product as the given values.
	 * @param typeInfo is a String that will be the type of the product. It can be set as Office_Chair, Office_Desk, Meeting_Table, Bookcase or Office_Cabinet.
	 * @param modelInfo is an integer that will be the model of the product. Depends on the type.
	 * @param colourInfo is a String that will be the colour of the product. Depends on the type.
	 * @param amountInfo is an integer that will be the amount of the product. It should be bigger than 0.
	 */
	public Product(String typeInfo, int modelInfo, String colourInfo, int amountInfo)
	{
		Scanner inSC=new Scanner(System.in);
		int numberInput=modelInfo;
		String typeInput=new String(typeInfo);
		String colourInput=new String(colourInfo);
		
		//If the type input is not valid, asks for a new type input.
		do
		{
			try
			{
				setType(typeInput);
			}
			catch(InvalidType exc)
			{
				System.err.print(exc);
				System.out.println("Please enter the type again.");
				typeInput=new String(inSC.next());
			}
		}
		while(type==null);

		//Sets the possible colours and the models according to the type.
		setInfo();

		//If the model input is not valid, asks for a new model input.
		do
		{
			try
			{
				setModel(numberInput);
			}
			catch(InvalidModel exc)
			{
				System.err.print(exc);
				System.out.println("Please enter the model number again.");
				numberInput=inSC.nextInt();
			}	
		}
		while(getModel()<1 || getModel()>getAvailableModels());
		
		//If the colour input is not valid, asks for a new colour input.
		do
		{
			try
			{
				setColour(colourInput);
			}
			catch(InvalidColour exc)
			{
				System.err.print(exc);
				System.out.println("Please enter the colour again.");
				colourInput=new String(inSC.next());
			}
		}
		while(colour==null);
		
		numberInput=amountInfo;
		//If the amount input is not valid, asks for a new amount input.
		do
		{
			try
			{
				setAmount(numberInput);
			}
			catch(InvalidAmount exc)
			{
				System.err.print(exc);
				System.out.println("Please enter the amount again.");
				numberInput=inSC.nextInt();
			}	
		}
		while(numberInput<0);
	}
	
	//Getters
	/**
	 * Returns the number of total available model amount of the product.
	 */
	public int getAvailableModels(){return availableModels;}
	
	/**
	 * @param indexNo is an integer that will be used to return the colour from colours at the given indexNo. It should not be less than 0 or bigger than the colours amount.
	 * @return the colour at the given indexNo
	 * @throws IndexOutOfBoundsException if the given indexNo does not have the requirements
	 */
	public String getAvailableColour(int indexNo) throws IndexOutOfBoundsException
	{
		if(indexNo<0 || indexNo>colours.length)
			throw new IndexOutOfBoundsException();
		
		return colours[indexNo];
	}
	
	/**
	 * @return the type of the product.
	 */
	public String getType() {return type;}
	
	/**
	 * @return the model of the product.
	 */
	public int getModel(){return model;}
	
	/**
	 * @return the colour of the product.
	 */
	public String getColour(){return colour;}
	
	/**
	 * @return the amount of the product.
	 */
	public int getAmount(){return amount;}
	
	/**
	 * @return the store info of the product.
	 * @return -1 if the store info is not set for this product.
	 */
	public int getStoreInfo(){return storeInfo;}
	
	//Setters
	/**
	 * Sets the storeInfo as the given storeNo. It means this product belongs to that store.
	 * If there is not a store with the given storeNo, asks for another store number input.
	 * @param storeNo is an integer that will be set as storeInfo.
	 */
	public void setStoreInfo(int storeNo)
	{
		int no=storeNo;
		while(no<0 || no>Branch.getTotalBranchCount())
		{
			System.out.println("Invalid store no. Please enter again.");
			Scanner sc=new Scanner(System.in);
			no=sc.nextInt();
		}
		storeInfo=no;
	}
	
	/**
	 * Sets the type of the product as the given type info.
	 * @param newType is a String that will be registered as the type of the product.
	 * @throws InvalidType if newType is not one of the types available.
	 */
	private void setType(String newType)throws InvalidType
	{
		if(Arrays.asList(types).contains(newType))
			type=new String(newType);
		else
			throw new InvalidType("This store does not produce "+newType+".");
	}
	
	/**
	 * Sets the model of the product as the given model info.
	 * @param newModel is an integer that will be regiistered ad the model of the product.
	 * @throws InvalidModel if the newModel is not one of the models available for the product type.
	 */
	private void setModel(int newModel) throws InvalidModel
	{
		model=newModel;
		if(getModel()<1 || getModel()>getAvailableModels())
			throw new InvalidModel("There is no Model "+getModel()+" for "+getType()+".");
	}
	
	/**
	 * Sets the colour of the product as the given colour info.
	 * @param newColour is a String that will be registered as the colour of the product.
	 * @throws InvalidColour if the newColour is not one of the colour available for the product type.
	 */
	private void setColour(String newColour)throws InvalidColour
	{
		if(Arrays.asList(colours).contains(newColour))
			colour=new String(newColour);
		else
			throw new InvalidColour(getType()+" cannot be "+newColour+".");
	}
	
	/**
	 * When the type is registered, sets the possible colour options and the possible model amount.
	 * Colour options in order: Black, White, Red, Blue, Green.
	 * Office_Chair have 7 models and have 5 colours.
	 * Office_Desk have 5 models and 4 colours.
	 * Meeting_Table has 10 models and 4 colours.
	 * Bookcase has 12 models and 1 colour.
	 * Office_Cabinet have 12 models and 1 colour.
	 */
	private void setInfo()
	{
		if(getType().equals("Office_Chair"))
		{
			String colourlist[]= {"Black", "White", "Red", "Blue", "Green"};
			colours=colourlist;
			availableModels=7;
		}
		else if(getType().equals("Office_Desk"))
		{
			String colourlist[]= {"Black", "White", "Red", "Blue"};
			colours=colourlist;
			availableModels=5;
		}
		else if(getType().equals("Meeting_Table"))
		{
			String colourlist[]= {"Black", "White", "Red", "Blue"};
			colours=colourlist;
			availableModels=10;
		}
		else if(getType().equals("Bookcase"))
		{
			String colourlist[]= {"Black"};
			colours=colourlist;
			availableModels=12;
		}
		else if(getType().equals("Office_Cabinet"))
		{
			String colourlist[]= {"Black"};
			colours=colourlist;
			availableModels=12;
		}
	}
	
	/**
	 * Sets the amount of the product.
	 * @param newAmount is an integer that will be registered as amount of the product.
	 * @throws InvalidAmount if newAmount is less than 0
	 */
	private void setAmount(int newAmount)throws InvalidAmount
	{
		if(newAmount<0)
			throw new InvalidAmount("There cannot be negative amount of products.");
		
		amount=newAmount;
	}
	
	//Other Functions
	/**
	 * Writes type, model and the colour of the product.
	 */
	public String toString()
	{
		return String.format(getType()+" "+getModel()+" "+getColour());
	}
	
	/**
	 * Makes a copy of the product.
	 */
	public Object clone() throws CloneNotSupportedException
	{
		return super.clone();
	}
	
	/**
	 * Compares two products.
	 * @param other is a product that will be compared with the current product.
	 * @return true if the type, model and the colour of the products are the same.
	 * @return false if the type, model and the colour of the products are not the same.
	 */
	public boolean equals(Product other)
	{
		if(getType().equals(other.getType()) && getColour().equals(other.getColour()) && getModel()==other.getModel())
			return true;
		
		return false;
	}
	
	/**
	 * Increments or decrements the amount of the product.
	 * @param changeValue will be the negative or positive change for the amount of the product.
	 * @throws InvalidAmount if the amount is tried to be set as a negative value.
	 */
	public void changeAmount(int changeValue)throws InvalidAmount
	{
		setAmount(getAmount()+changeValue);
	}
	
}
