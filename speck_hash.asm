;
;  Copyright © 2017 Odzhan. All Rights Reserved.
;
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions are
;  met:
;
;  1. Redistributions of source code must retain the above copyright
;  notice, this list of conditions and the following disclaimer.
;
;  2. Redistributions in binary form must reproduce the above copyright
;  notice, this list of conditions and the following disclaimer in the
;  documentation and/or other materials provided with the distribution.
;
;  3. The name of the author may not be used to endorse or promote products
;  derived from this software without specific prior written permission.
;
;  THIS SOFTWARE IS PROVIDED BY AUTHORS "AS IS" AND ANY EXPRESS OR
;  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
;  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
;  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
;  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
;  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
;  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
;  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
;  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
;  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
;  POSSIBILITY OF SUCH DAMAGE.

; -----------------------------------------------
; Speck64/128 block cipher in x86 assembly
;
; size: 105 bytes (64 for just encryption) 
;
; global calls use cdecl convention
;
; -----------------------------------------------

    bits 32

%define SPECK_RNDS 27

;
; speck64/128 encryption in 64 bytes
; returns 32-bit ciphertext or "hash" in eax
;
%ifndef BIN
    global _speck32_hash
%endif

%define k0 edi    
%define k1 ebp    
%define k2 ecx    
%define k3 esi

%define x0 ebx    
%define x1 edx

speck64_encryptx:
_speck64_encryptx:    
    pushad    
    mov    esi, [esp+32+8]   ; esi = input
    push   esi               ; save
    
    lodsd
    xchg   eax, x0           ; x0 = in[0]
    lodsd
    xchg   eax, x1           ; x1 = in[1]
    
    mov    esi, [esp+32+8]   ; esi = key
    lodsd
    xchg   eax, k0           ; k0 = key[0] 
    lodsd
    xchg   eax, k1           ; k1 = key[1]
    lodsd
    xchg   eax, k2           ; k2 = key[2]
    lodsd 
    xchg   eax, k3           ; k3 = key[3]    
    xor    eax, eax          ; i = 0
spk_el:
    ; x0 = (ROTR32(x0, 8) + x1) ^ k0;
    ror    x0, 8
    add    x0, x1
    xor    x0, k0
    ; x1 = ROTL32(x1, 3) ^ x0;
    rol    x1, 3
    xor    x1, x0
    ; k1 = (ROTR32(k1, 8) + k0) ^ i;
    ror    k1, 8
    add    k1, k0
    xor    k1, eax
    ; k0 = ROTL32(k0, 3) ^ k1;
    rol    k0, 3
    xor    k0, k1    
    xchg   k3, k2
    xchg   k3, k1
    ; i++
    inc    eax
    cmp    al, SPECK_RNDS    
    jnz    spk_el
    
    pop    edi    
    xchg   eax, x0
    stosd
    xchg   eax, x1
    stosd
    popad
    ret

%endif    
