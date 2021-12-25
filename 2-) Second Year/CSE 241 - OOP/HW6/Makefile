
JFLAGS = -g
JC = javac
.SUFFIXES: .java .class
.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
	ArrayList.java \
	Collection.java \
	HashSet.java \
	InvalidIndex.java \
	InvalidType.java \
	Iterator.java \
	LinkedList.java \
	List.java \
	mainArrayListTest.java \
	mainHashSetTest.java \
	mainLinkedListTest.java \
	NoElement.java \
	NoNextElement.java \
	Queue.java \
	Set.java

default: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) *.class
