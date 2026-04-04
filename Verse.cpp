/* Verse class function definitions
 * Computer Science, MVNU
 * CSC-3004 Introduction to Software Development
 *
 * NOTE: You may add code to this file, but do not
 * delete any code or delete any comments.
 *
 * STUDENT NAME: Nicholas Betounes
 */

#include "Verse.h"
#include <iostream>

using namespace std;

// Default constructor
Verse::Verse()
{  	
   verseText = "Uninitialized Verse!";
   verseRef = Ref();
} 

// REQUIRED: Parse constructor - pass verse string from file
Verse::Verse(const string s)
{
   // TODO: use Ref constructor to create verseRef
	string line = s;
	size_t pos = line.find(' ');

	// this line of code referenses the vers
	string refPart = line.substr(0, pos);
	verseRef = Ref(refPart);
   // store the remainder of the string in verseText
	verseText = line.substr(pos + 1);
}  	

// REQUIRED: Accessors
string Verse::getVerse()
{
	return verseText;
}

Ref Verse::getRef()
{
   return verseRef;
}

// display reference and verse
void Verse::display()
{
   verseRef.display();
   cout << " " << verseText;
}
