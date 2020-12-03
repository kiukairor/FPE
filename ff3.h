#ifndef _FF3_H_
#define _FF3_H_

#include "feistel.h"
#include "utils.h"
#include <gmpxx.h>
#include <openssl/bn.h>

using namespace std;


//missing max values and error cases
class FF3
{
    //private:
        //Feistel feistel;
        
        
        //int init(byte *data, unsigned int data_len);
        //void execRound();

    public:
        
        //FF3(Feistel feistel, byte *key, unsigned key_len, int base, byte *tweak){
        //    this->feistel=feistel;
        FF3(byte *key, unsigned key_len, int base, byte *tweak){
            this->key=key;
            this->key_len=key_len;
            this->base=base;
            this->tweak=tweak;
            this->tweak_len=56;;

        }
        
        struct ff3_helper{
            int u; //A len
            int v; //B len 
            byte *A;
            byte *B;
            byte Tl[4];
            byte Tr[4];
        };


        byte *key;
        byte *tweak;
        byte *cipher;
        AES aes;
        int cipher_len;
        int base;
        int key_len;
        int tweak_len;


        //ff3_helper init(byte *data, unsigned int data_len);
        ff3_helper init(byte *data, unsigned int data_len);
        int encrypt_round(int i, FF3::ff3_helper helper);
        int encrypt(byte *data, unsigned int data_len, byte *&out, int &out_len);
        
        int decrypt_round(int i, FF3::ff3_helper helper);
        int decrypt(byte *data, unsigned int data_len,byte *&out, int &out_len);


        //should not be here
        
        
        
        
        

        
};
#endif