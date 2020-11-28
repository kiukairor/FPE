#ifndef _FEISTEL_H_
#define _FEISTEL_H_

#include "utils.h"

class Feistel
{

public:
    int block_len;
    int left_len;
    int right_len;
    int rounds;

//    Feistel(byte* data, int data_len, int left_len, int right_len, int rounds){
//    this->data=data;
//    this->data_len=data_len;
//    //this->left_block=left_block;
//    this->left_len=left_len;
//    this->right_len=right_len;
//    this->rounds=rounds;
//    }
    
    Feistel(int block_len, int left_len, int right_len, int rounds){
        this->rounds=rounds;
        this->block_len;
        this->left_len=left_len;
        this->right_len=right_len;
    }




    Feistel(int rounds){
        this->rounds=rounds;
    }

private:

    //byte *key;
    //unsigned key_len;

    int split_data(byte *data, unsigned int data_len);
    int round_operation();
    int prf(byte *data, unsigned int data_len, byte *&out, unsigned int &out_len, byte *key, unsigned int key_len, byte *iv);

};
#endif














