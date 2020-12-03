
#include "ff3.h"
using namespace::chrono;
//careful NIST spec data is a string and is then converted, Do the same after
//int FF3::init(byte *data, unsigned int data_len){
//DV17 patch not implemented in tweak (0s)
FF3::ff3_helper FF3::init(byte *data, unsigned int data_len){
    
    int m=0;
    ff3_helper ff3_help;
    byte W[4]={0};

    int mod=data_len%2;

    int u = data_len/2 + mod;
    int v = data_len - u;

    print_array(data,data_len);
    ff3_help.A = new byte[u];
    memcpy(ff3_help.A,data,u);

    ff3_help.B = new byte[v];
    memcpy(ff3_help.B,data+u,v);

    byte Tl[4],Tr[4] ={0};
    
    memcpy(Tl, tweak,4);
    
    memcpy(Tr,tweak+4,4);
    
    ff3_help.u=u;
    ff3_help.v=v;
    memcpy(ff3_help.Tl,Tl,4);
    memcpy(ff3_help.Tr,Tr,4);
    this->aes=AES(128);
return ff3_help;
}

//i is the i-th round
//int FF3::execRound(int i, int u, int v, byte W[32], byte Tr[32], byte Tl[32]){
int FF3::encrypt_round(int i, FF3::ff3_helper helper){
    int fake=0;
    byte W[4]={0};
    int m = 0;
    unsigned int outLen=0;
    byte *i_array=new byte[4];
    byte *tmp;
    if ( i%2 == 0 ){
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

    int32toByteArray(i, i_array);
    xorByteArrays(W,4, i_array, 4, tmp);
    
    cout<< "revB" << endl;
    print_array(helper.B,helper.v);
    
    byte* revB = revByteArray(helper.B, helper.v);
    print_array(revB,helper.v);
    unsigned long long numP = numL(revB,helper.v,this->base);
    byte *a=new byte[12];
    bigInt2ByteArray(numP,a,12);

    byte *P = new byte[16];
    memcpy(P,tmp,4);
    memcpy(P+4,a,12);
    cout << "   P is :";
    print_array(P,16);
    cout << endl;
    byte *revP = revByteArray(P,16);
    cout << "revP is :";
    print_array(revP,16);
    cout << endl;

    //AES aes(128);
    byte *revKey = revByteArray(key,key_len);
    
    byte *S = aes.EncryptECB(revP, 16, revKey, outLen);
        
    byte *revS = revByteArray(S,outLen);
    
    cout  << "revS = " ;
    print_array_hex(revS,outLen);
    cout << endl;
    
    unsigned long long mod=pow((double)base,(double)m);
    
    //unsigned long long s=numL(revS, 16, 10, mod);
    __uint128_t hope=hope_numL(revS, outLen, 256);
    //printf("s=%llu\n", s*1000000000000000000000);
    //__int128 hope = s*1000000000000000000000;
    //printf("hope hope hope=%llu and uintmax_t = %d\n", hope, sizeof(uintmax_t));
    byte* revQA = revByteArray(helper.A,helper.u);
    //unsigned long long aa=numL(revQA,helper.u,10,mod);
    __uint128_t aa=hope_numL(revQA,helper.u,10);
    
    unsigned long long c=(aa+hope) % mod;
    printf("c the end=%llu\n", c);
    byte *c_arr=new byte [m];
    memset(c_arr,0,m);

    toByteArray(c_arr, m, base,c);
    revByteArray(c_arr,m,fake);
    
    memcpy(helper.A,helper.B,helper.v);
    memcpy(helper.B,c_arr,m);
    
    print_array(helper.A,helper.u);
    print_array(helper.B,helper.v);

    return 0;
}

int FF3::encrypt(byte *data, unsigned int data_len, byte *&out, int &out_len){

    if (data_len>16){
        cerr << "We are dealing with inputs larger to 16 bytes yet. Need for big integers support" << endl;
        //return -1;
    }
    
    //cout << "As long as numL is not using big int, this algo is prone to erros. Even if len <= 16" <<endl;
    Feistel feistel = Feistel(8);
    int m=0;
    
    //byte W[4]={0};
    //byte i_array[4]={0};
    
    

    FF3::ff3_helper helper = FF3::init(data,data_len);
    
    
    //cout <<" BEFORE begin n rounds" <<endl;
    //print_array(helper.A, helper.u);
    //print_array(helper.B, helper.v);
    //auto start = high_resolution_clock::now(); 
    for (int i=0; i<feistel.rounds; i++){
    //for (int i=0; i<1; i++){
        //cout << "i = " << i << endl;        
        FF3::encrypt_round(i,helper);
    }
    //auto end = high_resolution_clock::now(); 
    //auto duration = duration_cast<microseconds>(end - start); 
    //cout <<" AFTER n rounds" <<endl;
    //cout << "Time taken by function: " << dec << duration.count() << " microseconds" << endl; 
    //print_array(helper.A, helper.u);
    //print_array(helper.B, helper.v);
    out = new byte[helper.u+helper.v];
    out_len=helper.u+helper.v;

    //cipher = new byte[helper.u+helper.v];
    //memcpy(cipher, helper.A, helper.u);
    //memcpy(cipher+helper.u, helper.B, helper.v);

    memcpy(out, helper.A, helper.u);
    memcpy(out+helper.u, helper.B, helper.v);
    //print_array(cipher, cipher_len);




    return 0;
}


//i is the i-th round
//int FF3::execRound(int i, int u, int v, byte W[32], byte Tr[32], byte Tl[32]){
int FF3::decrypt_round(int i, FF3::ff3_helper helper){
    int fake=0;
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
    //cout << "OULOULOU i  array" << endl;
    int32toByteArray(i, i_array);
    //cout << "OULOULOU W^i" << endl;
    xorByteArrays(W,4, i_array, 4, tmp);
    
    
    //cout << "printing W,i,tmp=W^i" << endl;
    
    //print_array(i_array,4);
    //print_array(W,4);
    //print_array(tmp,4);
    
    //NUM_radix(REV(B))
    //cout << "printing v = " << dec << helper.v << endl;
    //cout << "printing u = " << dec << helper.u << endl;
    //print_array(helper.B,helper.v);
    
    //P right half
    byte* revA = revByteArray(helper.A, helper.u);
    //cout << "chelou numP array = " << endl;
    //print_array(rev,helper.v);
    //num_radix(tmp)^12
    unsigned long long numP = numL(revA,helper.u,this->base);
    //cout << "LA C TOUT FAUX au niveau de numL chelou numP = " << dec << numP << endl;
    byte *a=new byte[12];
    bigInt2ByteArray(numP,a,12);
    //print_array(a,12);

    byte *P = new byte[16];
    memcpy(P,tmp,4);
    memcpy(P+4,a,12);
    cout << "P   ";
    print_array(P,16);
    byte *revP = revByteArray(P,16);
    
    print_array(P,16);
    
    //S; REVB(P); aes_REV(K) 
    //AES aes(128);
    //print_array(key,16);
    byte *revKey=revByteArray(key,key_len);
    //print_array(key,16);

    unsigned int outLen=0;
    //DAFUK with IV in NIST doc
    //byte iv[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    byte *S = aes.EncryptECB(P, 16, revKey, outLen);
    //byte *S = aes.EncryptCBC(P, 16, key, iv, outLen);
    //print_array(S,16);
    byte *revS=revByteArray(S,outLen);
    
    print_array(S,16);

//    mpz_t z;
//EN FAIT JE VAIS APRES REDUIRE MODULO RADIX^M DC
//JE PEUX COMMENCER A CODER SALE AC un ERROR CASE ET ON VERRA APRES
//dc 2^64 should be more than enough for digits but then means forgetting some???
    //unsigned long a[20];
/* Initialize z and a */
    //mpz_import(z, 16, 1, sizeof(S[0]), 0, 0, S);
    //en fait je fais des trucs qui s'annulent j'ai limpression mais ca doit dependre de la base ds laque on est en fait
    //dc on ne reflechit pas on implemente step by step
    //unsigned long long s=numL(SS, 16, 256);
    __uint128_t s=hope_numL(revS, outLen, 256);
    //printf("s=%llu\n", s);
    unsigned long long mod=pow((double)base,(double)m);
    //printf("mwe %lld\n", mod);

    byte* revQB = revByteArray(helper.B,helper.v);
    //revByteArray(helper.A,helper.u, fake);
    __uint128_t bb=hope_numL(revQB,helper.v,10);
    
    unsigned long long c=(bb-s) % mod;
    byte *c_arr=new byte [m];
    memset(c_arr,0,m);

    toByteArray(c_arr, m, base,c);
    //print_array(c_arr,m);
    revByteArray(c_arr,m,fake);
    //print_array(c_arr,m);
    

    //Check sizes, cheloou un peu, tjrs v et pas u
    //cout << "Almost End of execRound " << i << endl;
    //print_array(helper.A,helper.u);
    //print_array(helper.B,helper.v);
    
    memcpy(helper.A,helper.B,helper.v);
    memcpy(helper.B,c_arr,m);
    
    //print_array(helper.A,helper.u);
    //print_array(helper.B,helper.v);
    
    //cout << "Before nd of execRound " << dec << c << endl;
    //cout << "End of execRound " << i << endl;



    return 0;
}


int FF3::decrypt(byte *data, unsigned int data_len, byte *&out, int &out_len){

    if (data_len>16){
        cerr << "We are dealing with inputs larger to 16 bytes yet. Need for big integers support" << endl;
        //return -1;
    }
    
    //cout << "As long as numL is not using big int, this algo is prone to erros. Even if len <= 16" <<endl;
    Feistel feistel = Feistel(8);
    int m=0;

    FF3::ff3_helper helper = FF3::init(data,data_len);
    
    //for (int i=2; i>0; i--){
    for (int i=feistel.rounds; i>0; i--){
        cout << "i = " << i << endl;        
        FF3::decrypt_round(i,helper);
    }
    cout << "GROS" << endl;
    //cipher = new byte[helper.u+helper.v];
    //cipher_len=helper.u+helper.v;
//
    //memcpy(cipher, helper.A, helper.u);
    //memcpy(cipher+helper.u, helper.B, helper.v);
    //print_array(cipher, cipher_len);

    out = new byte[helper.u+helper.v];
    out_len=helper.u+helper.v;

    memcpy(out, helper.A, helper.u);
    memcpy(out+helper.u, helper.B, helper.v);
    print_array(out,out_len);
    return 0;
}
