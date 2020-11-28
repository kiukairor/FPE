#ifndef _UTILS_H_
#define _UTILS_H_

#include<iostream>
#include <string.h>
#include <cmath>

#include "aes.h"

using namespace std;

typedef unsigned char byte;

void sdeup();

void print_array(const char *in, int in_len);
void print_array(byte *in, int in_len);
int bytelen(string byteString);
int num(string str, int radix);
int num(string str);
string tostr(int str_len, int radix, int x);

string rev(string str);
string revB(string str);
byte* revByteArray(byte array[], int len_array);


void alloc_matrix(byte **&matrix, int rows, int columns);
////should remove out_len at some point, it will alway be 16 bytes=128 bits
////int prf(byte *data, int data_len, byte* out, unsigned int &out_len, byte *key, int key_len, byte *iv);
int prf(byte *data, unsigned int data_len, byte *&out, unsigned int &out_len, byte *key, unsigned int key_len, byte *iv);
int hexStr2int(char *c);
int convertDecStr2Arr(string str, byte *arr, int arr_len);
byte* retconvertDecStr2Arr(string str);
int xorByteArrays(byte *max, int max_len, byte *min, int min_len, byte *&r);
int int2byteArray(int val, byte *&res, int res_len);
int int2byteArray(int val, byte res[4]);
//
#endif