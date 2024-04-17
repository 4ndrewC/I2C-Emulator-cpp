#include "slave.cpp"

class master: public I2C{
    public:
        int rec = 1;
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

        void inputstream(vector<byte> &bitstream){
            pull_down();
        }

        void send_data(byte bit){
            tick();
            sda_send(bit);
        }
};