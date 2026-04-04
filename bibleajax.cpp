#include <iostream>
#include <stdio.h>
#include <string.h>
#include "Bible.h"
#include "Ref.h"
#include "Verse.h"
#include "fifo.h"

using namespace std;

int main(int argc, char *argv[])
{
    // CGI header for web response
    cout << "Content-Type: text/plain\n\n";

    // For simplicity, assume input comes as command line args
    // argv[1] = book, argv[2] = chapter, argv[3] = verse, argv[4] = num_verse (optional)
    if (argc < 4) {
        cout << "<p>Missing input. Usage: book chapter verse [num_verse]</p>" << endl;
        return 1;
    }

    int bookNum = atoi(argv[1]);
    int chapterNum = atoi(argv[2]);
    int verseNum = atoi(argv[3]);
    int howMany = (argc >= 5) ? atoi(argv[4]) : 1;

    int maxChapters[67] = {
        0,50,40,27,36,34,24,21,4,31,24,
        22,25,29,36,10,13,10,42,150,31,
        12,8,66,52,5,48,12,14,3,9,
        1,4,7,3,3,3,2,14,4,28,
        16,24,21,28,16,16,13,6,6,4,
        4,5,3,6,4,3,1,13,5,5,
        3,5,1,1,1,22
    };

    // Validate input
    bool valid = true;
    if (bookNum < 1 || bookNum > 66) {
        cout << "<p>Invalid book number.</p>";
        valid = false;
    }
    if (chapterNum < 1 || chapterNum > maxChapters[bookNum]) {
        cout << "<p>Invalid chapter number.</p>";
        valid = false;
    }
    if (verseNum < 1) {
        cout << "<p>Verse must be positive.</p>";
        valid = false;
    }
    if (howMany < 1) {
        cout << "<p>Number of verses must be positive.</p>";
        valid = false;
    }

    if (!valid) return 1;

    // Create FIFOs
    Fifo requestFifo("request");
    Fifo replyFifo("reply");

    Ref ref(bookNum, chapterNum, verseNum);
    Bible webBible("/home/class/csc3004/Bibles/web-complete");

    Ref current = ref;
    for (int i = 0; i < howMany; i++) {
        string req = to_string(current.getBook()) + "|" +
                     to_string(current.getChapter()) + "|" +
                     to_string(current.getVerse());

        requestFifo.openwrite();
        requestFifo.send(req);
        requestFifo.fifoclose();

        replyFifo.openread();
        string reply = replyFifo.recv();
        replyFifo.fifoclose();

        cout << "<p>" << reply << "</p>";

        if (i < howMany - 1) {
            LookupResult nextStatus;
            current = webBible.next(current, nextStatus);
            if (nextStatus != SUCCESS) {
                cout << "<p><em>No more verses available.</em></p>";
                break;
            }
        }
    }

    return 0;
}