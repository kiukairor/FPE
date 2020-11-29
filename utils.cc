#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#include "utils.h"

void sdeup(){
    cout << "hello sdeup" << endl;
}

void print_array(const char *in, int in_len){

    for (int i=0; i<in_len-1; i++){
        printf("%d, ", in[i]);
    }
    printf("%d\n", in[in_len-1]);
    
return;
}

void print_array(byte *in, int in_len){

    for (int i=0; i<in_len-1; i++){
        printf("%d, ", in[i]);
    }
    printf("%d\n", in[in_len-1]);
    
return;
}

int bytelen(string byteString){
    return byteString.length();
}

int num(string str, int radix){
    int x=0;

    for (int i=0; i<str.length(); i++){
        x=x*radix+str[i];
    }

    return x;
}

int num(byte *arr, int arr_len, int radix){
    int x=0;

    for (int i=0; i<arr_len; i++){
        x=x*radix+arr[i];
    }

    return x;
}



int num(string str){
    int x=0;

    for (int i=0; i<str.length(); i++){
        x = (x<<2) + str[i];
    }

    return x;
}

//redo
string tostr(int str_len, int radix, int x){

    byte *tmp=new byte[str_len];
    if ( x >= (radix^str_len) ){
        return "";
    }

    for (int i=0;i<str_len;i++){
        tmp[str_len-1-i]=x%radix;
        x=x/radix;
    }

    //string str(tmp);
    string str( reinterpret_cast< char const* >(tmp) ) ;
    delete[] tmp;
    return str;

}


string rev(string str){
    
    std::string y(str.length(), '*');
    for (int i=0; i<str.length(); i++){
        y[i]=str[str.length()-1-i];
    }

    return y;
}

string revB(string str){
    
    string y(str.length(),'*');
    for (int i=0; i<str.length(); i++){
        y[i]=str[str.length()-1-i];
    }

    return y;
}

byte* revByteArray(byte array[], int len_array){

    byte *r = new byte[len_array];
    
    for (int i=0; i < len_array; i++){
        r[i]=array[len_array-1-i];
    }    

    return r;
}


int revByteArray(byte *&array, int len_array, int fake){

    byte *r = new byte[len_array];
    
    for (int i=0; i < len_array; i++){
        r[i]=array[len_array-1-i];
    }    

    memcpy(array,r,len_array);
    delete[] r;
    return 0;
}



void alloc_matrix(byte **&matrix, int rows, int columns){
    matrix = new byte*[rows];
    //matrix =  (byte **) calloc(sizeof(byte *), rows);
    
    for (int i=0; i < rows; i++){
      //  matrix[i] = (byte *) calloc(sizeof(byte),columns);
      matrix[i] = new byte [columns];
    }

    return;
}


int prf(byte *data, unsigned int data_len, byte *&out, unsigned int &out_len, byte *key, unsigned int key_len, byte *iv){
    
    
    if (key_len != 16){
        perror("wrong size key");
        return -1;
    }
    
    AES aes(key_len*8);
    byte tmp[16]={0};
    //split in blocks of 128 bits = 16 bytes
    int block_len=data_len/16;
    
    byte **X= NULL;

    alloc_matrix(X, block_len,16);
    
    for (int i=0; i < block_len; i++){
        memcpy(X[i], data+16*i,16);
        //cout << "after mempy i = " << i <<endl;
    }
    
    byte **Y=NULL;;// = new byte[block_len+1][16];
    alloc_matrix(Y, block_len+1,16);
    memset(Y[0],0,16);

    //0^128 = 128 bits = 16 bytes set to 0    
    // MUST BE OPTIMIZED NO NEED TO STORE ALL PREVIOUS VALUES
    for (int i=1; i < block_len+1; i++){
        for (int k=0; k<16; k++){
            tmp[k]=(byte) ((int) Y[i-1][k]) ^ ((int) X[i-1][k]);    
        }
        //tmp= Y[i-1]^X[i-1];
        Y[i]=aes.EncryptCBC(tmp,16,key,iv,out_len);
    }

    //16=aes->Nb*sizeof(byte);
    cout << "ZEUBI" <<endl;
    memcpy(out,Y[block_len],16);
    cout << "GROS FDP" <<endl;
    
    //print_array(Y[block_len],16);
    for (int i=0; i<block_len; i++)
        delete[] X[i];
    delete[] X;

    for (int i=0; i<block_len+1; i++)
        delete[] Y[i];
    delete[] Y;

    print_array(out,16);
    
    return 0;

}

int hexStr2int(char *c){
    char *endptr;
    long res;

    res=strtol(c, &endptr, 16);

    return res;
}

int convertDecStr2Arr(string str, byte *arr, int arr_len){
    
    if (str.length() != arr_len){
        return -1;
    }
    
    for (int i=0; i < arr_len ; i++){
        arr[i]=(int)str.data()[i]- (int)'0';
    }

    cout << "end" <<endl;
    return 0;
}

byte* retconvertDecStr2Arr(string str){

    
    byte *arr=new byte[str.length()];
    for (int i=0; i < str.length() ; i++){
        arr[i]=(int)str.data()[i]- (int)'0';
        printf("%d\n", arr[i]);
    }

    return arr;
}
//
////a and b should be of byte size >= min_len
int xorByteArrays(byte *max, int max_len, byte *min, int min_len, byte *&r){
    int len = (max_len >min_len) ? max_len : min_len;
    cout << "len = " << len << endl;
    r=new byte[max_len];
    for (int i=min_len-1, j=max_len-1; i>-1; i--,j--){
        r[j]=max[j]^min[i];
        printf("max[i]=%d, min= %d, r=%d\n", r[j],max[j],min[i]);
    }

    return 0;

}


int int32toByteArray(int val, byte res[4]){
    
    // int = 4 bytes = 4*8 bits...
    res[3] = (val << 24) >> 24;
    res[2] = (val << 16) >> 24;
    res[1] = (val << 8) >> 24;
    res[0] = val >> 24;
    
    return 0; 
}


//val could be very >> 2^32, deal with that later
//depends on the plain and the radix
int bigInt2ByteArray(int in, byte *&res, int res_len){
    
    //byte *res=new byte[res_len]={0};
    memset(res,0,res_len);
    unsigned int val=in;
    for (int i=0; i < res_len; i++){
        res[res_len-1-i]=in%256;
        in=in/256;
    }
    print_array(res,res_len);

    return 0; 
}
