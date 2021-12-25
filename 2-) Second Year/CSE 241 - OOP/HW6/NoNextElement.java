
/**
*This is an exception class that will be thrown if the user tries to 
reach to non existing elements of a set&list.
*@author Yeşim_Yalçın
*/
public class NoNextElement extends Exception
{
	/**
	 * Constructor that sets the error message.
	 */
	public NoNextElement(String message)
	{
		super(message);
	}
}
