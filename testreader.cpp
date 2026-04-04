
/* testreader.cpp
 * Computer Science, MVNU
 * CSC-3004 Introduction to Software Development
 *
 * main function for Project 1
 *
 * NOTE: You may add code to this file, but do not
 * delete any code or delete any comments.
 *
 * STUDENT NAME: Nicholas Betounes
 */

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main (int argc, char **argv)
{

   // Create Bible object to process the raw text file
   Bible webBible("/home/class/csc3004/Bibles/web-complete");
	
   cout << "\n--- Index Diagnostics ---\n";
   webBible.debugIndex();   // call your debug function
   cout << "-------------------------\n\n";
   Verse verse;
   int bookNum, chapterNum, verseNum;
   LookupResult result;
	
   cout << "Using Bible from: ";
   webBible.display();
	
   // Prompt for input: get reference to lookup
   // This is good for testing your classes quickly for the first milestone.
  // cout << "Enter 3 integers for book, chapter and verse to find: " << flush;
  // cin >> bookNum >> chapterNum >> verseNum;
   // TODO: your final program should get input from command line arguments instead.
   if (argc != 4 && argc != 5) {
       cout << "Usage: " << argv[0] << " <book> <chapter> <verse> [numVerses]" << endl;
       return 1;
   }
    
   bookNum = atoi(argv[1]);
   chapterNum = atoi(argv[2]);
   verseNum = atoi(argv[3]);
   // Create a reference from the numbers
   Ref ref(bookNum, chapterNum, verseNum);

   // Index size is referenced from the Bible class
   cout << "Index size: " << webBible.getIndexSize() << endl;
   auto last = webBible.getLastIndexEntry();
   cout << "Last verse byte offset: " << last.second << endl;
   cout << "Offset of entered ref: " << webBible.getOffset(ref) << endl;

  

   // Use the Bible object to retrieve the verse by reference
   cout << "Looking up reference: ";
   ref.display();
   cout << endl;

   verse = webBible.lookup(ref, result);
   cout << "Result status: " << result << endl;
   verse.display();
   cout << endl;

int howMany = (argc == 5) ? atoi(argv[4]) : 1;   // 5th argument = number of verses

   Ref current = ref;
   for (int i = 1; i < howMany; i++)
   {
       LookupResult nextStatus;
       current = webBible.next(current, nextStatus);

       if (nextStatus != SUCCESS) break;

       LookupResult vResult;
       Verse nextV = webBible.lookup(current, vResult);
       if (vResult == SUCCESS) {
           nextV.display();
           cout << endl;
       }
   }
}
