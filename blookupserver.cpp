/***************************************************************************
 * sslookupserver.cpp  -  Program to serve Shakespeare search results
 *
 * copyright : (C) 2009 by Jim Skon
 ***************************************************************************/
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include "Bible.h"
#include "Ref.h"
#include "Verse.h"
#include "fifo.h"

using namespace std;

const string RECEIVE_PIPE = "request";
const string SEND_PIPE    = "reply";

Ref parseRef(string str) {
    // remove newline / whitespace
    str.erase(str.find_last_not_of(" \n\r\t") + 1);

    // replace | with space so parsing works with "1|1|1"
    for (char &c : str) if (c == '|') c = ' ';

    int book, chapter, verse;
    stringstream ss(str);

    ss >> book >> chapter >> verse;

    if (ss.fail()) {
        cerr << "[DEBUG] Invalid reference format: '" << str << "'" << endl;
        return Ref();
    }

    return Ref(book, chapter, verse);
}


int main() {
    // Disable buffering for immediate terminal output
    cout.setf(ios::unitbuf);

    // Create Bible object and build index
       Bible bible("/home/class/csc3004/Bibles/web-complete");
    cout << "[DEBUG] Bible object created and index built." << endl;

    // Create FIFOs for communication
    Fifo recfifo(RECEIVE_PIPE);
    Fifo sendfifo(SEND_PIPE);

    cout << "[DEBUG] FIFOs created. Waiting for requests..." << endl;


    recfifo.openread();
    while (true) {
        // Wait for a request
        
        string request = recfifo.recv();
        cout << "[DEBUG] Received request: " << request << endl;

        // Parse the request into a Ref
        Ref r = parseRef(request);

        LookupResult status;
        Verse v = bible.lookup(r, status);

        // Send back the verse text
        sendfifo.openwrite();
        if (status == SUCCESS) {
            cout << "[DEBUG] Sending verse: " << v.getVerse() << endl;
        sendfifo.send(v.getRef().getBook() + "|" + 
                         std::to_string(v.getRef().getChapter()) + "|" +
                         std::to_string(v.getRef().getVerse()) + "|" +
                         v.getVerse());
        } else {
            cout << "[DEBUG] Reference not found" << endl;
            sendfifo.send("Error: Reference not found");
        }

        // End-of-message marker
        sendfifo.send("$end");
        sendfifo.fifoclose();
    }

    return 0;
}