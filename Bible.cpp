/* Bible class function definitions
 * Computer Science, MVNU
 * CSC-3004 Introduction to Software Development
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

// Default constructor
Bible::Bible()
{ 
   infile = "/home/class/csc3004/Bibles/web-complete";
}

// Constructor – pass bible filename
Bible::Bible(const string s) {
    infile = s;
    isOpen = false;
    buildIndex();   // ? ADD THIS
}

// REQUIRED: lookup finds a given verse in this Bible
Verse Bible::lookup(const Ref ref, LookupResult& status)
{
   // OPTIONAL: Return the reference after the given parameter ref
  auto it = index.find(ref);

    if (it == index.end())
    {
        status = OTHER;
        return Verse();   // correct type
    }

    ifstream infileStream(infile.c_str());
    if (!infileStream)
    {
        status = OTHER;
        return Verse();
    }

    infileStream.seekg(it->second);  // go to byte offset

    string line;
    getline(infileStream, line);

    status = SUCCESS;
    return Verse(line);   // correct type
}

// REQUIRED: Return the next verse from the Bible file stream if the file is open.
// If the file is not open, open the file and return the first verse.
Verse Bible::nextVerse(LookupResult& status)
{
    if (!isOpen)
    {
        instream.open(infile);
        if (!instream)
        {
            status = OTHER;
            return Verse();
        }
        isOpen = true;
    }

    string line;
    if (getline(instream, line))
    {
        status = SUCCESS;
        return Verse(line);
    }
    else
    {
        status = OTHER;
        return Verse();
    }
}
// REQUIRED: Return an error message string to describe status
string Bible::error(LookupResult status)
{
   return "Invalid Entry Please try again";
}

void Bible::display()
{
   cout << "Bible file: " << infile << endl;
}
	
// OPTIONAL access functions

Ref Bible::next(const Ref ref, LookupResult& status)
{
    auto it = index.upper_bound(ref); // next after ref

    if (it == index.end())
    {
        status = OTHER;
        return Ref();  // empty Ref if no next
    }

    status = SUCCESS;
    return it->first;
}

// OPTIONAL: Return the reference after the given ref


// OPTIONAL: Return the reference before the given ref
Ref Bible::prev(const Ref ref, LookupResult& status)
{
   return ref;
}


int Bible::getIndexSize() const { 
    return index.size(); 
}

pair<Ref,int> Bible::getLastIndexEntry() const { 
    return *index.rbegin(); 
}

int Bible::getOffset(const Ref& r) const { 
    auto it = index.find(r);
    if (it != index.end()) return it->second;
    return -1; // or some invalid value
}

void Bible::buildIndex()
{
    ifstream infileStream(infile.c_str());
    if (!infileStream) {
        cerr << "Error opening file for indexing\n";
        return;
    }

    string line;
    int position;

    while (!infileStream.fail())
    {
        position = infileStream.tellg();   // byte offset
        getline(infileStream, line);

        if (line.length() > 0)
        {
            Verse v(line);          // create verse from line
            Ref r = v.getRef();     // extract reference

            index[r] = position;    // store in map
        }
    }

    infileStream.close(); // <- THIS IS THE LAST LINE INSIDE buildIndex()
}

// <- debugIndex() MUST BE OUTSIDE buildIndex()
void Bible::debugIndex() const {
    std::cout << "Index contains " << index.size() << " references\n";
    if (!index.empty()) {
        auto last = *index.rbegin();
        std::cout << "Last verse byte offset: " << last.second << "\n";
    }

    // Print first few references for verification
    int count = 0;
    for (auto it = index.begin(); it != index.end() && count < 3; ++it, ++count) {
        std::cout << "Ref: ";
        it->first.display();
        std::cout << " -> Byte offset: " << it->second << "\n";
    }
}
