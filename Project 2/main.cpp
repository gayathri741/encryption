//
//  main.cpp
//  Project 2
//
//  Created by Gayatri Sudheer on 2019-11-07.
//  Copyright © 2019 Gayatri Sudheer. All rights reserved.
/*
 Encode:
 - scrambling a set of numbers (0,255) -> S
 - plaintext into cipher text (XOR with R, which is an element)
 - converting ciphertext so that yo can print it -> ASCII armour
 
 increment i modulo 256 => increment i, then modulo 256 (i= (i+1)%256)
 
 Dynamic memory allocation for array:
 example: p_int = new int[10];
 
 int encrypted[256];
 encrypted[n] = S[n] ^ R;
*/

#include <iostream>
#include <cmath>

#ifndef MARMOSET_TESTING
int main();
#endif

char *encode(char *plaintext, unsigned long key);
unsigned long length(char plaintext[]);
char *decode(char *ciphertext, unsigned long key);

#ifndef MARMOSET_TESTING
int main() {
    char str0[]{"Hello world!"};
    char *ciphertext{ encode( str0, 51323) };
    std::cout << "\"" << ciphertext << "\"" << std::endl;
    char *plaintext{ decode(ciphertext, 51323) };
    std::cout << "\"" << plaintext << "\"" << std::endl;
    delete[] plaintext;
    plaintext = nullptr;
    delete[] ciphertext;
    ciphertext = nullptr;
    return 0;
}
#endif


unsigned long length(char plaintext[]){
    long c{0};
    long counter{0};
    while (plaintext[c]!='\0'){
        ++counter;
        ++c;
    }
    return counter;
}

char *encode(char *plaintext, unsigned long key){
    unsigned char S[256];
    unsigned long size = length(plaintext);
    
    //to add appropriate nullspaces when plaintext is not divisible by 4
    if (size%4 !=0){
        int h= 4-size%4;
        size = size+h;
    }
    unsigned int *scramble{new unsigned int[size]{'\0'}}; //to dynamically allocate the xor'ed plaintext into an array
    
    for (unsigned int l{0}; l<256; ++l)
        S[l]= static_cast<unsigned char>(l);
    //scramble
    int i{0};
    int j{0};
    for (int m{0}; m<256 ; ++m){
        int k = i%64;
        char temp;
        j = (j + static_cast<unsigned int>(S[i]) +((key >> k)&1))%256;
        temp = S[i];
        S[i] = S[j];
        S[j] = temp;
        i = (i+1)%256;
    }
    std::cout << "i= " << i << std::endl;
    std::cout << "j= " << j << std::endl;

    int R{0};
    int r{0};
     
    for (unsigned long n{0}; n<size; ++n){
        i = (i+1)%256;
        j = (j + static_cast<unsigned int>(S[i]))%256;
        unsigned char temp{0};
        temp = S[i];
        S[i] = S[j];
        S[j] = temp;
        r = (static_cast<unsigned int>(S[i]) + static_cast<unsigned int>(S[j]))%256;
        R = S[r];
        scramble[n] = plaintext[n] ^ R;
    }
    for (long h{0}; h<size; ++h)
        std::cout << scramble[h] << " ";
    std::cout << std::endl;
    //ascii armour
    char *ciphertext{new char[5*(size/4)+1]{}};
    long count{0};
    long m{0};
    for (long k{0}; k<size; k=k+4){
        m = k + count;
        unsigned long temp{0};
        temp=(static_cast<unsigned int>(scramble[k]) << 24)+(static_cast<unsigned int>(scramble[k+1]) << 16) + (static_cast<unsigned int>(scramble[k+2]) << 8) + (static_cast<unsigned int>(scramble[k+3]));
        std::cout << temp << std::endl;
        ciphertext[m+4]= (temp%85)+'!';
        temp /= 85;
        ciphertext[m+3]= (temp%85)+'!';
        temp /= 85;
        ciphertext[m+2]= (temp%85)+'!';
        temp /= 85;
        ciphertext[m+1]= (temp%85)+'!';
        temp /= 85;
        ciphertext[m]= (temp%85)+'!';
        ++count;
    }
    delete[] scramble;
    scramble = nullptr;
    
    return ciphertext;
}
char *decode(char *ciphertext, unsigned long key){
    long size = length(ciphertext);
    long k{0};
    long count{0};
    char *scramble{new char[4*(size/5)]{}}; //to dynamically allocate the un-ascii armoured data into an array
    //reversing ascii armour
    for (long m{0}; m<size; m=m+5){
        k = m - count;
        unsigned long temp{0};
        temp = (static_cast<unsigned int>(ciphertext[m+4])-'!')+((static_cast<unsigned int>(ciphertext[m+3])-'!')*85)+((static_cast<unsigned int>(ciphertext[m+2])-'!')*85*85)+((static_cast<unsigned int>(ciphertext[m+1])-'!')*85*85*85)+((static_cast<unsigned int>(ciphertext[m])-'!')*85*85*85*85);
        scramble[k] = temp >> 24;
        scramble[k+1] = temp >> 16;
        scramble[k+2] = temp >> 8;
        scramble[k+3] =  temp;
        ++count;
    }
    
    size = 4*(size/5);
    unsigned char S[256];
    for (unsigned int l{0}; l<256; ++l)
        S[l]= static_cast<unsigned char>(l);
    int i{0};
    int j{0};
    for (int m{0}; m<256 ; ++m){
        int k = i%64;
        char temp;
        j = (j + static_cast<unsigned int>(S[i]) +((key >> k)&1))%256;
        temp = S[i];
        S[i] = S[j];
        S[j] = temp;
        i = (i+1)%256;
    }
    int R{0};
    int r{0};
    char *plaintext{new char[size]{}};
    for (unsigned long n{0}; n<size; ++n){
        i = (i+1)%256;
        j = (j + static_cast<unsigned int>(S[i]))%256;
        unsigned char temp{0};
        temp = S[i];
        S[i] = S[j];
        S[j] = temp;
        r = (static_cast<unsigned int>(S[i]) + static_cast<unsigned int>(S[j]))%256;
        R = (S[r]);
        plaintext[n] = static_cast<unsigned int>(scramble[n]) ^ R;
    }
   
    delete[] scramble;
    scramble = nullptr;
    
    return plaintext;
}

