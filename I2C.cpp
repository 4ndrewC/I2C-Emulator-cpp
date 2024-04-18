#include <iostream>
#include <bits/stdc++.h>

using namespace std;

typedef uint8_t u8;

#define pb push_back
#define mp make_pair

const static u8 DATASIZE = 8;
const static u8 ADDRSIZE = 7;
const static u8 RWLOC = ADDRSIZE;
const static u8 DATALOC = RWLOC+1;
const static u8 WINDOW = 10;
const static u8 MEMSIZE = 255;
static vector<u8> EEPROM(MEMSIZE);
static u8 memptr = 0;

class I2C{
    public:
        static u8 start;
        static u8 stop;

        static vector<u8> SDA, SCL;

        void sda_send(u8 bit){
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

u8 I2C::start = 0;
u8 I2C::stop = 1;
vector<u8> I2C::SDA(WINDOW, 1);
vector<u8> I2C::SCL(WINDOW, 1);
