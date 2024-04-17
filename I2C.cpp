#include <iostream>
#include <bits/stdc++.h>

using namespace std;

#define byte uint8_t

#define pb push_back
#define mp make_pair
#define pbyte pair<byte, byte>
#define vbyte vector<byte>
#define vvbyte vector<vector<byte>>

const static byte DATASIZE = 7;
const static byte ADDRSIZE = 7;
const static byte WINDOW = 10;
const static byte MEMSIZE = 255;
static vector<byte> mem(MEMSIZE);
static byte memptr = 0;

class I2C{
    public:
        // static byte state;
        static byte start;
        static byte stop;

        static vector<byte> SDA, SCL;

        void sda_send(byte bit){
            SDA.erase(SDA.begin());
            SDA.pb(bit);
        }

        void tick(){
            SCL.erase(SCL.begin());
            if(start) SCL.pb(!*--SCL.end());
            else SCL.pb(1);
        }
        
        void start_state(){
            start = 1;
            stop = 0;
            // state = 1;
        }

        void stop_state(){
            start = 0;
            stop = 1;
        }

        void check_startcond(){
            if(SCL[8] && SCL[9] && SDA[8] && !SDA[9]){
                start_state();
            }
        }

        void check_stopcond(){
            if(SCL[8] && SCL[9] && !SDA[8] && SDA[9]){
                stop_state();
            }
        }        
};

// byte I2C::state = 1;
byte I2C::start = 0;
byte I2C::stop = 1;
vector<byte> I2C::SDA(WINDOW, 1);
vector<byte> I2C::SCL(WINDOW, 1);