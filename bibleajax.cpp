/* Demo server program for Bible lookup using AJAX/CGI interface
 * By James Skon, Febrary 10, 2011
 * updated by Bob Kasper, January 2020
 * updated by Deborah Wilson, February 2026
 * Mount Vernon Nazarene University
 *
 * This sample program works using the cgicc AJAX library to
 * allow live communication between a web page and a program running on the
 * same server that hosts the web server.
 *
 * This program is run by a request from the associated html web document.
 * A Javascript client function invokes an AJAX request,
 * passing the input form data as the standard input string.
 *
 * The cgi.getElement function parses the input string, searching for the matching
 * field name, and returing a "form_iterator" oject, which contains the actual
 * string the user entered into the corresponding field. The actual values can be
 * accessed by dereferencing the form iterator twice, e.g. **verse
 * refers to the actual string entered in the form's "verse" field.
 *
 * STUDENT NAME: Nicholas Betounes
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
// Included references to previous pages in with the other included programs to keep things organized
#include "Bible.h"
#include "Ref.h"
#include "Verse.h"

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"

using namespace std;
using namespace cgicc;

int main()
{
   /* A CGI program must send a response header with content type
    * back to the web client before any other output.
    * For an AJAX request, our response is not a complete HTML document,
    * so the response type is just plain text to insert into the web page.
    */
   cout << "Content-Type: text/plain\n\n";

   Cgicc cgi;  // create object used to access CGI request data

   // GET THE INPUT DATA
   // browser sends us a string of field name/value pairs from HTML form
   // retrieve the value for each appropriate field name
   form_iterator st = cgi.getElement("search_type");
   form_iterator book = cgi.getElement("book");
   form_iterator chapter = cgi.getElement("chapter");
   form_iterator verse = cgi.getElement("verse");
   form_iterator nv = cgi.getElement("num_verse");

   // Convert and check input data
   bool validInput = false;
   if (chapter != cgi.getElements().end())
   {
      int chapterNum = chapter->getIntegerValue();
      if (chapterNum > 150)
      {
         cout << "<p>The chapter number (" << chapterNum << ") is too high.</p>" << endl;
      }
      else if (chapterNum <= 0)
      {
         cout << "<p>The chapter must be a positive number.</p>" << endl;
      }
      else
      {
         validInput = true;
      }
   }

   // TODO: OTHER INPUT VALUE CHECKS ARE NEEDED ... but that's up to you!

   bool bookOk = false;
   bool verseOk = false;
   bool nvOk = false;

   /* BOOK CHECK */
   if (book != cgi.getElements().end())
   {
       int bookNum = book->getIntegerValue();
       if (bookNum >= 1 && bookNum <= 66)
           bookOk = true;
       else
           cout << "<p>The book number must be between 1 and 66.</p>" << endl;
   }
   else
   {
       cout << "<p>Missing book.</p>" << endl;
   }

   /* VERSE CHECK */
   if (verse != cgi.getElements().end())
   {
       int verseNum = verse->getIntegerValue();
       if (verseNum > 0)
           verseOk = true;
       else
           cout << "<p>The verse must be a positive number.</p>" << endl;
   }
   else
   {
       cout << "<p>Missing verse.</p>" << endl;
   }

   /* NUM VERSE CHECK */
   if (nv != cgi.getElements().end())
   {
       int nvNum = nv->getIntegerValue();
       if (nvNum > 0)
           nvOk = true;
       else
           cout << "<p>The number of verses must be positive.</p>" << endl;
   }
   else
   {
       cout << "<p>Missing number of verses.</p>" << endl;
   }

   /* TODO: PUT CODE HERE TO CALL YOUR BIBLE CLASS FUNCTIONS
    *        TO LOOK UP THE REQUESTED VERSES
    */
   // Get's bible object from server
   Bible webBible("/home/class/csc3004/Bibles/web-complete");

   int bookNum = book->getIntegerValue();
   int chapNum = chapter->getIntegerValue();
   int verseNum = verse->getIntegerValue();
   int howMany = nv->getIntegerValue();

   Ref ref(bookNum, chapterNum, verseNum);

   LookupResult result;

   // First verse
   Verse v = webBible.lookup(ref, result);

   if (result == SUCCESS)
   {
       cout << "<p>";
       v.display();
       cout << "</p>";

       // If user asked for more than one verse
       for (int i = 1; i < howMany; i++)
       {
           v = webBible.nextVerse(result);
           if (result == SUCCESS)
           {
               cout << "<p>";
               v.display();
               cout << "</p>";
           }
           else break;
       }
   }
   else


   /* SEND BACK THE RESULTS
    * Finally we send the result back to the client on the standard output stream
    * in HTML text format.
    * This string will be inserted as is inside a container on the web page,
    * so we must include HTML formatting commands to make things look presentable!
    */
   if (validInput)
   {
      cout << "Search Type: <b>" << **st << "</b>" << endl;
      cout << "<p>Your result: "
           << **book << " " << **chapter << ":" << **verse
           << "<em> The " << **nv
           << " actual verse(s) retreived from the server should go here!</em></p>" << endl;
   }
   else
   {
      cout << "<p>Invalid Input: <em>report the more specific problem.</em></p>" << endl;
   }

   return 0;
}
