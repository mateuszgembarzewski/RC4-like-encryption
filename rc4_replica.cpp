#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
using namespace std;

/* convert a string to an array of unsigned chars */
unsigned char* str2char(string in) {
    unsigned char *out;
    out = (unsigned char*)calloc(in.length(), sizeof(char));
    for (int i=0; i < in.length(); i++)
        out[i] = in.at(i);
    return out;
}

/* convert an array of unsigned chars to a hex string */
string char2str(unsigned char* in, size_t insize) {
    stringstream out;
    out << setfill('0') << setw(2) << hex;
    for (int i=0; i < insize; i++) out << (unsigned int)in[i];

    return out.str();
}

/* swaps the values of two unsigned chars */
void swap(unsigned char* a, unsigned char* b) {
    unsigned char tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Implements RC4 key scheduling algorithm
 */
//The permutation is initialized with a variable key length
//typically between 40-2048 bits

unsigned char* ksa(unsigned char* key, size_t keylength) {
    int j = 0;
    int i = 0;
    unsigned char *s;
    s = (unsigned char*)calloc(256, sizeof(char));

    for(int i = 0; i<256; i++) {
        s[i] = i;
    }

    for(int i = 0; i<256 ; i++) {

        j = (j + s[i] + key[i % keylength]) % 256;
        swap(s[i], s[j]);
    }

    return s;
}
//store return  unsigned char
unsigned char* prga(unsigned char* s, size_t numchars) {
    int i = 0;
    int j = 0;
    unsigned char *k;
    k = (unsigned char*)calloc(numchars,sizeof(char));
    
    for (int h = 0; h < numchars; h++){
        i = (i + 1) % 256;
        j = (j + s[i]) % 256;
        swap(s[i],s[j]);
        k[h] =s[(s[i] + s[j]) % 256];
        }

    return k;
    }

unsigned char* encrypt(unsigned char *plaintext, size_t plainsize,
                       unsigned char *key, size_t keysize) {
    
    unsigned char *p;
    p = ksa(key,keysize);
    unsigned char *k;
    k = prga(p, plainsize);
    unsigned char *c;
    c = (unsigned char*)calloc(plainsize, sizeof(char));

    for(int i=0; i < plainsize; i++) {

        c[i] = plaintext[i] ^ k[i];

    }
    
    return c;
}

unsigned char* decrypt(unsigned char *ciphertext, size_t ciphersize,
                       unsigned char *key, size_t keysize) {
    return encrypt(ciphertext,ciphersize,key,keysize);
}

int main() {
    string keystr("Secret");
    string plainstr("Attack at dawn");

    unsigned char *cipher;
    cipher = encrypt(str2char(plainstr), plainstr.length(),
                     str2char(keystr), keystr.length());

    cout << char2str(cipher, plainstr.length()) << endl;
    unsigned char c[] = {0xbb, 0xf3, 0x16, 0xe8, 0xd9, 0x40, 0xaf, 0x0a, 0xd3};
    printf("%s\n", decrypt(c, 9, str2char("Key"), 3));
}
