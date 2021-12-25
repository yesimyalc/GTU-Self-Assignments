
/**
*This is an exception class that will be thrown if the user tries to 
use a method with invalid type.
*@author Yeşim_Yalçın
*/
public class InvalidType extends Exception
{
	/**
	 * Constructor that sets the error message.
	 */
	public InvalidType(String message)
	{
		super(message);
	}
}
