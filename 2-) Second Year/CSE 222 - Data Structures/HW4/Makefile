
JFLAGS = -g
JC = javac
.SUFFIXES: .java .class
.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
	BinarySearchTree.java \
	BSTHeapTree.java \
	DriverCode.java \
	HelperMethods.java \
	MaxHeap.java \
	MaxHeapIterator.java

default: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) *.class
