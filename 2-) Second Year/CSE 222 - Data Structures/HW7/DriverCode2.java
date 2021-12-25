import java.util.Random;

public class DriverCode2 {

	public static void main(String[] args) 
	{
		Random rand=new Random();
		int upperbound;
		System.out.println("Filling up all the trees.");
		System.out.println("Generating 10000 numbers and inserting them into the XS sized BinarySearchTrees");
		BinarySearchTree<Integer> bstXS1=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstXS2=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstXS3=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstXS4=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstXS5=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstXS6=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstXS7=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstXS8=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstXS9=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstXS10=new BinarySearchTree<Integer>();
		upperbound=50001;
		for(int j=0; j<100000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<10000)
				check=bstXS1.add(number);
			else if(j>=10000 && j<20000)
				check=bstXS2.add(number);
			else if(j>=20000 && j<30000)
				check=bstXS3.add(number);
			else if(j>=30000 && j<40000)
				check=bstXS4.add(number);
			else if(j>=40000 && j<50000)
				check=bstXS5.add(number);
			else if(j>=50000 && j<60000)
				check=bstXS6.add(number);
			else if(j>=60000 && j<70000)
				check=bstXS7.add(number);
			else if(j>=70000 && j<80000)
				check=bstXS8.add(number);
			else if(j>=80000 && j<90000)
				check=bstXS9.add(number);
			else
				check=bstXS10.add(number);
		
			if(check==false)
				j--;
		}
				
		BinarySearchTree<Integer> bstS1=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstS2=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstS3=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstS4=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstS5=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstS6=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstS7=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstS8=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstS9=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstS10=new BinarySearchTree<Integer>();
		System.out.println("Generating 20000 numbers and inserting them into the S sized BinarySearchTrees");
		upperbound=100001;
		for(int j=0; j<200000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<20000)
				check=bstS1.add(number);
			else if(j>=20000 && j<40000)
				check=bstS2.add(number);
			else if(j>=40000 && j<60000)
				check=bstS3.add(number);
			else if(j>=60000 && j<80000)
				check=bstS4.add(number);
			else if(j>=80000 && j<100000)
				check=bstS5.add(number);
			else if(j>=100000 && j<120000)
				check=bstS6.add(number);
			else if(j>=120000 && j<140000)
				check=bstS7.add(number);
			else if(j>=140000 && j<160000)
				check=bstS8.add(number);
			else if(j>=160000 && j<180000)
				check=bstS9.add(number);
			else
				check=bstS10.add(number);
		
			if(check==false)
				j--;
		}
			
		BinarySearchTree<Integer> bstM1=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstM2=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstM3=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstM4=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstM5=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstM6=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstM7=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstM8=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstM9=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstM10=new BinarySearchTree<Integer>();
		System.out.println("Generating 40000 numbers and inserting them into the M sized BinarySearchTrees");
		upperbound=200001;
		for(int j=0; j<400000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<40000)
				check=bstM1.add(number);
			else if(j>=40000 && j<80000)
				check=bstM2.add(number);
			else if(j>=80000 && j<120000)
				check=bstM3.add(number);
			else if(j>=120000 && j<160000)
				check=bstM4.add(number);
			else if(j>=160000 && j<200000)
				check=bstM5.add(number);
			else if(j>=200000 && j<240000)
				check=bstM6.add(number);
			else if(j>=240000 && j<280000)
				check=bstM7.add(number);
			else if(j>=280000 && j<320000)
				check=bstM8.add(number);
			else if(j>=320000 && j<360000)
				check=bstM9.add(number);
			else
				check=bstM10.add(number);
		
			if(check==false)
				j--;
		}
			
		BinarySearchTree<Integer> bstL1=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstL2=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstL3=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstL4=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstL5=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstL6=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstL7=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstL8=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstL9=new BinarySearchTree<Integer>();
		BinarySearchTree<Integer> bstL10=new BinarySearchTree<Integer>();
		System.out.println("Generating 80000 numbers and inserting them into the L sized BinarySearchTrees");
		upperbound=400001;
		for(int j=0; j<800000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<80000)
				check=bstL1.add(number);
			else if(j>=80000 && j<160000)
				check=bstL2.add(number);
			else if(j>=160000 && j<240000)
				check=bstL3.add(number);
			else if(j>=240000 && j<320000)
				check=bstL4.add(number);
			else if(j>=320000 && j<400000)
				check=bstL5.add(number);
			else if(j>=400000 && j<480000)
				check=bstL6.add(number);
			else if(j>=480000 && j<560000)
				check=bstL7.add(number);
			else if(j>=560000 && j<640000)
				check=bstL8.add(number);
			else if(j>=640000 && j<720000)
				check=bstL9.add(number);
			else
				check=bstL10.add(number);
		
			if(check==false)
				j--;
		}
		
		
		BinarySearchTree<Integer> rbtXS1=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtXS2=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtXS3=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtXS4=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtXS5=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtXS6=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtXS7=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtXS8=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtXS9=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtXS10=new RedBlackTree<Integer>();
		System.out.println("Generating 10000 numbers and inserting them into the XS sized RedBlackTrees");
		upperbound=50001;
		for(int j=0; j<100000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<10000)
				check=rbtXS1.add(number);
			else if(j>=10000 && j<20000)
				check=rbtXS2.add(number);
			else if(j>=20000 && j<30000)
				check=rbtXS3.add(number);
			else if(j>=30000 && j<40000)
				check=rbtXS4.add(number);
			else if(j>=40000 && j<50000)
				check=rbtXS5.add(number);
			else if(j>=50000 && j<60000)
				check=rbtXS6.add(number);
			else if(j>=60000 && j<70000)
				check=rbtXS7.add(number);
			else if(j>=70000 && j<80000)
				check=rbtXS8.add(number);
			else if(j>=80000 && j<90000)
				check=rbtXS9.add(number);
			else
				check=rbtXS10.add(number);
		
			if(check==false)
				j--;
		}
		
		BinarySearchTree<Integer> rbtS1=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtS2=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtS3=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtS4=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtS5=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtS6=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtS7=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtS8=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtS9=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtS10=new RedBlackTree<Integer>();
		System.out.println("Generating 20000 numbers and inserting them into the S sized RedBlackTrees");
		upperbound=100001;
		for(int j=0; j<200000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<20000)
				check=rbtS1.add(number);
			else if(j>=20000 && j<40000)
				check=rbtS2.add(number);
			else if(j>=40000 && j<60000)
				check=rbtS3.add(number);
			else if(j>=60000 && j<80000)
				check=rbtS4.add(number);
			else if(j>=80000 && j<100000)
				check=rbtS5.add(number);
			else if(j>=100000 && j<120000)
				check=rbtS6.add(number);
			else if(j>=120000 && j<140000)
				check=rbtS7.add(number);
			else if(j>=140000 && j<160000)
				check=rbtS8.add(number);
			else if(j>=160000 && j<180000)
				check=rbtS9.add(number);
			else
				check=rbtS10.add(number);
		
			if(check==false)
				j--;
		}
		
		BinarySearchTree<Integer> rbtM1=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtM2=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtM3=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtM4=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtM5=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtM6=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtM7=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtM8=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtM9=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtM10=new RedBlackTree<Integer>();
		System.out.println("Generating 40000 numbers and inserting them into the M sized RedBlackTrees");
		upperbound=200001;
		for(int j=0; j<400000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<40000)
				check=rbtM1.add(number);
			else if(j>=40000 && j<80000)
				check=rbtM2.add(number);
			else if(j>=80000 && j<120000)
				check=rbtM3.add(number);
			else if(j>=120000 && j<160000)
				check=rbtM4.add(number);
			else if(j>=160000 && j<200000)
				check=rbtM5.add(number);
			else if(j>=200000 && j<240000)
				check=rbtM6.add(number);
			else if(j>=240000 && j<280000)
				check=rbtM7.add(number);
			else if(j>=280000 && j<320000)
				check=rbtM8.add(number);
			else if(j>=320000 && j<360000)
				check=rbtM9.add(number);
			else
				check=rbtM10.add(number);
		
			if(check==false)
				j--;
		}
		
		BinarySearchTree<Integer> rbtL1=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtL2=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtL3=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtL4=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtL5=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtL6=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtL7=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtL8=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtL9=new RedBlackTree<Integer>();
		BinarySearchTree<Integer> rbtL10=new RedBlackTree<Integer>();
		System.out.println("Generating 80000 numbers and inserting them into the L sized RedBlackTrees");
		upperbound=400001;
		for(int j=0; j<800000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<80000)
				check=rbtL1.add(number);
			else if(j>=80000 && j<160000)
				check=rbtL2.add(number);
			else if(j>=160000 && j<240000)
				check=rbtL3.add(number);
			else if(j>=240000 && j<320000)
				check=rbtL4.add(number);
			else if(j>=320000 && j<400000)
				check=rbtL5.add(number);
			else if(j>=400000 && j<480000)
				check=rbtL6.add(number);
			else if(j>=480000 && j<560000)
				check=rbtL7.add(number);
			else if(j>=560000 && j<640000)
				check=rbtL8.add(number);
			else if(j>=640000 && j<720000)
				check=rbtL9.add(number);
			else
				check=rbtL10.add(number);
		
			if(check==false)
				j--;
		}

		
		TwoThreeTree<Integer> tttXS1=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttXS2=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttXS3=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttXS4=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttXS5=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttXS6=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttXS7=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttXS8=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttXS9=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttXS10=new TwoThreeTree<Integer>();
		System.out.println("Generating 10000 numbers and inserting them into the XS sized TwoThreeTrees");
		upperbound=50001;
		for(int j=0; j<100000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<10000)
				check=tttXS1.add(number);
			else if(j>=10000 && j<20000)
				check=tttXS2.add(number);
			else if(j>=20000 && j<30000)
				check=tttXS3.add(number);
			else if(j>=30000 && j<40000)
				check=tttXS4.add(number);
			else if(j>=40000 && j<50000)
				check=tttXS5.add(number);
			else if(j>=50000 && j<60000)
				check=tttXS6.add(number);
			else if(j>=60000 && j<70000)
				check=tttXS7.add(number);
			else if(j>=70000 && j<80000)
				check=tttXS8.add(number);
			else if(j>=80000 && j<90000)
				check=tttXS9.add(number);
			else
				check=tttXS10.add(number);
		
			if(check==false)
				j--;
		}
		
		TwoThreeTree<Integer> tttS1=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttS2=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttS3=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttS4=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttS5=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttS6=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttS7=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttS8=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttS9=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttS10=new TwoThreeTree<Integer>();
		System.out.println("Generating 20000 numbers and inserting them into the S sized TwoThreeTrees");
		upperbound=100001;
		for(int j=0; j<200000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<20000)
				check=tttS1.add(number);
			else if(j>=20000 && j<40000)
				check=tttS2.add(number);
			else if(j>=40000 && j<60000)
				check=tttS3.add(number);
			else if(j>=60000 && j<80000)
				check=tttS4.add(number);
			else if(j>=80000 && j<100000)
				check=tttS5.add(number);
			else if(j>=100000 && j<120000)
				check=tttS6.add(number);
			else if(j>=120000 && j<140000)
				check=tttS7.add(number);
			else if(j>=140000 && j<160000)
				check=tttS8.add(number);
			else if(j>=160000 && j<180000)
				check=tttS9.add(number);
			else
				check=tttS10.add(number);
		
			if(check==false)
				j--;
		}
		
		TwoThreeTree<Integer> tttM1=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttM2=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttM3=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttM4=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttM5=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttM6=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttM7=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttM8=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttM9=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttM10=new TwoThreeTree<Integer>();
		System.out.println("Generating 40000 numbers and inserting them into the M sized TwoThreeTrees");
		upperbound=200001;
		for(int j=0; j<400000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<40000)
				check=tttM1.add(number);
			else if(j>=40000 && j<80000)
				check=tttM2.add(number);
			else if(j>=80000 && j<120000)
				check=tttM3.add(number);
			else if(j>=120000 && j<160000)
				check=tttM4.add(number);
			else if(j>=160000 && j<200000)
				check=tttM5.add(number);
			else if(j>=200000 && j<240000)
				check=tttM6.add(number);
			else if(j>=240000 && j<280000)
				check=tttM7.add(number);
			else if(j>=280000 && j<320000)
				check=tttM8.add(number);
			else if(j>=320000 && j<360000)
				check=tttM9.add(number);
			else
				check=tttM10.add(number);
		
			if(check==false)
				j--;
		}
		
		TwoThreeTree<Integer> tttL1=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttL2=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttL3=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttL4=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttL5=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttL6=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttL7=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttL8=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttL9=new TwoThreeTree<Integer>();
		TwoThreeTree<Integer> tttL10=new TwoThreeTree<Integer>();
		System.out.println("Generating 80000 numbers and inserting them into the L sized TwoThreeTrees");
		upperbound=400001;
		for(int j=0; j<800000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<80000)
				check=tttL1.add(number);
			else if(j>=80000 && j<160000)
				check=tttL2.add(number);
			else if(j>=160000 && j<240000)
				check=tttL3.add(number);
			else if(j>=240000 && j<320000)
				check=tttL4.add(number);
			else if(j>=320000 && j<400000)
				check=tttL5.add(number);
			else if(j>=400000 && j<480000)
				check=tttL6.add(number);
			else if(j>=480000 && j<560000)
				check=tttL7.add(number);
			else if(j>=560000 && j<640000)
				check=tttL8.add(number);
			else if(j>=640000 && j<720000)
				check=tttL9.add(number);
			else
				check=tttL10.add(number);
		
			if(check==false)
				j--;
		}
		
		
		BTree<Integer> btXS1=new BTree<Integer>(7);
		BTree<Integer> btXS2=new BTree<Integer>(7);
		BTree<Integer> btXS3=new BTree<Integer>(7);
		BTree<Integer> btXS4=new BTree<Integer>(7);
		BTree<Integer> btXS5=new BTree<Integer>(7);
		BTree<Integer> btXS6=new BTree<Integer>(7);
		BTree<Integer> btXS7=new BTree<Integer>(7);
		BTree<Integer> btXS8=new BTree<Integer>(7);
		BTree<Integer> btXS9=new BTree<Integer>(7);
		BTree<Integer> btXS10=new BTree<Integer>(7);
		System.out.println("Generating 10000 numbers and inserting them into the XS sized order 7 BTrees");
		upperbound=50001;
		for(int j=0; j<100000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<10000)
				check=btXS1.add(number);
			else if(j>=10000 && j<20000)
				check=btXS2.add(number);
			else if(j>=20000 && j<30000)
				check=btXS3.add(number);
			else if(j>=30000 && j<40000)
				check=btXS4.add(number);
			else if(j>=40000 && j<50000)
				check=btXS5.add(number);
			else if(j>=50000 && j<60000)
				check=btXS6.add(number);
			else if(j>=60000 && j<70000)
				check=btXS7.add(number);
			else if(j>=70000 && j<80000)
				check=btXS8.add(number);
			else if(j>=80000 && j<90000)
				check=btXS9.add(number);
			else
				check=btXS10.add(number);
		
			if(check==false)
				j--;
		}
			
		BTree<Integer> btS1=new BTree<Integer>(7);
		BTree<Integer> btS2=new BTree<Integer>(7);
		BTree<Integer> btS3=new BTree<Integer>(7);
		BTree<Integer> btS4=new BTree<Integer>(7);
		BTree<Integer> btS5=new BTree<Integer>(7);
		BTree<Integer> btS6=new BTree<Integer>(7);
		BTree<Integer> btS7=new BTree<Integer>(7);
		BTree<Integer> btS8=new BTree<Integer>(7);
		BTree<Integer> btS9=new BTree<Integer>(7);
		BTree<Integer> btS10=new BTree<Integer>(7);
		System.out.println("Generating 20000 numbers and inserting them into the S sized order 7 BTrees");
		upperbound=100001;
		for(int j=0; j<200000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<20000)
				check=btS1.add(number);
			else if(j>=20000 && j<40000)
				check=btS2.add(number);
			else if(j>=40000 && j<60000)
				check=btS3.add(number);
			else if(j>=60000 && j<80000)
				check=btS4.add(number);
			else if(j>=80000 && j<100000)
				check=btS5.add(number);
			else if(j>=100000 && j<120000)
				check=btS6.add(number);
			else if(j>=120000 && j<140000)
				check=btS7.add(number);
			else if(j>=140000 && j<160000)
				check=btS8.add(number);
			else if(j>=160000 && j<180000)
				check=btS9.add(number);
			else
				check=btS10.add(number);
		
			if(check==false)
				j--;
		}
		
		BTree<Integer> btM1=new BTree<Integer>(7);
		BTree<Integer> btM2=new BTree<Integer>(7);
		BTree<Integer> btM3=new BTree<Integer>(7);
		BTree<Integer> btM4=new BTree<Integer>(7);
		BTree<Integer> btM5=new BTree<Integer>(7);
		BTree<Integer> btM6=new BTree<Integer>(7);
		BTree<Integer> btM7=new BTree<Integer>(7);
		BTree<Integer> btM8=new BTree<Integer>(7);
		BTree<Integer> btM9=new BTree<Integer>(7);
		BTree<Integer> btM10=new BTree<Integer>(7);
		System.out.println("Generating 40000 numbers and inserting them into the M sized order 7 BTrees");
		upperbound=200001;
		for(int j=0; j<400000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<40000)
				check=btM1.add(number);
			else if(j>=40000 && j<80000)
				check=btM2.add(number);
			else if(j>=80000 && j<120000)
				check=btM3.add(number);
			else if(j>=120000 && j<160000)
				check=btM4.add(number);
			else if(j>=160000 && j<200000)
				check=btM5.add(number);
			else if(j>=200000 && j<240000)
				check=btM6.add(number);
			else if(j>=240000 && j<280000)
				check=btM7.add(number);
			else if(j>=280000 && j<320000)
				check=btM8.add(number);
			else if(j>=320000 && j<360000)
				check=btM9.add(number);
			else
				check=btM10.add(number);
		
			if(check==false)
				j--;
		}
		
		BTree<Integer> btL1=new BTree<Integer>(7);
		BTree<Integer> btL2=new BTree<Integer>(7);
		BTree<Integer> btL3=new BTree<Integer>(7);
		BTree<Integer> btL4=new BTree<Integer>(7);
		BTree<Integer> btL5=new BTree<Integer>(7);
		BTree<Integer> btL6=new BTree<Integer>(7);
		BTree<Integer> btL7=new BTree<Integer>(7);
		BTree<Integer> btL8=new BTree<Integer>(7);
		BTree<Integer> btL9=new BTree<Integer>(7);
		BTree<Integer> btL10=new BTree<Integer>(7);
		System.out.println("Generating 80000 numbers and inserting them into the L sized order 7 BTrees");
		upperbound=400001;
		for(int j=0; j<800000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<80000)
				check=btL1.add(number);
			else if(j>=80000 && j<160000)
				check=btL2.add(number);
			else if(j>=160000 && j<240000)
				check=btL3.add(number);
			else if(j>=240000 && j<320000)
				check=btL4.add(number);
			else if(j>=320000 && j<400000)
				check=btL5.add(number);
			else if(j>=400000 && j<480000)
				check=btL6.add(number);
			else if(j>=480000 && j<560000)
				check=btL7.add(number);
			else if(j>=560000 && j<640000)
				check=btL8.add(number);
			else if(j>=640000 && j<720000)
				check=btL9.add(number);
			else
				check=btL10.add(number);
		
			if(check==false)
				j--;
		}
		
		
		SkipList<Integer> slXS1=new SkipList<Integer>();
		SkipList<Integer> slXS2=new SkipList<Integer>();
		SkipList<Integer> slXS3=new SkipList<Integer>();
		SkipList<Integer> slXS4=new SkipList<Integer>();
		SkipList<Integer> slXS5=new SkipList<Integer>();
		SkipList<Integer> slXS6=new SkipList<Integer>();
		SkipList<Integer> slXS7=new SkipList<Integer>();
		SkipList<Integer> slXS8=new SkipList<Integer>();
		SkipList<Integer> slXS9=new SkipList<Integer>();
		SkipList<Integer> slXS10=new SkipList<Integer>();
		System.out.println("Generating 10000 numbers and inserting them into the XS sized SkipList");
		upperbound=50001;
		for(int j=0; j<100000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<10000)
				check=slXS1.add(number);
			else if(j>=10000 && j<20000)
				check=slXS2.add(number);
			else if(j>=20000 && j<30000)
				check=slXS3.add(number);
			else if(j>=30000 && j<40000)
				check=slXS4.add(number);
			else if(j>=40000 && j<50000)
				check=slXS5.add(number);
			else if(j>=50000 && j<60000)
				check=slXS6.add(number);
			else if(j>=60000 && j<70000)
				check=slXS7.add(number);
			else if(j>=70000 && j<80000)
				check=slXS8.add(number);
			else if(j>=80000 && j<90000)
				check=slXS9.add(number);
			else
				check=slXS10.add(number);
		
			if(check==false)
				j--;
		}
		
		SkipList<Integer> slS1=new SkipList<Integer>();
		SkipList<Integer> slS2=new SkipList<Integer>();
		SkipList<Integer> slS3=new SkipList<Integer>();
		SkipList<Integer> slS4=new SkipList<Integer>();
		SkipList<Integer> slS5=new SkipList<Integer>();
		SkipList<Integer> slS6=new SkipList<Integer>();
		SkipList<Integer> slS7=new SkipList<Integer>();
		SkipList<Integer> slS8=new SkipList<Integer>();
		SkipList<Integer> slS9=new SkipList<Integer>();
		SkipList<Integer> slS10=new SkipList<Integer>();
		System.out.println("Generating 20000 numbers and inserting them into the S sized SkipList");
		upperbound=100001;
		for(int j=0; j<200000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<20000)
				check=slS1.add(number);
			else if(j>=20000 && j<40000)
				check=slS2.add(number);
			else if(j>=40000 && j<60000)
				check=slS3.add(number);
			else if(j>=60000 && j<80000)
				check=slS4.add(number);
			else if(j>=80000 && j<100000)
				check=slS5.add(number);
			else if(j>=100000 && j<120000)
				check=slS6.add(number);
			else if(j>=120000 && j<140000)
				check=slS7.add(number);
			else if(j>=140000 && j<160000)
				check=slS8.add(number);
			else if(j>=160000 && j<180000)
				check=slS9.add(number);
			else
				check=slS10.add(number);
		
			if(check==false)
				j--;
		}
		
		SkipList<Integer> slM1=new SkipList<Integer>();
		SkipList<Integer> slM2=new SkipList<Integer>();
		SkipList<Integer> slM3=new SkipList<Integer>();
		SkipList<Integer> slM4=new SkipList<Integer>();
		SkipList<Integer> slM5=new SkipList<Integer>();
		SkipList<Integer> slM6=new SkipList<Integer>();
		SkipList<Integer> slM7=new SkipList<Integer>();
		SkipList<Integer> slM8=new SkipList<Integer>();
		SkipList<Integer> slM9=new SkipList<Integer>();
		SkipList<Integer> slM10=new SkipList<Integer>();
		System.out.println("Generating 40000 numbers and inserting them into the M sized SkipList");
		upperbound=200001;
		for(int j=0; j<400000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<40000)
				check=slM1.add(number);
			else if(j>=40000 && j<80000)
				check=slM2.add(number);
			else if(j>=80000 && j<120000)
				check=slM3.add(number);
			else if(j>=120000 && j<160000)
				check=slM4.add(number);
			else if(j>=160000 && j<200000)
				check=slM5.add(number);
			else if(j>=200000 && j<240000)
				check=slM6.add(number);
			else if(j>=240000 && j<280000)
				check=slM7.add(number);
			else if(j>=280000 && j<320000)
				check=slM8.add(number);
			else if(j>=320000 && j<360000)
				check=slM9.add(number);
			else
				check=slM10.add(number);
		
			if(check==false)
				j--;
		}
		
		SkipList<Integer> slL1=new SkipList<Integer>();
		SkipList<Integer> slL2=new SkipList<Integer>();
		SkipList<Integer> slL3=new SkipList<Integer>();
		SkipList<Integer> slL4=new SkipList<Integer>();
		SkipList<Integer> slL5=new SkipList<Integer>();
		SkipList<Integer> slL6=new SkipList<Integer>();
		SkipList<Integer> slL7=new SkipList<Integer>();
		SkipList<Integer> slL8=new SkipList<Integer>();
		SkipList<Integer> slL9=new SkipList<Integer>();
		SkipList<Integer> slL10=new SkipList<Integer>();
		System.out.println("Generating 80000 numbers and inserting them into the L sized SkipList");
		upperbound=400001;
		for(int j=0; j<800000; ++j)
		{
			Integer number=rand.nextInt(upperbound);
			boolean check=true;
			if(j>=0 && j<80000)
				check=slL1.add(number);
			else if(j>=80000 && j<160000)
				check=slL2.add(number);
			else if(j>=160000 && j<240000)
				check=slL3.add(number);
			else if(j>=240000 && j<320000)
				check=slL4.add(number);
			else if(j>=320000 && j<400000)
				check=slL5.add(number);
			else if(j>=400000 && j<480000)
				check=slL6.add(number);
			else if(j>=480000 && j<560000)
				check=slL7.add(number);
			else if(j>=560000 && j<640000)
				check=slL8.add(number);
			else if(j>=640000 && j<720000)
				check=slL9.add(number);
			else
				check=slL10.add(number);
		
			if(check==false)
				j--;
		}
		

		int lowerbound;
		System.out.println("\n*****TEST START*****");
		System.out.println("Inserting 100 extra elements to all the BinarySearchTrees and calculating the average running time.");
		System.out.println("Testing XS Sized (10.000 elements) BinarySearchTree: ");
		long startTimeBSTXS = System.nanoTime();
		lowerbound=50001;
		upperbound=lowerbound+500;
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=bstXS1.add(number);
			else if(j>=100 && j<200)
				check=bstXS2.add(number);
			else if(j>=200 && j<300)
				check=bstXS3.add(number);
			else if(j>=300 && j<400)
				check=bstXS4.add(number);
			else if(j>=400 && j<500)
				check=bstXS5.add(number);
			else if(j>=500 && j<600)
				check=bstXS6.add(number);
			else if(j>=600 && j<700)
				check=bstXS7.add(number);
			else if(j>=700 && j<800)
				check=bstXS8.add(number);
			else if(j>=800 && j<900)
				check=bstXS9.add(number);
			else
				check=bstXS10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeBSTXS   = System.nanoTime();
		long totalTimeBSTXS = endTimeBSTXS - startTimeBSTXS;
		System.out.println("->Total time of the process is: "+totalTimeBSTXS+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeBSTXS/10+" ns.");
		
		System.out.println("Testing S Sized (20.000 elements) BinarySearchTree: ");
		lowerbound=100001;
		upperbound=lowerbound+500;
		long startTimeBSTS = System.nanoTime();
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=bstS1.add(number);
			else if(j>=100 && j<200)
				check=bstS2.add(number);
			else if(j>=200 && j<300)
				check=bstS3.add(number);
			else if(j>=300 && j<400)
				check=bstS4.add(number);
			else if(j>=400 && j<500)
				check=bstS5.add(number);
			else if(j>=500 && j<600)
				check=bstS6.add(number);
			else if(j>=600 && j<700)
				check=bstS7.add(number);
			else if(j>=700 && j<800)
				check=bstS8.add(number);
			else if(j>=800 && j<900)
				check=bstS9.add(number);
			else
				check=bstS10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeBSTS   = System.nanoTime();
		long totalTimeBSTS = endTimeBSTS - startTimeBSTS;
		System.out.println("->Total time of the process is: "+totalTimeBSTS+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeBSTS/10+" ns.");
		
		System.out.println("Testing M Sized (40.000 elements) BinarySearchTree: ");
		lowerbound=200001;
		upperbound=lowerbound+500;
		long startTimeBSTM = System.nanoTime();
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=bstM1.add(number);
			else if(j>=100 && j<200)
				check=bstM2.add(number);
			else if(j>=200 && j<300)
				check=bstM3.add(number);
			else if(j>=300 && j<400)
				check=bstM4.add(number);
			else if(j>=400 && j<500)
				check=bstM5.add(number);
			else if(j>=500 && j<600)
				check=bstM6.add(number);
			else if(j>=600 && j<700)
				check=bstM7.add(number);
			else if(j>=700 && j<800)
				check=bstM8.add(number);
			else if(j>=800 && j<900)
				check=bstM9.add(number);
			else
				check=bstM10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeBSTM  = System.nanoTime();
		long totalTimeBSTM = endTimeBSTM - startTimeBSTM;
		System.out.println("->Total time of the process is: "+totalTimeBSTM+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeBSTM/10+" ns.");
		
		System.out.println("Testing L Sized (80.000 elements) BinarySearchTree: ");
		lowerbound=400001;
		upperbound=lowerbound+500;
		long startTimeBSTL = System.nanoTime();
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=bstL1.add(number);
			else if(j>=100 && j<200)
				check=bstL2.add(number);
			else if(j>=200 && j<300)
				check=bstL3.add(number);
			else if(j>=300 && j<400)
				check=bstL4.add(number);
			else if(j>=400 && j<500)
				check=bstL5.add(number);
			else if(j>=500 && j<600)
				check=bstL6.add(number);
			else if(j>=600 && j<700)
				check=bstL7.add(number);
			else if(j>=700 && j<800)
				check=bstL8.add(number);
			else if(j>=800 && j<900)
				check=bstL9.add(number);
			else
				check=bstL10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeBSTL  = System.nanoTime();
		long totalTimeBSTL = endTimeBSTL - startTimeBSTL;
		System.out.println("->Total time of the process is: "+totalTimeBSTL+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeBSTL/10+" ns.");
		
		
		System.out.println("\nInserting 100 extra elements to all the RedBlackTrees and calculating the average running time.");
		System.out.println("Testing XS Sized (10.000 elements) RedBlackTree: ");
		long startTimeRBTXS = System.nanoTime();
		lowerbound=50001;
		upperbound=lowerbound+500;
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=rbtXS1.add(number);
			else if(j>=100 && j<200)
				check=rbtXS2.add(number);
			else if(j>=200 && j<300)
				check=rbtXS3.add(number);
			else if(j>=300 && j<400)
				check=rbtXS4.add(number);
			else if(j>=400 && j<500)
				check=rbtXS5.add(number);
			else if(j>=500 && j<600)
				check=rbtXS6.add(number);
			else if(j>=600 && j<700)
				check=rbtXS7.add(number);
			else if(j>=700 && j<800)
				check=rbtXS8.add(number);
			else if(j>=800 && j<900)
				check=rbtXS9.add(number);
			else
				check=rbtXS10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeRBTXS   = System.nanoTime();
		long totalTimeRBTXS = endTimeRBTXS - startTimeRBTXS;
		System.out.println("->Total time of the process is: "+totalTimeRBTXS+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeRBTXS/10+" ns.");
		
		System.out.println("Testing S Sized (20.000 elements) RedBlackTree: ");
		lowerbound=100001;
		upperbound=lowerbound+500;
		long startTimeRBTS = System.nanoTime();
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=rbtS1.add(number);
			else if(j>=100 && j<200)
				check=rbtS2.add(number);
			else if(j>=200 && j<300)
				check=rbtS3.add(number);
			else if(j>=300 && j<400)
				check=rbtS4.add(number);
			else if(j>=400 && j<500)
				check=rbtS5.add(number);
			else if(j>=500 && j<600)
				check=rbtS6.add(number);
			else if(j>=600 && j<700)
				check=rbtS7.add(number);
			else if(j>=700 && j<800)
				check=rbtS8.add(number);
			else if(j>=800 && j<900)
				check=rbtS9.add(number);
			else
				check=rbtS10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeRBTS   = System.nanoTime();
		long totalTimeRBTS = endTimeRBTS - startTimeRBTS;
		System.out.println("->Total time of the process is: "+totalTimeRBTS+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeRBTS/10+" ns.");
		
		System.out.println("Testing M Sized (40.000 elements) RedBlackTree: ");
		lowerbound=200001;
		upperbound=lowerbound+500;
		long startTimeRBTM = System.nanoTime();
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=rbtM1.add(number);
			else if(j>=100 && j<200)
				check=rbtM2.add(number);
			else if(j>=200 && j<300)
				check=rbtM3.add(number);
			else if(j>=300 && j<400)
				check=rbtM4.add(number);
			else if(j>=400 && j<500)
				check=rbtM5.add(number);
			else if(j>=500 && j<600)
				check=rbtM6.add(number);
			else if(j>=600 && j<700)
				check=rbtM7.add(number);
			else if(j>=700 && j<800)
				check=rbtM8.add(number);
			else if(j>=800 && j<900)
				check=rbtM9.add(number);
			else
				check=rbtM10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeRBTM  = System.nanoTime();
		long totalTimeRBTM = endTimeRBTM - startTimeRBTM;
		System.out.println("->Total time of the process is: "+totalTimeRBTM+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeRBTM/10+" ns.");
		
		System.out.println("Testing L Sized (80.000 elements) RedBlackTree: ");
		lowerbound=400001;
		upperbound=lowerbound+500;
		long startTimeRBTL = System.nanoTime();
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=rbtL1.add(number);
			else if(j>=100 && j<200)
				check=rbtL2.add(number);
			else if(j>=200 && j<300)
				check=rbtL3.add(number);
			else if(j>=300 && j<400)
				check=rbtL4.add(number);
			else if(j>=400 && j<500)
				check=rbtL5.add(number);
			else if(j>=500 && j<600)
				check=rbtL6.add(number);
			else if(j>=600 && j<700)
				check=rbtL7.add(number);
			else if(j>=700 && j<800)
				check=rbtL8.add(number);
			else if(j>=800 && j<900)
				check=rbtL9.add(number);
			else
				check=rbtL10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeRBTL  = System.nanoTime();
		long totalTimeRBTL = endTimeRBTL - startTimeRBTL;
		System.out.println("->Total time of the process is: "+totalTimeRBTL+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeRBTL/10+" ns.");
		
		
		System.out.println("\nInserting 100 extra elements to all the TwoThreeTrees and calculating the average running time.");
		System.out.println("Testing XS Sized (10.000 elements) TwoThreeTree: ");
		long startTimeTTTXS = System.nanoTime();
		lowerbound=50001;
		upperbound=lowerbound+500;
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=tttXS1.add(number);
			else if(j>=100 && j<200)
				check=tttXS2.add(number);
			else if(j>=200 && j<300)
				check=tttXS3.add(number);
			else if(j>=300 && j<400)
				check=tttXS4.add(number);
			else if(j>=400 && j<500)
				check=tttXS5.add(number);
			else if(j>=500 && j<600)
				check=tttXS6.add(number);
			else if(j>=600 && j<700)
				check=tttXS7.add(number);
			else if(j>=700 && j<800)
				check=tttXS8.add(number);
			else if(j>=800 && j<900)
				check=tttXS9.add(number);
			else
				check=tttXS10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeTTTXS   = System.nanoTime();
		long totalTimeTTTXS = endTimeTTTXS - startTimeTTTXS;
		System.out.println("->Total time of the process is: "+totalTimeTTTXS+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeTTTXS/10+" ns.");
		
		System.out.println("Testing S Sized (20.000 elements) TwoThreeTree: ");
		lowerbound=100001;
		upperbound=lowerbound+500;
		long startTimeTTTS = System.nanoTime();
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=tttS1.add(number);
			else if(j>=100 && j<200)
				check=tttS2.add(number);
			else if(j>=200 && j<300)
				check=tttS3.add(number);
			else if(j>=300 && j<400)
				check=tttS4.add(number);
			else if(j>=400 && j<500)
				check=tttS5.add(number);
			else if(j>=500 && j<600)
				check=tttS6.add(number);
			else if(j>=600 && j<700)
				check=tttS7.add(number);
			else if(j>=700 && j<800)
				check=tttS8.add(number);
			else if(j>=800 && j<900)
				check=tttS9.add(number);
			else
				check=tttS10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeTTTS   = System.nanoTime();
		long totalTimeTTTS = endTimeTTTS - startTimeTTTS;
		System.out.println("->Total time of the process is: "+totalTimeTTTS+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeTTTS/10+" ns.");
		
		System.out.println("Testing M Sized (40.000 elements) TwoThreeTree: ");
		lowerbound=200001;
		upperbound=lowerbound+500;
		long startTimeTTTM = System.nanoTime();
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=tttM1.add(number);
			else if(j>=100 && j<200)
				check=tttM2.add(number);
			else if(j>=200 && j<300)
				check=tttM3.add(number);
			else if(j>=300 && j<400)
				check=tttM4.add(number);
			else if(j>=400 && j<500)
				check=tttM5.add(number);
			else if(j>=500 && j<600)
				check=tttM6.add(number);
			else if(j>=600 && j<700)
				check=tttM7.add(number);
			else if(j>=700 && j<800)
				check=tttM8.add(number);
			else if(j>=800 && j<900)
				check=tttM9.add(number);
			else
				check=tttM10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeTTTM  = System.nanoTime();
		long totalTimeTTTM = endTimeTTTM - startTimeTTTM;
		System.out.println("->Total time of the process is: "+totalTimeTTTM+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeTTTM/10+" ns.");
		
		System.out.println("Testing L Sized (80.000 elements) TwoThreeTree: ");
		lowerbound=400001;
		upperbound=lowerbound+500;
		long startTimeTTTL = System.nanoTime();
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=tttL1.add(number);
			else if(j>=100 && j<200)
				check=tttL2.add(number);
			else if(j>=200 && j<300)
				check=tttL3.add(number);
			else if(j>=300 && j<400)
				check=tttL4.add(number);
			else if(j>=400 && j<500)
				check=tttL5.add(number);
			else if(j>=500 && j<600)
				check=tttL6.add(number);
			else if(j>=600 && j<700)
				check=tttL7.add(number);
			else if(j>=700 && j<800)
				check=tttL8.add(number);
			else if(j>=800 && j<900)
				check=tttL9.add(number);
			else
				check=tttL10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeTTTL  = System.nanoTime();
		long totalTimeTTTL = endTimeTTTL - startTimeTTTL;
		System.out.println("->Total time of the process is: "+totalTimeTTTL+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeTTTL/10+" ns.");
		
		
		System.out.println("\nInserting 100 extra elements to all the 7 order BTrees and calculating the average running time.");
		System.out.println("Testing XS Sized (10.000 elements) BTree: ");
		long startTimeBTXS = System.nanoTime();
		lowerbound=50001;
		upperbound=lowerbound+500;
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=btXS1.add(number);
			else if(j>=100 && j<200)
				check=btXS2.add(number);
			else if(j>=200 && j<300)
				check=btXS3.add(number);
			else if(j>=300 && j<400)
				check=btXS4.add(number);
			else if(j>=400 && j<500)
				check=btXS5.add(number);
			else if(j>=500 && j<600)
				check=btXS6.add(number);
			else if(j>=600 && j<700)
				check=btXS7.add(number);
			else if(j>=700 && j<800)
				check=btXS8.add(number);
			else if(j>=800 && j<900)
				check=btXS9.add(number);
			else
				check=btXS10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeBTXS   = System.nanoTime();
		long totalTimeBTXS = endTimeBTXS - startTimeBTXS;
		System.out.println("->Total time of the process is: "+totalTimeBTXS+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeBTXS/10+" ns.");
		
		System.out.println("Testing S Sized (20.000 elements) BTree: ");
		lowerbound=100001;
		upperbound=lowerbound+500;
		long startTimeBTS = System.nanoTime();
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=btS1.add(number);
			else if(j>=100 && j<200)
				check=btS2.add(number);
			else if(j>=200 && j<300)
				check=btS3.add(number);
			else if(j>=300 && j<400)
				check=btS4.add(number);
			else if(j>=400 && j<500)
				check=btS5.add(number);
			else if(j>=500 && j<600)
				check=btS6.add(number);
			else if(j>=600 && j<700)
				check=btS7.add(number);
			else if(j>=700 && j<800)
				check=btS8.add(number);
			else if(j>=800 && j<900)
				check=btS9.add(number);
			else
				check=btS10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeBTS   = System.nanoTime();
		long totalTimeBTS = endTimeBTS - startTimeBTS;
		System.out.println("->Total time of the process is: "+totalTimeBTS+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeBTS/10+" ns.");
		
		System.out.println("Testing M Sized (40.000 elements) BTree: ");
		lowerbound=200001;
		upperbound=lowerbound+500;
		long startTimeBTM = System.nanoTime();
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=btM1.add(number);
			else if(j>=100 && j<200)
				check=btM2.add(number);
			else if(j>=200 && j<300)
				check=btM3.add(number);
			else if(j>=300 && j<400)
				check=btM4.add(number);
			else if(j>=400 && j<500)
				check=btM5.add(number);
			else if(j>=500 && j<600)
				check=btM6.add(number);
			else if(j>=600 && j<700)
				check=btM7.add(number);
			else if(j>=700 && j<800)
				check=btM8.add(number);
			else if(j>=800 && j<900)
				check=btM9.add(number);
			else
				check=btM10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeBTM  = System.nanoTime();
		long totalTimeBTM = endTimeBTM - startTimeBTM;
		System.out.println("->Total time of the process is: "+totalTimeBTM+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeBTM/10+" ns.");
		
		System.out.println("Testing L Sized (80.000 elements) BTree: ");
		lowerbound=400001;
		upperbound=lowerbound+500;
		long startTimeBTL = System.nanoTime();
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=btL1.add(number);
			else if(j>=100 && j<200)
				check=btL2.add(number);
			else if(j>=200 && j<300)
				check=btL3.add(number);
			else if(j>=300 && j<400)
				check=btL4.add(number);
			else if(j>=400 && j<500)
				check=btL5.add(number);
			else if(j>=500 && j<600)
				check=btL6.add(number);
			else if(j>=600 && j<700)
				check=btL7.add(number);
			else if(j>=700 && j<800)
				check=btL8.add(number);
			else if(j>=800 && j<900)
				check=btL9.add(number);
			else
				check=btL10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeBTL  = System.nanoTime();
		long totalTimeBTL = endTimeBTL - startTimeBTL;
		System.out.println("->Total time of the process is: "+totalTimeBTL+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeBTL/10+" ns.");
		
		
		System.out.println("\nInserting 100 extra elements to all the SkipLists and calculating the average running time.");
		System.out.println("Testing XS Sized (10.000 elements) SkipList: ");
		long startTimeSLXS = System.nanoTime();
		lowerbound=50001;
		upperbound=lowerbound+500;
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=slXS1.add(number);
			else if(j>=100 && j<200)
				check=slXS2.add(number);
			else if(j>=200 && j<300)
				check=slXS3.add(number);
			else if(j>=300 && j<400)
				check=slXS4.add(number);
			else if(j>=400 && j<500)
				check=slXS5.add(number);
			else if(j>=500 && j<600)
				check=slXS6.add(number);
			else if(j>=600 && j<700)
				check=slXS7.add(number);
			else if(j>=700 && j<800)
				check=slXS8.add(number);
			else if(j>=800 && j<900)
				check=slXS9.add(number);
			else
				check=slXS10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeSLXS   = System.nanoTime();
		long totalTimeSLXS = endTimeSLXS - startTimeSLXS;
		System.out.println("->Total time of the process is: "+totalTimeSLXS+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeSLXS/10+" ns.");
		
		System.out.println("Testing S Sized (20.000 elements) SkipList: ");
		lowerbound=100001;
		upperbound=lowerbound+500;
		long startTimeSLS = System.nanoTime();
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=slS1.add(number);
			else if(j>=100 && j<200)
				check=slS2.add(number);
			else if(j>=200 && j<300)
				check=slS3.add(number);
			else if(j>=300 && j<400)
				check=slS4.add(number);
			else if(j>=400 && j<500)
				check=slS5.add(number);
			else if(j>=500 && j<600)
				check=slS6.add(number);
			else if(j>=600 && j<700)
				check=slS7.add(number);
			else if(j>=700 && j<800)
				check=slS8.add(number);
			else if(j>=800 && j<900)
				check=slS9.add(number);
			else
				check=slS10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeSLS   = System.nanoTime();
		long totalTimeSLS = endTimeSLS - startTimeSLS;
		System.out.println("->Total time of the process is: "+totalTimeSLS+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeSLS/10+" ns.");
		
		System.out.println("Testing M Sized (40.000 elements) SkipList: ");
		lowerbound=200001;
		upperbound=lowerbound+500;
		long startTimeSLM = System.nanoTime();
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=slM1.add(number);
			else if(j>=100 && j<200)
				check=slM2.add(number);
			else if(j>=200 && j<300)
				check=slM3.add(number);
			else if(j>=300 && j<400)
				check=slM4.add(number);
			else if(j>=400 && j<500)
				check=slM5.add(number);
			else if(j>=500 && j<600)
				check=slM6.add(number);
			else if(j>=600 && j<700)
				check=slM7.add(number);
			else if(j>=700 && j<800)
				check=slM8.add(number);
			else if(j>=800 && j<900)
				check=slM9.add(number);
			else
				check=slM10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeSLM  = System.nanoTime();
		long totalTimeSLM = endTimeSLM - startTimeSLM;
		System.out.println("->Total time of the process is: "+totalTimeSLM+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeSLM/10+" ns.");
		
		System.out.println("Testing L Sized (80.000 elements) SkipList: ");
		lowerbound=400001;
		upperbound=lowerbound+500;
		long startTimeSLL = System.nanoTime();
		for(int j=0; j<1000; ++j)
		{
			Integer number=rand.nextInt(upperbound-lowerbound)+lowerbound;
			boolean check=true;
			if(j>=0 && j<100)
				check=slL1.add(number);
			else if(j>=100 && j<200)
				check=slL2.add(number);
			else if(j>=200 && j<300)
				check=slL3.add(number);
			else if(j>=300 && j<400)
				check=slL4.add(number);
			else if(j>=400 && j<500)
				check=slL5.add(number);
			else if(j>=500 && j<600)
				check=slL6.add(number);
			else if(j>=600 && j<700)
				check=slL7.add(number);
			else if(j>=700 && j<800)
				check=slL8.add(number);
			else if(j>=800 && j<900)
				check=slL9.add(number);
			else
				check=slL10.add(number);
		
			if(check==false)
				j--;
		}
		long endTimeSLL  = System.nanoTime();
		long totalTimeSLL = endTimeSLL - startTimeSLL;
		System.out.println("->Total time of the process is: "+totalTimeSLL+" ns.");
		System.out.println("->The average of 1 insertion is: "+totalTimeSLL/10+" ns.");
		
		
		
		System.out.println("\n*****TESTING OF HELPER METHOD*****");
		System.out.println("Testing with a AVLTree");
		BinarySearchTree<Integer> avlt=new AVLTree<Integer>();
		HelperMethods.AVLOrRedBlack(avlt);
		System.out.println("\nTesting with a RedBlackTree");
		HelperMethods.AVLOrRedBlack(rbtXS1);
		System.out.println("\nTesting with a regular BinarySearchTree");
		HelperMethods.AVLOrRedBlack(bstXS1);
		
	}

}
