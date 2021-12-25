
JFLAGS = -g
JC = javac
.SUFFIXES: .java .class
.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
	Admin.java \
	AutomationSystem.java \
	Branch.java \
	BranchEmployee.java \
	CompanySystem.java \
	Customer.java \
	HybridList.java \
	InvalidAmount.java \
	InvalidBranchNumber.java \
	InvalidCapacity.java \
	InvalidColour.java \
	InvalidIndex.java \
	InvalidModel.java \
	InvalidStatus.java \
	InvalidType.java \
	KWArrayList.java \
	LList.java \
	NoElement.java \
	OnlineStore.java \
	Product.java \
	Store.java \
	User.java

default: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) *.class
