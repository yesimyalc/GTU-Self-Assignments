
JFLAGS = -g
JC = javac
.SUFFIXES: .java .class
.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
	AVLTree.java \
	BinarySearchTree.java \
	BTree.java \
	DriverCode2.java \
	HelperMethods.java \
	NotRedBlackTreeException.java \
	RedBlackTree.java \
	RotatingBinarySearchTree.java \
	SearchTree.java \
	SkipList.java \
	Tree.java \
	TwoThreeTree.java

default: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) *.class
