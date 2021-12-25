
JFLAGS = -g
JC = javac
.SUFFIXES: .java .class
.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
	AbstractUser.java \
	Categories.java \
	Customer.java \
	DiscountComparator.java \
	DriverCode.java \
	HeapSort.java \
	InsertionSort.java \
	PriceComparator.java \
	Product.java \
	QuickSort.java \
	ShoppingSystem.java \
	Trader.java \
	User.java

default: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) *.class
