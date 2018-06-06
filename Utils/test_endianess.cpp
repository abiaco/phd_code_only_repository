#include<iostream>

using namespace std;


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

int main(){
	unsigned char s[4]= {0xF7, 0x8F, 0x99, 0x61};
	int32_t sample = get_value_from_sample(s);
	cout << sample;
}