#include "master.cpp"

I2C line1;

void print_lines(){
    cout<<"SDA Line: ";
    for(u8 i : line1.SDA) cout<<(int)i<<" ";
    cout<<endl;
    cout<<"SCL Line: ";
    for(u8 i: line1.SCL) cout<<(int)i<<" ";
    cout<<endl;
}

void run(master &master1, master &master2, vector<u8> bitstream){
    cout<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
    //transmission starts here
    master1.inputstream(bitstream);
    line1.check_startcond();
    print_lines();   
    cout<<"Start: "<<(int)line1.start<<endl;
    for(int i = 0; i<ADDRSIZE; i++){
        master1.send_data(bitstream[i]);
        master2.receive();
        
        print_lines();
        
    }
    cout<<"Rec for slave 1 -------------------------"<<(int)master2.rec<<endl;

    //send read/write bit
    master1.send_data(bitstream[RWLOC]);
    master2.receive();
    // slave1.receive();
    print_lines();
    cout<<"rw bit: "<<(int)master2.rw<<endl;
    //ack bit
    cout<<"send ack bit: "<<endl;
    master2.sendack();
    print_lines();

    //data bits
    for(int i = DATALOC; i<DATALOC+DATASIZE; i++){
        master1.send_data(bitstream[i]);
        master2.receive();
        print_lines();
    }

    //release SDA line
    cout<<"release SDA"<<endl;
    master1.release();
    master2.receive();
    print_lines();
    line1.check_stopcond();
    cout<<"Stop: "<<(int)line1.stop<<endl;
    cout<<"Start: "<<(int)line1.start<<endl;
    

    // ---------------------------------------------------------------------
    //reset everything for next transmission
    master2.state = 1;
    master2.idx = 0;
    master2.rec = 1;
    for(int i = 0; i<ADDRSIZE; i++){
        master2.received_addr[i] = 0;
    }
    for(int i = 0; i<DATASIZE; i++){
        master2.data[i] = 0;
    }
}

int main(){
    //create I2C line
    line1 = I2C();

    """sample usage""";

    //create master
    vector<u8> master1addr = {1,1,0,1,1,1,0};
    master master1(master1addr);

    vector<u8> plcaddr = {0,1,1,1,0,0,0};
    master plc(plcaddr);

    
    //programmables
    string sample_message = "1EA4rhgo8792mGP";
    vector<u8> targetaddr = {1,1,0,1,1,1,0};
    string received_message = "";
    //dont need to write here
    u8 readwrite = 1;
    for(int i = 0; i<sample_message.length(); i++){
        vector<u8> curbitstream(16);
        for(int j = 0; j<ADDRSIZE; j++) curbitstream[j] = targetaddr[j];
        curbitstream[RWLOC] = readwrite;
        u8 byte = (u8)sample_message.substr(i,1)[0];
        //reverse because protocol is big endian
        vector<u8> rev(DATASIZE);
        for(int j = 0; j<DATASIZE; j++){
            rev[j] = byte&1;
            byte>>=1;
        }
        for(int j = 0; j<DATASIZE; j++) curbitstream[DATALOC+DATASIZE-j-1] = rev[j];
        run(plc, master1, curbitstream);
        received_message+= (char)master1.drx;
    }
    cout<<received_message<<endl;

    return 0;
}
