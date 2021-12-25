#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

//Tests if the set is POSET, if not makes it a POSET
void posetTest(int& rCount, vector< vector<char> >& testSubSet, vector<char>& testMainSet);

//Sets the relations to draw the hasse diagram
void drawHasse(int& rCount, vector< vector<char> >& testSubSet);

int main()
{
    vector<char> mainSet;//Will store the set elements
    vector< vector<char> > subSets;//Will store the relation sets' elements
    int relationCount;
    char readCharacter;//This is for reading characters from the input file

    ifstream readSet;
    readSet.open("input.txt");//Opening the input file and checking if it can be opened to read from
    if(readSet.fail())
    {
        cerr<<"The file cannot be opened to read."<<endl;
        exit(-1);
    }

    ofstream writeResult;//Opening the output file and checking if it can be opened to write onto
    writeResult.open("output.txt");
    if(writeResult.fail())
    {
        cerr<<"The file cannot be opened to write."<<endl;
        exit(-1);
    }

    /*The loop will not break until the following character that will be read is a newline.
    I chose it as newline because there are two newlines at the end of the input.txt file. I would do it differently if there
    was something different at the end of the input.txt file. So it should work properly with the given input.txt in the homework*/
    while(readSet.peek()!='\n')
    {
       readSet>>relationCount;//Reading how many relations there are in the following set
       readSet.ignore(1000, '\n');

       mainSet.resize(0);//Resizing as zero because there might be things that are stored in mainSet from the previous loops, this will delete them
       readCharacter='0';//Resetting the read character for the next loop
       while(readCharacter!='\n')
       {
           readSet.get(readCharacter);//Reading the elements in the set one by one
           mainSet.push_back(readCharacter);//Registering them into our main set
           readSet.get(readCharacter);//Reading the commas or newlines. If this is a newline it means we have already registered all the elements in the set
       }

       subSets.resize(relationCount);//Resizing the subSets according to the given relationCount. If there were more elements from the previous loops, they will be deleted
       for(int i=0; i<relationCount; ++i)
           subSets[i].resize(2);

       for(int i=0; i<relationCount; ++i)
           for(int j=0; j<2; ++j)
           {
               readSet>>subSets[i][j];//All the relations will be stored as a subset of the main set. i is for the relation number and j is for the element number in the relation
               for(int k=0; k<mainSet.size(); ++k)//Checking the validity of the elements in the relations
               {
                   if(subSets[i][j]==mainSet[k])//If the element is inside the main set, it means it is a valid element
                       break;
                   else if(k==mainSet.size()-1)//If the element cannot be found in the main set, it means it is an invalid element.
                   {
                       cerr<<"The element in the subset is not found in the relation set. Invalid subset element."<<endl;
                       exit(-1);
                   }
               }
               readSet.get(readCharacter);//To read the commas and the newlines
           }

        posetTest(relationCount, subSets, mainSet);

        //Starting to write to the file
        writeResult<<"n"<<endl<<
        "POSET: ";
        for(int i=0; i<relationCount; ++i)
           writeResult<<"("<<subSets[i][0]<<","<<subSets[i][1]<<")";//Printing out the relations
        writeResult<<endl;

        //Removing the reflexive and transitive relations in the set
        drawHasse(relationCount, subSets);

        //Continuing to write to the file
        for(int i=0; i<relationCount; ++i)
            writeResult<<subSets[i][0]<<","<<subSets[i][1]<<endl;

    }

    writeResult.close();
    readSet.close();

    return 0;
}

/*Tests for Reflexivity, Symmetricity, Antisymmetricity, Transitivity
If the set is not a POSET, it makes it a POSET*/
void posetTest(int& rCount, vector< vector<char> >& testSubSet, vector<char>& testMainSet)
{
    vector<char> rFound;//Will store found reflexive set elements

    //Reflexive Test
    for(int i=0; i<rCount; ++i)
        if(testSubSet[i][0]==testSubSet[i][1])//Checking if the elements of the relation is the same, identifies the element if it is
        {
            for(int j=0; j<rFound.size(); ++j)
            {
                if(rFound[j]==testSubSet[i][0])//Checks if the element was previously stored in rFound
                        break;
                else if(j==rFound.size()-1)
                    rFound.push_back(testSubSet[i][0]);//The element will be stored in rFound if it was not previously stored
            }
            if(rFound.size()==0)//If there was nothing in rFound from before, it automatically stores the identified element
                rFound.push_back(testSubSet[i][0]);
        }

    //Fixing the Reflexivity if the set is not reflexive
    //If testMainSet's size is equal to rFound size it means the set is reflexive
    if(testMainSet.size()!=rFound.size())
    {
        for(int i=0; i<testMainSet.size(); ++i)
            for(int j=0; j<=rFound.size(); ++j)
            {
                //Checking which elements are missing
                if(j!=rFound.size() && testMainSet[i]==rFound[j])
                    break;
                else if(j==rFound.size())
                {
                    //Putting the missing elements to the testSubSet to make the set reflexive
                    rCount++;
                    testSubSet.resize(rCount);
                    testSubSet[rCount-1].resize(2);
                    for(int k=0; k<2; ++k)
                        testSubSet[rCount-1][k]=testMainSet[i];
                }
            }
    }

    //Symmetric Test
    for(int i=0; i<rCount-1; ++i)
        for(int j=i+1; j<rCount; ++j)
            if(testSubSet[i][0]==testSubSet[j][1] && testSubSet[i][1]==testSubSet[j][0] && i!=j)//Checking for a symmetric relation set for each relation set
            {
                //If there is a symmetric pair of sets, deletes one of them
                testSubSet.erase(testSubSet.begin()+j);
                rCount--;
                j--;
            }

    //No need to check for antisymmetricity because the reflexivity is checked and the set is turned into reflexive if it was not from the start.
    //Moreover, the set is checked for symmetricity and it is turned into non symmetric (aside from the reflexive elements) if it was from the start.
    //If a set does not have any symmetric elements (aside from the reflexive ones), it is an antisymmetric set.

    //Transitive Test
    for(int i=0; i<rCount; ++i)
    {
        for(int j=0; j<rCount; ++j)
        {
            /*Checking if there is a starting condition.
            A starting condition happens if there are sets like (a,b) and (b,c) in the relation*/
            if(testSubSet[i][1]==testSubSet[j][0])
                for(int k=0; k<rCount; ++k)
                {
                    if(testSubSet[k][0]==testSubSet[i][0] && testSubSet[k][1]==testSubSet[j][1])//Checking if there is a (a,c) for the (a,b) and (b,c) (this was an example)
                        break;
                    else if(k==rCount-1)//If there is not, adds it to the testSubSet to make the set transitive
                    {
                        rCount++;
                        testSubSet.resize(rCount);
                        testSubSet[rCount-1].resize(2);
                        testSubSet[rCount-1][0]=testSubSet[i][0];
                        testSubSet[rCount-1][1]=testSubSet[j][1];
                    }
                }
        }
    }
}

//Removes the reflexive and the transitive relations from the set
void drawHasse(int& rCount, vector< vector<char> >& testSubSet)
{
    //Removing the reflexive ones
    for(int i=0; i<rCount; ++i)
        if(testSubSet[i][0]==testSubSet[i][1])
        {
            testSubSet.erase(testSubSet.begin()+i);
            rCount--;
            i--;
        }

    //Removing the transitive ones
    for(int i=0; i<rCount; ++i)
    {
        for(int j=0; j<rCount; ++j)
        {
            /*Checking if there is a starting condition.
            A starting condition happens if there are sets like (a,b) and (b,c) in the relation*/
            if(testSubSet[i][1]==testSubSet[j][0])
                for(int k=0; k<rCount; ++k)
                    if(testSubSet[k][0]==testSubSet[i][0] && testSubSet[k][1]==testSubSet[j][1])//Checking if there is a (a,c) for the (a,b) and (b,c) and erasing it(this was an example)
                    {
                        testSubSet.erase(testSubSet.begin()+k);
                        rCount--;
                        k--;
                        i--;
                    }
        }
    }
}
