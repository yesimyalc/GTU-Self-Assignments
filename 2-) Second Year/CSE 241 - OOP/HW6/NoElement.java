
/**
*This is an exception class that will be thrown if the user tries to 
reach to elements of a set&list with no elements.
*@author Yeşim_Yalçın
*/
public class NoElement extends Exception
{
	/**
	 * Constructor that sets the error message.
	 */
	public NoElement(String message)
	{
		super(message);
	}
}
