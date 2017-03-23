

// test unit for speck
// odzhan

#include <stdio.h>
#include <string.h>

#include "speck.h"

void print_bytes(char *s, void *p, int len) {
  int i;
  printf("%s : ", s);
  for (i=0; i<len; i++) {
    printf ("%02x ", ((uint8_t*)p)[i]);
  }
  putchar('\n');
}

// p = 0x3b7265747475432d 
uint8_t plain32[]=
{ 0x74, 0x65, 
  0x4c, 0x69 };

// c = 0x8c6fa548454e028b  
uint8_t cipher32[]=
{ 0x68, 0xa8, 
  0xf2, 0x42 };

// key = 0x03020100, 0x0b0a0908, 0x13121110, 0x1b1a1918   
uint8_t key32[]=
{ 0x00, 0x01, 
  0x08, 0x09,
  0x10, 0x11, 
  0x18, 0x19 };
  
// p = 0x3b7265747475432d 
uint8_t plain64[]=
{ 0x74, 0x65, 0x72, 0x3b,
  0x2d, 0x43, 0x75, 0x74 };

// c = 0x8c6fa548454e028b  
uint8_t cipher64[]=
{ 0x48, 0xa5, 0x6f, 0x8c, 
  0x8b, 0x02, 0x4e, 0x45 };

// key = 0x03020100, 0x0b0a0908, 0x13121110, 0x1b1a1918   
uint8_t key64[]=
{ 0x00, 0x01, 0x02, 0x03,
  0x08, 0x09, 0x0a, 0x0b,
  0x10, 0x11, 0x12, 0x13,
  0x18, 0x19, 0x1a, 0x1b };
  
// 1b1a1918 13121110 0b0a0908 03020100
  
// p = 65 73 6f 68 74 20 6e 49 
//     20 2e 72 65 6e 6f 6f 70 
uint8_t plain128[]=
{ 0x70, 0x6f, 0x6f, 0x6e, 0x65, 0x72, 0x2e, 0x20,
  0x49, 0x6e, 0x20, 0x74, 0x68, 0x6f, 0x73, 0x65 };

// c = 41 09 01 04 05 c0 f5 3e 
//     4e ee b4 8d 9c 18 8f 43  
uint8_t cipher128[]=
{ 0x43, 0x8f, 0x18, 0x9c, 0x8d, 0xb4, 0xee, 0x4e, 
  0x3e, 0xf5, 0xc0, 0x05, 0x04, 0x01, 0x09, 0x41 };

// key = 1f 1e 1d 1c 1b 1a 19 18 
//       17 16 15 14 13 12 11 10 
//       0f 0e 0d 0c 0b 0a 09 08 
//       07 06 05 04 03 02 01 00 
uint8_t key128[]=
{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
  
typedef struct _test_t {
  int     keylen;
  uint8_t *key;
  int     blocklen;  
  uint8_t *plain;
  uint8_t *cipher;
} test_t;

// test vectors
test_t tv[3]=
{{8,  key32,   4, plain32,  cipher32},
 {16, key64,   8, plain64,  cipher64},
 {32, key128, 16, plain128, cipher128}};
  
int main (void)
{
  uint64_t subkeys[34], buf[2];
  int      equ, i;
  
  for (i=0; i<sizeof(tv)/sizeof(test_t); i++)
  {
    printf ("******************************");
    // copy plaintext to local buffer
    memcpy (buf, tv[i].plain, tv[i].blocklen);
  
    printf ("\nSPECK%i/%i\n", 
        tv[i].blocklen*8, tv[i].keylen*8);
    
    print_bytes("K ", tv[i].key,    tv[i].keylen);
    print_bytes("PT", tv[i].plain,  tv[i].blocklen);
    print_bytes("CT", tv[i].cipher, tv[i].blocklen);

    if (tv[i].blocklen == 4)
    {      
      speck32_setkey (tv[i].key, subkeys);
      speck32_encrypt (subkeys, SPECK_ENCRYPT, buf);
      //speck32_encryptx (tv[i].key, buf);    
    } else if (tv[i].blocklen == 8)
    {      
      speck64_setkey (tv[i].key, subkeys);
      speck64_encrypt (subkeys, SPECK_ENCRYPT, buf);
      //speck64_encryptx (tv[i].key, buf);
    } else {
      speck128_setkey (tv[i].key, subkeys);
      speck128_encrypt (SPECK_ENCRYPT, buf, subkeys);
      //speck128_encryptx(tv[i].key, buf);
    }
    
    equ = memcmp(tv[i].cipher, buf, tv[i].blocklen)==0;
    
    printf ("\nEncryption %s\n", equ ? "OK" : "FAILED");
    print_bytes("CT", buf, tv[i].blocklen);
    
    if (tv[i].blocklen == 4)
    {      
      speck32_setkey (tv[i].key, subkeys);
      speck32_encrypt (subkeys, SPECK_DECRYPT, buf);
    } else if (tv[i].blocklen == 8)
    {      
      speck64_setkey (tv[i].key, subkeys);
      speck64_encrypt (subkeys, SPECK_DECRYPT, buf);
    } else {
      speck128_setkey (tv[i].key, subkeys);
      speck128_encrypt (SPECK_DECRYPT, buf, subkeys);
    }    
    
    equ = memcmp(tv[i].plain, buf, tv[i].blocklen)==0;
    
    printf ("\nDecryption %s\n", equ ? "OK" : "FAILED");
    print_bytes("PT", buf, tv[i].blocklen);
  }
  return 0;
}
