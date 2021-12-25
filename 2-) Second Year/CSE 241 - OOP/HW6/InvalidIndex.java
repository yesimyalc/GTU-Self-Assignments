
/**
*This is an exception class that will be thrown if the user tries to 
reach to an element in an invalid index.
*@author Yeşim_Yalçın
*/
public class InvalidIndex extends Exception
{
	/**
	 * Constructor that sets the error message.
	 */
	public InvalidIndex(String message)
	{
		super(message);
	}
}
