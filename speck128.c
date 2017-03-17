/**
  Copyright © 2017 Odzhan. All Rights Reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  3. The name of the author may not be used to endorse or promote products
  derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY AUTHORS "AS IS" AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */

#include "speck.h"

#ifndef SINGLE

void speck128_setkey(
    const void *in, 
    void *out)
{
  uint64_t i, t, k0, k1, k2, k3;

  // copy 256-bit key to local space
  k0 = ((uint64_t*)in)[0];
  k1 = ((uint64_t*)in)[1];
  k2 = ((uint64_t*)in)[2];
  k3 = ((uint64_t*)in)[3];

  // expand 256-bit key into round keys
  for (i=0; i<34; i++)
  {
    ((uint64_t*)out)[i] = k0;
    
    k1 = (ROTR64(k1, 8) + k0) ^ i;
    k0 = ROTL64(k0,  3) ^ k1;
    
    // rotate left 64-bits
    XCHG(k3, k2, t);
    XCHG(k3, k1, t);
  }
}

void speck128_encrypt(
    int enc, 
    void *in,
    const void *keys)
{
  uint64_t i;
  uint64_t *ks=(uint64_t*)keys;
  
  // copy input to local space
  uint64_t x = ((uint64_t*)in)[0];
  uint64_t y = ((uint64_t*)in)[1];
  
  for (i=0; i<34; i++)
  {
    if (enc == SPECK_DECRYPT)
    {
      x = ROTR64(x  ^ y, 3);
      y = ROTL64((y ^ ks[34-1-i]) - x, 8);        
    } else {
      y = (ROTR64(y, 8) + x) ^ ks[i];
      x = ROTL64(x, 3)  ^ y;
    }
  }
  // save result
  ((uint64_t*)in)[0] = x;
  ((uint64_t*)in)[1] = y;
}

#else

void speck128_encryptx(
    const void *key, 
    void *in)
{
  uint64_t i, t, k0, k1, k2, k3, x0, x1;
  
  // copy 256-bit key to local space
  k0 = ((uint64_t*)key)[0];
  k1 = ((uint64_t*)key)[1];
  k2 = ((uint64_t*)key)[2];
  k3 = ((uint64_t*)key)[3];
  
  // copy input to local space
  x0 = ((uint64_t*)in)[0];
  x1 = ((uint64_t*)in)[1];
  
  for (i=0; i<34; i++)
  {
    // encrypt block
    x1 = (ROTR64(x1, 8) + x0) ^ k0;
    x0 =  ROTL64(x0, 3) ^ x1;
    
    // create next subkey
    k1 = (ROTR64(k1, 8) + k0) ^ i;
    k0 =  ROTL64(k0, 3) ^ k1;

    XCHG(k3, k2, t);
    XCHG(k3, k1, t);    
  }
  // save result
  ((uint64_t*)in)[0] = x0;
  ((uint64_t*)in)[1] = x1;
}

#endif