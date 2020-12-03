#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>
#include <unistd.h>
#include <signal.h>
#include<iostream>
#include <string.h>
#include <cmath>
#include "aes.h"
#include "utils.h"
//#include "feistel.h"
#include "ff3.h"

#include <openssl/aes.h>
#include <openssl/bn.h>
using namespace::chrono;


void handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}


int main(){
    signal(SIGSEGV, handler); 
    
    unsigned char plain[] = { 0x08, 0x09, 0x00, 0x01, 0x02, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x00, 0x00, 0x00 }; 
    
    unsigned char P[] = {205, 129, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 115, 10, 51, 250};

    
    //unsigned char plain[] = { 0x6a, 0x84, 0x86, 0x7c, 0xd7, 0x7e, 0x12, 0xad, 0x07, 0xea, 0x1b, 0xe8, 0x95, 0xc5, 0x3f, 0xa3 }; 
    //unsigned char plain[] = {0x2e, 0x58, 0x66, 0x92, 0xe6, 0x47, 0xf5, 0x02, 0x8e, 0xc6, 0xfa, 0x47, 0xa5, 0x5a, 0x2a, 0xab};
    //unsigned char plain[] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
    //                         0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51,
    //                         0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11, 0xE5, 0xFB, 0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF,
    //                         0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17, 0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10};
    //iv VERY BAD example
    //unsigned char iv[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff }; 
    //unsigned char iv[] = {0x25, 0x69, 0x53, 0xb2, 0xfe, 0xab, 0x2a, 0x04, 0xae, 0x01, 0x80, 0xd8, 0x33, 0x5b, 0xbe, 0xd6};
    //unsigned char iv[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    byte cipher[]= {0xDE, 0x7F, 0x3E, 0x4A, 0x21, 0x0E, 0x3D, 0xFF, 0x34, 0x69, 0x24, 0xFB, 0xFF, 0x75, 0x00, 0x39};

    //unsigned char revkey[] = {0x94, 0x6a, 0xfc, 0x04, 0x6f, 0x6d, 0x03, 0x7f, 0x4f, 0xa4, 0x80, 0xd5, 0xd8, 0x59, 0x43, 0xef};
    unsigned char key[] = {0xef, 0x43, 0x59, 0xd8, 0xd5, 0x80, 0xaa, 0x4f, 0x7f, 0x03, 0x6d, 0x6f, 0x04, 0xfc, 0x6a, 0x94};
                            //EF    43    59   D8     D5      80  AA     4F   7F    03    6D     6F   04    FC     6A     94
    unsigned int plain_len = sizeof(plain)/sizeof(plain[0]);  //bytes in plaintext
    unsigned int p_len = sizeof(P)/sizeof(P[0]);  //bytes in plaintext
    unsigned int k_len = sizeof(key)/sizeof(key[0]);
    unsigned int cipher_len = sizeof(cipher)/sizeof(cipher[0]);  //bytes in plaintext
    
    unsigned int outLen = 0;  // out param - bytes in сiphertext


    unsigned char *fdp=revByteArray(P, p_len);
    unsigned char *revKey=revByteArray(key, k_len);

    AES aes(128);  //128 - key length, can be 128, 192 or 256
    //unsigned char *c = aes.EncryptCBC(plain, plain_len, key, iv, outLen);
//    unsigned char *c = aes.EncryptECB(P, p_len, key, outLen);
        unsigned char *c1 = aes.EncryptECB(P, p_len, revKey, outLen);
        unsigned char *c2 = aes.EncryptECB(fdp, p_len, key, outLen);
        unsigned char *dec = aes.DecryptECB(cipher, cipher_len, revKey);
//    unsigned char *c3 = aes.EncryptECB(fdp, p_len, revKey, outLen);
//    cout << "sizeof(plain) = " << dec << outLen <<endl;
//    print_array(P, p_len);
//    print_array(fdp, p_len);
//    print_array_hex(c, outLen);
//    print_array_hex(c1, outLen);
//    print_array(c1, outLen);
//    print_array_hex(c2, outLen);
//    print_array(c2, outLen);


//    cout << "cipher = " <<endl;
//    print_array_hex(cipher,cipher_len); 
//    cout << " for plain = " << endl;
//    print_array_hex(dec,cipher_len);
//	string dec_str="109876";    
//    byte *arr=new byte[dec_str.length()];

    byte *out;
    int out_len=0;

    byte *rec;
    int rec_len=0;
    int key_len=sizeof(key);
    //int res = prf(plain,plain_len, out, outLen, key, key_len,iv);
    //int res = prf(plain, plain_len, out, outLen, key, key_len, iv);
    byte tweak[8]={0xD8, 0xE7, 0x92, 0x0A, 0xFA, 0x33, 0x0A, 0x73};    
    //byte tweak[8]={0x9a, 0x76, 0x8a, 0x92, 0xf6, 0x0e, 0x12, 0xd8};
    
    

    FF3 ff3 = FF3(key, key_len, 10, tweak);
    //FF3::ff3_helper helper = ff3.init(plain, plain_len);
    
    
   //int iter=1;
   //cout << "dafuk data_len = " << dec << plain_len;
   //auto start = high_resolution_clock::now(); 
   //for (int i=0;i<iter; i++){
       int r = ff3.encrypt(plain, plain_len, out, out_len);
       cout << " will decrypt" <<endl;
       r=ff3.decrypt(out,out_len, rec,rec_len);
   //}
   //
   //auto end = high_resolution_clock::now(); 
   //auto duration = duration_cast<microseconds>(end - start); 
   //cout << "Time taken by function: " << dec << duration.count()/iter << " microseconds" << endl; 

return 0;
}













