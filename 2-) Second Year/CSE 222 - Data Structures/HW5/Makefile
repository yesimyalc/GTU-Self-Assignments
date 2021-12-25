
JFLAGS = -g
JC = javac
.SUFFIXES: .java .class
.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
	ChainHashMap.java \
	ChainHashMap2.java \
	CoalescedHashMap.java \
	DriverCode.java \
	IterableHashMap.java \
	KWHashMap.java

default: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) *.class
