import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

/**
*A class that holds the general informations and methods of a BranchEmployee entity.
*/
public class BranchEmployee extends User
{	
	//Constructors
	/**
	 * Constructor that sets everything as an empty string and the status as old. It should be avoided to use this constructor.
	 */
	public BranchEmployee()
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
	public BranchEmployee(String nameInfo, String surnameInfo, String mailInfo, String passInfo)
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
	 */
	public BranchEmployee(String nameInfo, String surnameInfo, String mailInfo, String passInfo, String statusInfo)
	{
		super(nameInfo, surnameInfo, mailInfo, passInfo, statusInfo);
	
		//If the employee is new, the employee should be saved.
		if(getStatus().equals("new"))
			saveEmployee();
	}
	
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
			//Checks if the email was registered from BranchEmployeesInfo.txt file
			File infoFile = new File("BranchEmployeesInfo.txt");
			Scanner fileSC= new Scanner(infoFile);
			Scanner inSC= new Scanner(System.in);
			String mailInput=new String(newMail);
			while(fileSC.hasNext())
			{
				String readWord=new String(fileSC.next());
				if(readWord.equals(mailInput))
				{
					//If it was registered from before, asks for a new email.
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
	 * Writes the name and the surname of the employee.
	 */
	public String toString()
	{
		return String.format(getName()+" "+getSurname());
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
			File infoFile = new File("BranchEmployeesInfo.txt");
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
				fileSC=null;
				System.gc();
				throw new InvalidStatus("There is not an already existing branch employee with this email address.");
			}
			fileSC.close();
			fileSC=null;
			System.gc();
		}
		catch(FileNotFoundException exc)
		{
			System.out.println("The file does not exist.");
			System.exit(-1);
		}
	}
	
	/**
	 * Saves the created employee to the BranchEmployeesInfo.txt file as a registered employee.
	 */
	public void saveEmployee()
	{
		try
		{
			FileWriter fw = new FileWriter("BranchEmployeesInfo.txt", true);
			fw.write("\n"+getEmail()+" "+getName()+" "+getSurname()+" "+getPassword());
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
	 * Deletes the employee from the BranchEmployeesInfo.txt file.
	 */
	public void deleteEmployee()
	{
        try 
        {   
			File inputFile = new File("BranchEmployeesInfo.txt");
			File outputFile = new File("temp.txt");

            Scanner sc = new Scanner(inputFile);
            FileWriter fw = new FileWriter(outputFile);

            fw.write(sc.nextLine()+"\n");
            while(sc.hasNextLine()) 
            {
                String str = sc.nextLine();             
                str = str.replaceAll(getEmail()+" "+getName()+" "+getSurname()+" "+getPassword(), "");
                
                if(!str.equals(""))
                	fw.write("\n"+str);
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
	 * Makes a copy of the employee.
	 */
	public Object clone() throws CloneNotSupportedException
	{
		return super.clone();
	}
}
