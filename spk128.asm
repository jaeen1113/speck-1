;
;  Copyright © 2017 Odzhan, Peter Ferrie. All Rights Reserved.
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
; Speck128/256 block cipher in x86-64 assembly
;
; size: 105 bytes (88 for just encryption) 
;
; global calls use microsoft fastcall convention
;
; -----------------------------------------------

    bits 64

%define SPECK_RNDS 34
    
%ifndef SINGLE
    
%ifndef BIN
    global speck128_setkey
    global speck128_encrypt
%endif
    
%define k0 rdi    
%define k1 rbx    
%define k2 rsi    
%define k3 rcx
    
speck128_setkey:
    push   rbx
    push   rdi
    push   rsi   

    mov    k0, [rcx]         ; k0 = key[0]
    mov    k1, [rcx+8]       ; k1 = key[1]
    mov    k2, [rcx+16]      ; k2 = key[2]
    mov    k3, [rcx+24]      ; k3 = key[3]

    xor    eax, eax
spk_sk:
    ; ((uint32_t*)ks)[i] = k0;
    mov    [rdx+rax*8], k0
    ; k1 = (ROTR32(k1, 8) + k0) ^ i;
    ror    k1, 8
    add    k1, k0
    xor    k1, rax
    ; k0 = ROTL32(k0, 3) ^ k1;
    rol    k0, 3
    xor    k0, k1
    ; rotate left 32-bits
    xchg   k3, k2
    xchg   k3, k1
    ; i++
    add    al, 1
    cmp    al, SPECK_RNDS    
    jnz    spk_sk   
    
    pop    rsi
    pop    rdi
    pop    rbx
    ret

%define x0 rax    
%define x1 rbx
    
speck64_encrypt:
    push   rbx
    push   rdi
    push   rsi
    
    push   rdx
    mov    x0, [rdx]         ; x0 = in[0]
    mov    x1, [rdx+8]       ; x1 = in[1] 
    
    xchg   eax, x1
    test   ecx, ecx
    mov    cl, SPECK_RNDS
    jz     spk_e0
spk_d0:
    ; x1 = ROTR32(x1 ^ x0, 3);
    xor    x1, x0
    ror    x1, 3
    ; x0 = ROTL32((x0 ^ ks[SPECK_RNDS-1-i]) - x1, 8);
    xor    x0, [edi+4*ecx-4]
    sub    x0, x1
    rol    x0, 8
    loop   spk_d0
    jmp    spk_end    
spk_e0:
    ; x0 = (ROTR32(x0, 8) + x1) ^ ks[i];
    ror    x0, 8
    add    x0, x1
    xor    x0, [edi]
    scasd
    ; x1 = ROTL32(x1, 3) ^ x0;
    rol    x1, 3
    xor    x1, x0
    loop   spk_e0
spk_end:
    pop    edi
    ; ((uint32_t*)in)[0] = x0;
    stosd
    xchg   eax, x1
    ; ((uint32_t*)in)[1] = x1;
    stosd    
    pop    rsi
    pop    rdi
    pop    rbx
    ret   
    
%else

;
; speck128/256 encryption in 88 bytes
;
%ifndef BIN
    global speck128_encryptx
%endif

%define k0 rdi    
%define k1 rbp    
%define k2 rsi    
%define k3 rcx

%define x0 rbx    
%define x1 rdx

speck128_encryptx:   
    push   rbp
    push   rbx
    push   rdi
    push   rsi   

    mov    k0, [rcx]         ; k0 = key[0]
    mov    k1, [rcx+8]       ; k1 = key[1]
    mov    k2, [rcx+16]      ; k2 = key[2]
    mov    k3, [rcx+24]      ; k3 = key[3]
    
    push   rdx
    mov    x0, [rdx]         ; x0 = in[0]
    mov    x1, [rdx+8]       ; x1 = in[1] 
    
    xor    eax, eax          ; i = 0
spk_el:
    ; x1 = (ROTR64(x1, 8) + x0) ^ k0;
    ror    x1, 8
    add    x1, x0
    xor    x1, k0
    ; x0 =  ROTL64(x0, 3) ^ x1;
    rol    x0, 3
    xor    x0, x1
    ; k1 = (ROTR64(k1, 8) + k0) ^ i;
    ror    k1, 8
    add    k1, k0
    xor    k1, rax
    ; k0 = ROTL64(k0, 3) ^ k1;
    rol    k0, 3
    xor    k0, k1
    
    xchg   k3, k2
    xchg   k3, k1
    ; i++
    add    al, 1
    cmp    al, SPECK_RNDS    
    jnz    spk_el
    
    pop    rax
    mov    [rax], x0
    mov    [rax+8], x1
    
    pop    rsi
    pop    rdi
    pop    rbx
    pop    rbp
    ret

%endif    