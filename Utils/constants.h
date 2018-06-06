
#ifndef ENOBIORECORD_CONSTANTS_H
#define ENOBIORECORD_CONSTANTS_H

#include <chrono>

using namespace std;

#define STATE_WAIT 0
#define STATE_CONNECTED 1
#define STATE_RECORD 2

#define GUI_PORT "8890"
#define GUI_PORT_KEYLOGGER "8891"
#define ENOBIO_PORT "1234"
#define SR 500
#define MESSAGE_LENGTH 1440
#define CHANNEL_NO 20
#define BPS 4
#define NS 20
#define SAMPLE_SIZE 4 * CHANNEL_NO
#define BUFFER_LENGTH BPS*NS*CHANNEL_NO + 8*NS //channel data + timestamp

typedef chrono::time_point<chrono::system_clock> time_point;

const char *channels[] = {"P7", "P4", "Cz", "Pz", "P3", "P8", "O1", "O2", "T8", "F8", "C4", "F4", "Fp2", "Fz", "C3", "F3", "Fp1", "T7", "F7", "EXT"};

const char* hostname = "155.245.23.84";

#endif //ENOBIORECORD_CONSTANTS_H
