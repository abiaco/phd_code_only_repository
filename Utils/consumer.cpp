#ifndef CONSUMER_CPP
#define CONSUMER_CPP

#include <fstream>
#include <iostream>

#include "utils.hpp"
#include "blocking_queue.hpp"

using namespace std;
void consume(b_queue<unsigned char* > *q, bool *conn){
    cout << " started consumer\n";

    string filename = generate_filename("EEG");

    string raw_filename = generate_filename("Raw");

    cout << " created filename consumer\n";
    ofstream out (filename);
    ofstream rawout (raw_filename);

    cout << " opened file consumer\n";
    print_header(out);
    cout << " printed header consumer\n";

    while (*conn){
        cout << "queue size: " << q->size() << endl;
        if (!(*conn)) {
            cout <<"connection closed; still to print: " << q->size() << endl;
        }
        unsigned char* p = (*q).pop();
        unsigned char *cpyptr = p;
        print_raw(cpyptr, rawout);
        print_buffer_with_timestamp(cpyptr, out);


        cout << "done printing; cleaning up\n";

    }
    while (q->size() > 0){
        unsigned char* p = (*q).pop();
        unsigned char *cpyptr = p;
        print_raw(cpyptr, rawout);
        print_buffer_with_timestamp(cpyptr, out);
       // delete cpyptr;
    }
    cout << "done printing; flushing\n";
    out.flush();
    rawout.flush();
    cout << " flushed file consumer\n";
    rawout.close();
    out.close();
    cout << "closed file consumer\n";
    cout << "consumer DONE\n";
}

#endif