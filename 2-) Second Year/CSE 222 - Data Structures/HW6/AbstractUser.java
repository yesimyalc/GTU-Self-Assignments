import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

/**
 * An abstract class for the users of the system. 
 * Does the shared basic operations of the users.
 */
public abstract class AbstractUser implements User
{
	protected String ID;
	protected String password;
	protected String name;
	protected static int userCount;
	
	/**
	 * This is a constructor for a user that is not registered to the system before.
	 * Automatically determines the user id, asks for a password.
	 * Saves the user into the UsersInfo.txt file.
	 * @param userName is the user's name who is going to be created.
	 */
	public AbstractUser(String userName)
	{
		if(userCount==0)
			setUserCount();
		Scanner sc=new Scanner(System.in);
		ID=String.valueOf(10000000+userCount-2);
		System.out.print("Please enter your password: ");
		password=sc.next();
		while(password.length()!=6)
		{
			System.out.println("The password should contain 6 characters. Please enter again.");
			password=sc.next();
		}
		name=userName;
		userCount++;
	}
	
	/**
	 * This is a constructor for a user that has been registered to the system before.
	 * Checks if the user actually was registered before.
	 * Automatically sets the information of the user. 
	 * @param userID is the ID of the user.
	 * @param userPass is the password of the user.
	 */
	public AbstractUser(String userID, String userPass)
	{
		if(userCount==0)
			setUserCount();
		login(userID, userPass);
		name=checkID(getID(), getPass());
	}
	
	/**
	 * This is a constructor for a user that has been registered to the system before.
	 * Pre: Assumes the user was actually registered before does not check.
	 * Automatically sets the information of the user. 
	 * @param userID
	 * @param userName
	 * @param userPass
	 * @param check
	 */
	public AbstractUser(String userID, String userName, String userPass, boolean check)
	{
		ID=userID;
		name=userName;
		password=userPass;
	}
	
	/**
	 * Checks if the user can login to the system with the gicen ID and the password.
	 * If there is no user with the given ID and password, asks for a new one.
	 * @param searchedID
	 * @param searchedPass
	 */
	private void login(String searchedID, String searchedPass)
	{
		Scanner sc=new Scanner(System.in);
		while(checkID(searchedID, searchedPass)==null)
		{
			System.out.println("There is no registered user with the given information. Please enter your ID and password again.");
			System.out.print("Enter ID: ");
			searchedID=sc.next();
			System.out.print("Enter Password: ");
			searchedPass=sc.next();
		}
		ID=searchedID;
		password=searchedPass;
	}
	
	/**
	 * Counts how many users have been registered before in the system and sets the amount as userCount.
	 */
	private void setUserCount()
	{
		try
		{
			File infoFile = new File("UsersInfo.txt");
			Scanner fileSC= new Scanner(infoFile);
			while(fileSC.hasNext())
			{
				userCount++;
				fileSC.nextLine();
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
	 * Checks if there is a registered user with the given ID and the password
	 * @param searchedID is the given ID
	 * @param searchedPass is the given password
	 * @return true if there is a user, false if there is not
	 */
	private String checkID(String searchedID, String searchedPass)
	{
		try
		{
			File infoFile = new File("UsersInfo.txt");
			Scanner fileSC= new Scanner(infoFile);
			Scanner inSC= new Scanner(System.in);
			
			while(fileSC.hasNext())
			{
				String readWord=new String(fileSC.next());
				if(readWord.equals(searchedID))
				{
					StringBuffer readWord2=new StringBuffer(fileSC.next());
					String read=fileSC.next();
					while(!read.equals(";"))
					{
						readWord2.append(" "+read);
						read=fileSC.next();
					}
					String readWord3=new String(fileSC.next());
					
					if(readWord3.equals(searchedPass))
					{
						fileSC.close();
						return readWord2.toString();
					}
					fileSC.next();
					fileSC.close();	
					return null;
				}
				else
					fileSC.nextLine();
			}
			fileSC.close();	
			return null;
		}
		catch(FileNotFoundException exc)
		{
			System.out.println("The file does not exist.");
			System.exit(-1);
		}
		return null;
	}
	
	/**
	 * @return the ID of the user.
	 */
	public String getID() {return ID;}
	/**
	 * @return the password of the user.
	 */
	public String getPass() {return password;}
	/**
	 * @return the name of the user.
	 */
	public String getName() {return name;}
	/**
	 * @return the amount of users who are registered in the system.
	 */
	public int getUserCount() {return userCount;}
	
	/**
	 * Prints out the user id and the user name
	 */
	public String toString()
	{
		return String.format(getID()+" "+getName());
	}
	
	/**
	 * Checks if the given object has the same id as the user.
	 */
	public boolean equals(Object other)
	{
		if(getID().compareTo(((User) other).getID())==0)
			return true;
		else
			return false;
	}
}
