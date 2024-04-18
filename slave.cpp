#include "I2C.cpp"

class slave: public I2C{
    public:
        //registers
        u8 idx = 0;
        u8 drx = 0;
        u8 rrx = 0;
        u8 state = 1;

        vector<u8> addr, received_addr, data;

        //bits
        u8 rec = 1;
        u8 rw;

        slave(vector<u8> &a){
            addr.resize(ADDRSIZE);
            received_addr.resize(ADDRSIZE);
            data.resize(DATASIZE);
            for(int i = 0; i<ADDRSIZE; i++){
                addr[i] = a[i];
            }
        }

        u8 checkaddr(){
            for(int i = 0; i<ADDRSIZE; i++){
                if(addr[i]!=received_addr[i]){
                    return 0;
                }
            }
            return 1;
        }

        void sendack(){
            sda_send(1);
            tick();
        }

        void receive(){
            u8 bit = SDA[WINDOW-1];
            if(!rec) return;
            if(start){
                if(state&(1<<0)){
                    cout<<"state 1"<<endl;
                    received_addr[idx] = bit;
                    int temp = (int)idx;
                    ++idx;
                    if(idx==ADDRSIZE){
                        idx = 0;
                        //move to next state
                        rec = checkaddr();
                        if(!rec){
                            cout<<"Address not matched"<<endl;
                            return;
                        }
                        state<<=1;
                    }
                }
                else if(state&(1<<1)){
                    cout<<"state 2"<<endl;
                    rw = bit;
                    idx = 0;
                    state<<=1;
                }
                else if(state&(1<<2)){
                    cout<<"state 3"<<endl;
                    data[idx] = bit;
                    drx = (drx<<1) | static_cast<int>(bit);
                    int temp = (int)idx;
                    ++idx;
                    if(idx==DATASIZE){
                        idx = 0;
                        state<<=1;
                        cout<<"data: "<<(int)drx<<endl;
                        if(rw){
                            rrx = EEPROM[drx];
                            cout<<"read data: "<<(int)drx<<endl;
                        }
                        else{
                            EEPROM[memptr] = drx;
                            ++memptr;
                            cout<<"wrote data: "<<(int)drx<<" into mem location: "<<memptr-1<<endl;
                        }
                    }
                }
                else if(state&(1<<3)){
                    cout<<"state 4"<<endl;

                }
                

            }
        }
};
