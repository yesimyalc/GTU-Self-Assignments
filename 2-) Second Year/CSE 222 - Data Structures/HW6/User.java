
/**
 * An interface for the users of the system.
 */
public interface User 
{
	/**
	 * @return the ID of the user.
	 */
	public String getID();
	/**
	 * @return the password of the user.
	 */
	public String getPass();
	/**
	 * @return the name of the user.
	 */
	public String getName();
	/**
	 * @return the amount of users who are registered in the system.
	 */
	public int getUserCount();
}
