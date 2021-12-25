import java.util.Scanner;

/**
*An abstract class that holds the name, surname, email, password, status info and methods of a User entity.
*/
public abstract class User implements Cloneable
{
	//Fields
	protected String name;
	protected String surname;
	protected String email;
	protected String password;
	protected String status;
	
	//Constructors
	/**
	 * @param nameInfo is a String that will be the name of the user
	 * @param surnameInfo is a String that will be the surname of the user
	 * @param mailInfo is a String that will be the email of the user
	 * @param passInfo is a String that will be the password of the user
	 * @param statusInfo is a String that will be the status of the user. It should be either new or old.
	 * Constructor that sets the user information as the given information.
	 */
	public User(String nameInfo, String surnameInfo, String mailInfo, String passInfo, String statusInfo)
	{
		Scanner inSC=new Scanner(System.in);
		try
		{
			name=new String(nameInfo);
			surname=new String(surnameInfo);
			password=new String(passInfo);
			
			//Checks if the statusInput is new or old. If it is not, asks for a new status input.
			String statusInput=new String(statusInfo);
			do
			{
				try
				{
					setStatus(statusInput);
				}
				catch(InvalidStatus exc)
				{
					System.err.print(exc);
					System.out.println("Please enter the status again.");
					statusInput=new String(inSC.next());
				}
			}
			while(status==null);
			
			/*Checks if there is a user registered with the email information.
			 *If the input is old and there is not, it means the input was wrong.
			 *If the input is new and there is, it also means the input was wrong.
			 */
			if(status.equals("old"))
			{	
				checkEmail(mailInfo);
				email=new String(mailInfo);
			}
			else
				setEmail(mailInfo);	

		}
		catch(InvalidStatus exc)
		{
			System.err.print(exc);
			System.exit(-1);
		}
	}
	
	//Getters
	/**
	 * @return the name of the user
	 */
	public String getName(){return name;}
	/**
	 * @return the surname of the user
	 */
	public String getSurname(){return surname;}
	/**
	 * @return the email of the user
	 */
	public String getEmail(){return email;}
	/**
	 * @return the password of the user
	 */
	public String getPassword(){return password;}
	/**
	 * @return the status of the user
	 */
	public String getStatus() {return status;}
	
	//Setters
	/**
	 * This method is used when the status is new. 
	 * Checks if the email info was registered from before.
	 * Sets the email if the email info was not registered from before.
	 * @param newMail is a String that will be set as the email information.
	 */
	protected abstract void setEmail(String newMail);
	
	/**
	 * Sets the status information. Also checks if it is new or old.
	 * @param newStatus is a String that will be set as the status information.
	 * @throws InvalidStatus if the given status is not new or old.
	 */
	protected void setStatus(String newStatus)throws InvalidStatus
	{
		if(!newStatus.equals("new") && !newStatus.equals("old"))
			throw new InvalidStatus("The status of a user can be either new or old.\n");
				
		status=new String(newStatus);
	}
	
	//Other Functions
	/**
	 * Checks if the given email information was registered from before.
	 * @param mailInfo is a String that holds and email information that will be searched if it was registered from before.
	 * @throws InvalidStatus if the given email information was already registered when the status was new, or the email was not registered when the status was old.
	 */
	protected abstract void checkEmail(String mailInfo)throws InvalidStatus;
	
	/**
	 * @param other is a User 
	 * @return true if the users have the same name, surname and email
	 * @return false if the users are not the same name, surname and email
	 */
	public boolean equals(User other)
	{
		if(getName().equals(other.getName()) && getSurname().equals(other.getSurname()) && getEmail().equals(other.getEmail()))
			return true;
		return false;	
	}
}
