import java.util.Comparator;

/**
 * A comparator class that compares 2 Products according to their current discounter prices
 */
public class PriceComparator implements Comparator
{	
	/**
	 * Compares the product with another product.
	 * @return an integer greater than 0 if this.discountPrice>other.discountPrice, smaller than 0 if this.discountPrice<otherdiscountPrice, o if thisdiscountPrice=otherdiscountPrice
	 */
	public int compare(Object o1, Object o2) 
	{
		Product p1=(Product)o1;
		Product p2=(Product)o2;
		
		if(p1.getDiscountPrice()>p2.getDiscountPrice())
			return 1;
		else if(p1.getDiscountPrice()==p2.getDiscountPrice())
			return 0;
		else 
			return -1;
	}
}
