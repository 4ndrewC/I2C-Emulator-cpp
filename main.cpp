#include "master.cpp"

I2C line1;

void print_lines(){
    cout<<"SDA Line: ";
    for(byte i : line1.SDA) cout<<(int)i<<" ";
    cout<<endl;
    cout<<"SCL Line: ";
    for(byte i: line1.SCL) cout<<(int)i<<" ";
    cout<<endl;
}

void run(master &master1, slave &slave1, slave &slave2, vector<byte> &bitstream){
    //transmission starts here
    master1.inputstream(bitstream);
    line1.check_startcond();

    print_lines();    
    cout<<"Start: "<<(int)line1.start<<endl;
    for(int i = 0; i<ADDRSIZE; i++){
        master1.send_data(bitstream[i]);
        slave1.receive();
        slave2.receive();
        print_lines();
        
    }
    // cout<<"Slave received address: ";
    // for(byte i : slave1.received_addr){
    //     cout<<(int)i<<" ";
    // }
    // cout<<endl;
    for(byte bit: slave1.received_addr) cout<<(int)bit<<" ";
    cout<<endl;
    for(byte bit: slave2.received_addr) cout<<(int)bit<<" ";
    cout<<endl;
    cout<<"Rec for slave 1 -------------------------"<<(int)slave1.rec<<endl;
    cout<<"Rec for slave 2 -------------------------"<<(int)slave2.rec<<endl;

    //send read/write bit
    master1.send_data(bitstream[7]);
    slave1.receive();
    slave2.receive();
    // slave1.receive();
    print_lines();
    cout<<"rw bit: "<<(int)slave1.rw<<endl;
    cout<<"rw bit: "<<(int)slave2.rw<<endl;
    //ack bit
    cout<<"send ack bit: "<<endl;
    slave1.sendack();
    slave2.sendack();
    print_lines();

    //data bits
    for(int i = ADDRSIZE+1; i<ADDRSIZE+1+DATASIZE; i++){
        master1.send_data(bitstream[i]);
        slave1.receive();
        slave2.receive();
        print_lines();
    }
    // cout<<"Slave received data: ";
    // for(byte i : slave1.data){
    //     cout<<(int)i<<" ";
    // }
    // cout<<endl;

    //release SDA line
    cout<<"release SDA"<<endl;
    master1.release();
    slave1.receive();
    slave2.receive();
    print_lines();
    line1.check_stopcond();
    cout<<"Stop: "<<(int)line1.stop<<endl;
    cout<<"Start: "<<(int)line1.start<<endl;


    // ---------------------------------------------------------------------
    //reset everything for next transmission
    slave1.state = 1; slave2.state = 1;
    slave1.idx = 0; slave2.idx = 0;
    slave1.rec = 1; slave2.rec = 1;
    for(int i = 0; i<ADDRSIZE; i++){
        slave1.received_addr[i] = 0; slave2.received_addr[i] = 0;
    }
    for(int i = 0; i<DATASIZE; i++){
        slave1.data[i] = 0; slave2.data[i] = 0;
    }
}

int main(){
    //create I2C line
    line1 = I2C();

    //create master
    master master1 = master();

    //create slave
    vector<byte> slave1addr = {0,1,1,1,0,0,0};
    vector<byte> slave2addr = {0,1,1,0,0,1,1};
    slave slave1 = slave(slave1addr);
    slave slave2 = slave(slave2addr);

    vector<byte> bitstream = {0,1,1,1,0,0,0,0,1,1,1,1,1,1,1};
    vector<byte> bitstream2 = {0,1,1,0,0,1,1,1,0,0,0,0,0,0,0};

    run(master1, slave1, slave2, bitstream);
    run(master1, slave1, slave2, bitstream2);

    //see results
    cout<<(int)slave1.received_data<<endl;
    cout<<(int)slave2.drx<<endl;

    return 0;
}