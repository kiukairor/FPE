#include "ff3.h"

//careful NIST spec data is a string and is then converted, Do the same after
//int FF3::init(byte *data, unsigned int data_len){
FF3::ff3_helper FF3::init(byte *data, unsigned int data_len){
    //Feisel feistel = new Feistel (block_len, unsigned int left_len, unsigned int right_len);
    //Feistel feistel = Feistel (8);
    
    int m=0;
    ff3_helper ff3_help;
    byte W[4]={0};

    int mod=data_len%2;

    int u = data_len/2 + mod;
    int v = data_len - u;


    ff3_help.A = new byte[u];
    memcpy(ff3_help.A,data,u);

    ff3_help.B = new byte[v];
    memcpy(ff3_help.B,data+u,v);

    cout << "BK0" << endl;
    //byte *Tl = new byte[32];
    //memset(Tl, 0x00, 32);    
    print_array(tweak,7);
    byte Tl[4],Tr[4] ={0};
    
    //Tl
    //Tweak[0...27]=3*8+4
    //tweak[0],Tweak[1],Tweak[2],Tweak[3] & '11110000'; '11110000'=240
    memcpy(Tl, tweak,3);
    Tl[3]=tweak[3] & 240;
    print_array(Tl,4);    

    //Tr
    //tweak[4],Tweak[5],Tweak[6],Tweak[3] & '00001111'; '00001111'=15
    //byte Tr[4] ={0};
    memcpy(Tr,tweak+4,3);
    Tr[3]=tweak[3] & 15;
    print_array(Tr,4);    
    cout << "INIT" << endl;
    cout << "init A,B = " << endl;
    print_array(ff3_help.A,u);
    print_array(ff3_help.B,v);
    
    ff3_help.u=u;
    ff3_help.v=v;
    memcpy(ff3_help.Tl,Tl,4);
    memcpy(ff3_help.Tr,Tr,4);
    print_array(ff3_help.Tr,4);    
//return 0;
return ff3_help;
}

//i is the i-th round
//int FF3::execRound(int i, int u, int v, byte W[32], byte Tr[32], byte Tl[32]){
int FF3::execRound(int i, FF3::ff3_helper helper){

    byte W[4]={0};
    int m = 0;
    byte *i_array=new byte[4];
    byte *tmp;
    if (i%2 == 0 ){
        m=helper.u;
        memcpy(W,helper.Tr,4);
    }

    else if ( i%2 == 1){
        m=helper.v;
        memcpy(W,helper.Tl,4);
    }

    else{
        perror("you should never see this and get rid of this case\n");
        return -1;
    }


    
    //step4.ii; check doc NIST here
    //xorByteArrays()
    cout << "OULOULOU" << endl;
    int32toByteArray(i, i_array);
    cout << "OULOULOU" << endl;
    xorByteArrays(W,4, i_array, 4, tmp);
    
    
    cout << "printing W,i,tmp=W^i" << endl;
    //print_array(W,4);
    //print_array(i_array,4);
    print_array(tmp,4);
    
    //NUM_radix(REV(B))
    //cout << "printing v = " << dec << helper.v << endl;
    //cout << "printing u = " << dec << helper.u << endl;
    print_array(helper.B,helper.v);
    
    //P right half
    byte* rev = revByteArray(helper.B, helper.v);
    cout << "chelou numP array = " << endl;
    print_array(rev,helper.v);
    //num_radix(tmp)^12
    int numP = num(rev,helper.v,this->base);
    cout << "chelou numP = " << dec << numP << endl;
    
    //en fait je fais des trucs qui s'annulent j'ai limpression mais ca doit dependre de la base ds laque on est en fait
    //dc on ne reflechit pas on implemente step by step
    
    cout << "End of execRound " << i << endl;
    return 0;
}

int FF3::exec(byte *data, unsigned int data_len){

    Feistel feistel = Feistel(8);
    int m=0;
    
    //byte W[4]={0};
    //byte i_array[4]={0};
    
    

    FF3::ff3_helper helper = FF3::init(data,data_len);
    
    //for (int i=0; i<feistel.rounds; i++){
        for (int i=0; i<2; i++){
        cout << "i = " << i << endl;        
        FF3::execRound(i,helper);
    }
    return 0;
}
