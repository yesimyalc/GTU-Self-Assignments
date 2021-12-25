#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

int main()
{
    vector<char> mainSet;//Will store the set elements
    vector< vector<char> > subSets;//Will store the relation sets' elements
    int relationCount;
    ifstream readSet;
    char readCharacter;//This is for reading characters from the input file
    int i;
    int j;
    vector<char> rFound;//Will store found reflexive set elements
    string sTest;//Passed if symmetric, Failed if not
    string aTest;//Passed if antisymmetric, Failed if not
    string tTest;//Passed if transitive, Failed if not
    int foundTi;
    int foundTj;
    int foundTk;

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
       for(i=0; i<relationCount; ++i)
           subSets[i].resize(2);
       for(i=0; i<relationCount; ++i)
           for(j=0; j<2; ++j)
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

       //Starting to Write to the File
       writeResult<<"n"<<endl;
       for(i=0; i<relationCount; ++i)
           writeResult<<"("<<subSets[i][0]<<","<<subSets[i][1]<<")";//Printing out the relations

       //Reflexive Test
       rFound.resize(0);//Resizing as zero because there might be things that are stored in rFound from the previous loops, this will delete them
       for(i=0; i<relationCount; ++i)
           for(j=0; j<mainSet.size(); ++j)
               if(subSets[i][0]==subSets[i][1] && subSets[i][0]==mainSet[j])//Checking if the elements of the relation is the same, identifies the element if it is
               {
                   for(int k=0; k<rFound.size(); ++k)
                   {
                       if(rFound[k]==mainSet[j])//Checks if the element was previously stored in rFound
                          break;
                       else if(k==rFound.size()-1)
                          rFound.push_back(mainSet[j]);//The element will be stored in rFound if it was not previously stored
                   }
                   if(rFound.size()==0)//If there was nothing in rFound from before, it automatically stores the identified element
                       rFound.push_back(mainSet[j]);
               }

       if(rFound.size()==mainSet.size())//If all the elements in the main set was identified and stored while searching, it means there is a reflexive relation set for each element
       {
           writeResult<<endl<<"Reflexive: Yes. Because:";
           for(i=0; i<rFound.size(); ++i)
               writeResult<<"("<<rFound[i]<<","<<rFound[i]<<") ";//Printing out all the found reflexive relation sets
           writeResult<<"are found.";
       }
       else//If not all elements in the main set was identified and stored while searching, it means there is not a reflexive relation set for each element
       {
           writeResult<<endl<<"Reflexive: No. Because:";
           for(i=0; i<rFound.size(); ++i)
               writeResult<<"("<<rFound[i]<<","<<rFound[i]<<") ";//Printing out all the found reflexive relation sets
           if(rFound.size()==0)
               writeResult<<"none of the reflexive elements ";
           writeResult<<"are found.";

           for(i=0; i<mainSet.size(); ++i)
               for(j=0; j<rFound.size(); ++j)
               {   //Checking if the element in the main set was identified and stored, if not it means it does not have a reflexive relation set
                   if(rFound[j]==mainSet[i])
                       break;
                   else if(j==rFound.size()-1)
                       writeResult<<" ("<<mainSet[i]<<","<<mainSet[i]<<") is not found.";//Printing out all the missing reflexive relation sets
               }
       }

       //Symmetric Test
       sTest="Passed";

       for(i=0; i<relationCount-1; ++i)
       {
           for(j=0; j<relationCount; ++j)
           {
               if(((subSets[i][0]==subSets[i][1]) || ( subSets[i][0]==subSets[j][1] && subSets[i][1]==subSets[j][0])) && i!=j)//Checking for a symmetric relation set for each relation set
                   break;
               else if(j==relationCount-1)//If there is not a symmetric relation set for even one relation set, it means the relation is not symmetric
                   sTest="Failed";
           }
           if(sTest=="Failed")//Stops checking if there was a relation set with no symmetric
               break;
       }

       if(sTest=="Passed")
       {
           writeResult<<endl<<"Symmetric: Yes. Because all the symmetric elements are found for example:";

           for(i=0; i<relationCount; ++i)//This part is for deciding which pair to print out. Printing out sets with non equal elements are the priority
               if(subSets[i][0]!=subSets[i][1])
                  break;
           if(i==relationCount)//If there is no sets with non equal elements, a set with equal elements will be chosen
               i=0;

           writeResult<<"("<<subSets[i][0]<<","<<subSets[i][1]<<") and ("<<subSets[i][1]<<","<<subSets[i][0]<<") are found."<<endl;//Printing out the chosen set as an example
       }
       else
           writeResult<<endl<<"Symmetric: No. Because:("<<subSets[i][0]<<","<<subSets[i][1]<<") is found but ("<<subSets[i][1]<<","<<subSets[i][0]<<") is not found."<<endl;
           //Printing out one of the missing symmetric elements

       //Antisymmetric Test
       aTest="Passed";

       for(i=0; i<relationCount-1; ++i)
       {
           for(j=0; j<relationCount; ++j)
               if(subSets[i][0]!=subSets[i][1] && subSets[i][0]==subSets[j][1] && subSets[i][1]==subSets[j][0] && i!=j)
               {
                   //If there is a symmetric relation set for any of the sets (excluding the ones with the equal elements), it means the relation is not antisymmetric
                   aTest="Failed";
                   break;
               }
           if(aTest=="Failed")
               break;
       }

       if(aTest=="Passed")
       {
           writeResult<<"Antisymmetric: Yes. Because none of the symmetric elements excluding the ones with the same two values are found.";
           for(i=0; i<relationCount; ++i)
               if(subSets[i][0]!=subSets[i][1])//This part is for deciding which pair to print out. Printing out elements with non equal elements are the priority
               {
                   writeResult<<" For example:"<<
                   "("<<subSets[i][0]<<","<<subSets[i][1]<<") is found but ("<<subSets[i][1]<<","<<subSets[i][0]<<") is not found.";
                   //Printing out the chosen sets as an example
                   break;
               }
       }
       else
           writeResult<<"Antisymmetric: No. Because:("<<subSets[i][0]<<","<<subSets[i][1]<<") is found but also ("<<subSets[i][1]<<","<<subSets[i][0]<<") is found.";
           //Printing out one of the relation sets with a symmetric relation (excluding the ones with equal elements)

       //Transitive Test
       tTest="Passed";
       //Resetting the foundTs as -1. They will stay as -1 if there is no starting condition
       foundTi=-1;
       foundTj=-1;
       foundTk=-1;

       for(i=0; i<relationCount-1; ++i)
       {
           for(j=i+1; j<relationCount; ++j)
           {
               /*Checking if there is a starting condition.
               A starting condition happens if there are sets like (a,b) and (b,c) in the relation*/
               if(subSets[i][1]==subSets[j][0])
                   for(int k=0; k<relationCount; ++k)
                   {
                      if(subSets[k][0]==subSets[i][0] && subSets[k][1]==subSets[j][1])//Checking if there is a (a,c) for the (a,b) and (b,a) (this was an example)
                      {
                          //Storing the relation sets' numbers to print them out later
                          foundTi=i;
                          foundTj=j;
                          foundTk=k;
                          break;
                      }
                      else if(k==relationCount-1)//If there is no (a,c) for the (a,b) and (b,c), it means the relation is not transitive (this was an example)
                         tTest="Failed";
                   }
               else if(subSets[i][0]==subSets[i][1] && foundTi==-1)
               {
                   //If there is a relation set which has equal elements, it is again a starting condition
                   foundTi=i;
                   foundTj=i;
                   foundTk=i;
               }
               if(tTest=="Failed")//Stops checking if even for one start condition, the transitivity requirements is not met
                   break;
           }
           if(tTest=="Failed")
               break;
       }

       if(tTest=="Passed")
       {
           writeResult<<endl<<"Transitive: Yes. Because all the necessary elements are found.";
           if(foundTi!=-1)//The relation is still a transitive relation even though it does not have a starting condition. If that is the case, there wont be any found sets so no sets will be printed out
              writeResult<<"For example:("<<subSets[foundTk][0]<<","<<subSets[foundTk][1]<<") is found since ("<<subSets[foundTi][0]<<","<<subSets[foundTi][1]<<") and ("<<subSets[foundTj][0]<<","<<subSets[foundTj][1]<<") are found.";
              //Printing out the stored relation sets
           writeResult<<endl;
       }
       else
       {
           writeResult<<endl<<"Transitive: No. Because:"<<
           "("<<subSets[i][0]<<","<<subSets[i][1]<<") and ("<<subSets[j][0]<<","<<subSets[j][1]<<") is found but ("<<subSets[i][0]<<","<<subSets[j][1]<<") is not found."<<endl;
           //Printing out one of the reasons why the relation is not transitive
       }

    }

    writeResult.close();
    readSet.close();

    return 0;
}
