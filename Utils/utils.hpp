
#ifndef ENOBIORECORD_UTILS_H
#define ENOBIORECORD_UTILS_H

#include "constants.h"
#include <windows.h>
#include <string>
#include <vector>
using namespace std;


//utility funtion for generating filenames
string generate_filename(string type){

    string name = "..\\Logs\\";
    name.append(type);
    name.append("Logs\\");
    name.append(type);
    name.append("Log_");
    char sdate[9];
    char stime[9];
    _strdate_s(sdate, 9);
    _strtime_s(stime, 9);

    name.append(stime);
    name.append("_");
    name.append(sdate);
    name.append(".csv");

    for(int i = 0; i<name.length(); ++i){
        if (name[i] == '/' || name[i] == ':')
            name[i] = '-';
    }

    return name;
}

int32_t get_value_from_sample(unsigned char* a){
    int32_t b = 0;
    b = ((a[0] << 24) | (a[1] << 16) | (a[2] << 8) | (a[3]));
    return b;
}

uint64_t get_timestamp_from_sample(unsigned char* a){
    uint64_t b = 0;
    b = ( (a[0] << 56) | (a[1] << 48) | (a[2] << 40) | (a[3] << 32) | (a[4] << 24) | (a[5] << 16) << (a[6] << 8) | (a[7]) );
    return b;
}


void print_buffer_with_timestamp(unsigned char* buff, ofstream& out){
    for (int i = 0 ; i < NS; i++){
        // read channel data
        for (int j = 0 ; j < CHANNEL_NO; j++){
            out << get_value_from_sample(buff) << ",";
            // move buffer pointer up after reading one sample
            buff = buff + BPS;
        }
        out << get_timestamp_from_sample(buff) << endl;
        buff = buff + (2 * BPS); // 2*BPS = timestamp size
    }
}

void print_raw(unsigned char* buff, ofstream& out){
    out << buff;
}

void print_header(ofstream& out){
    for (int i = 0; i < CHANNEL_NO; i++){
        out << channels[i] << ",";
    }
    out <<"Timestamp" << endl;
}

template <typename T > void display_vector (vector<T> &v) {
    for (auto it = v.begin(); it != v.end(); ++it) {
        cout << (*it) << " ";
    }
    cout << endl;
}

#endif //ENOBIORECORD_UTILS_H
