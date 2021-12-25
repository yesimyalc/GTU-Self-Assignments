import java.util.ArrayList;

/**
 * A class that can hold the categories of a Product.
 * The categories can be accepted with a proper string form or in an arraylist form.
 * The categories are held in an arraylist form.
 * The categories can be converted into a proper string form if needed.
 */
public class Categories 
{
	ArrayList<String> categories;
	
	/**
	 * Pre: the string should be in a proper format example: "[""Blabla1 >> Blabla2 >> Blabla3""]"
	 * Blabla1, Blabla2, Blabla3 are set as the categories in the given order.
	 * A constructor that sets the categories by retrieving them from a string.
	 * @param stringForm is the given string.
	 */
	public Categories(String stringForm)
	{
		categories=new ArrayList<String>();
		String test=stringForm;
		int start=3;
		int end=4;
		char temp=test.charAt(4);
		while(temp!=']')
		{
			while(temp!='>' && temp!=']')
			{
				end++;
				temp=test.charAt(end);
			}
			if(temp=='>')
				categories.add(test.substring(start+1, end-1));
			else
				categories.add(test.substring(start+1, end-2));
			
			if(temp!=']')
			{
				end+=2;
				start=end;
				temp=test.charAt(end);
			}
		}
	}
	
	/**
	 * A constructor that retrieves the categories from the given arraylist
	 * @param theList is the given arraylist
	 */
	public Categories(ArrayList<String> theList)
	{
		categories=theList;
	}
	
	/**
	 * Converts the categories in a text form
	 * @return the text form of the categories
	 */
	public String convertToText()
	{
		StringBuffer result=new StringBuffer();
		result.append("\"[\"\"");
		for(int i=0; i<categories.size()-1; ++i)
			result.append(categories.get(i)+" >> ");
		result.append(categories.get(categories.size()-1)+"\"\"]\"");
		
		return result.toString();
	}
	
	/**
	 * @return true if the searchedCat is one of the categories, false if not
	 */
	public boolean contains(String searchedCat)
	{
		return categories.contains(searchedCat);
	}
}
