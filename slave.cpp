#include "I2C.cpp"

class slave: public I2C{
    public:
        //registers
        byte idx = 0;
        byte drx = 0;
        byte state = 1;

        vector<byte> addr, received_addr, data;
        byte received_data = 0;
        byte rec = 1;
        byte rw;

        slave(vector<byte> &a){
            addr.resize(ADDRSIZE);
            received_addr.resize(ADDRSIZE);
            data.resize(DATASIZE);
            for(int i = 0; i<ADDRSIZE; i++){
                addr[i] = a[i];
            }
        }

        byte checkaddr(){
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
            byte bit = SDA[WINDOW-1];
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
                    received_data = (received_data<<1) | static_cast<int>(bit);
                    int temp = (int)idx;
                    ++idx;
                    if(idx==DATASIZE){
                        idx = 0;
                        state<<=1;
                        cout<<"data: "<<(int)received_data<<endl;
                        if(rw){
                            drx = mem[received_data];
                            cout<<"read data: "<<(int)drx<<endl;
                        }
                        else{
                            mem[memptr] = received_data;
                            ++memptr;
                            cout<<"wrote data: "<<(int)received_data<<" into mem location: "<<memptr-1<<endl;
                        }
                        
                    }
                }
                else if(state&(1<<3)){
                    cout<<"state 4"<<endl;

                }
                

            }
        }
};