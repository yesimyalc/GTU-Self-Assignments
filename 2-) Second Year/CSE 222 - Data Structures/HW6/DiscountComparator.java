import java.util.Comparator;

/**
 * A comparator class that compares 2 Products according to their discount percentage
 */
public class DiscountComparator implements Comparator
{
	/**
	 * Compares the product with another product.
	 * @return an integer greater than 0 if this.discount>other.discount, smaller than 0 if this.discount<otherdiscount, o if this.discount=other.discount
	 */
	public int compare(Object o1, Object o2) 
	{
		Product p1=(Product)o1;
		Product p2=(Product)o2;
		
		if(p1.getDiscount()>p2.getDiscount())
			return 1;
		else if(p1.getDiscount()==p2.getDiscount())
			return 0;
		else 
			return -1;
	}
}
