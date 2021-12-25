
/**
*An interface that has the common methods of Branch and Online Store classes.
*/
public interface Store extends Cloneable
{
	/**
	 * @return the store number
	 */
	public int getNo();
	
	/**
	 * Prints the stock of the store.
	 */
	public void printStock();
	
	/**
	 * Changes the amount of the product that will be given as addedProduct.
	 * @param addedProduct is the product that will be added to/remove from the stock of the store.
	 * @param amount determines how many will be removed or added to the stock.
	 */
	public void changeStock(Product addedProduct, int amount);
	
	/**
	 * Searchs for a product in the stock of the store.
	 * @param searchedProduct is the product that will be searched in the stock of the store
	 * @return the index that the product is stored in the store, if there is at least 1 one it.
	 * @return -1 if the product does not exist in the stock.
	 */
	public int isInStock(Product searchedProduct);
	
	/**
	 * Makes a copy of the store.
	 */
	public Object clone() throws CloneNotSupportedException;
	
	/**
	 * Returns a product from the stock with the given index.
	 * @param index is an integer that will be the index of the product that will be searched in the stock of the store. It cannot be less than 0 or bigger than 118.
	 * @return the product that is inside the stock with the given index value.
	 * @throws InvalidIndex if the given index does not meet the requirements.
	 */
	public Product getProduct(int index) throws InvalidIndex;
	
	/**
	 * Changes the number of the store.
	 * @param newNo will be the new number of the store.
	 */
	public void changeNo(int newNo);
}
