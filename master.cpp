#include "slave.cpp"

class master: public slave{
    public:
        int rec = 1;

        master(vector<u8>& a): slave(a){

        }

        void pull_down(){
            sda_send(0);
        }

        void release(){
            SCL.erase(SCL.begin());
            SCL.pb(1);
            sda_send(0);

            SCL.erase(SCL.begin());
            SCL.pb(1);
            sda_send(1);
        }

        void inputstream(vector<u8> &bitstream){
            pull_down();
        }

        void send_data(u8 bit){
            tick();
            sda_send(bit);
        }
};
